#include "SimonPuzzle.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "SimonMonolith.h"
#include "SimonStepPad.h"
#include "CandyCollectible.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h" // (optional) for visualizing traces

// ---- Helper: find a grounded spawn transform near a monolith ----
// Places the spawn a bit in front of the monolith, raycasts down to sit on the ground,
// then adds a small vertical lift so it doesn't clip into the floor.
static bool GetGroundedSpawnTransform(ASimonMonolith* Mono, FTransform& OutXform,
                                      float ForwardOffset = 60.f,
                                      float DownCast = 400.f,
                                      float StartLift = 120.f,
                                      float FinalLift = 8.f,
                                      bool bDebug = false)
{
    if (!Mono) return false;

    const FVector Fwd = Mono->GetActorForwardVector();
    const FVector Start = Mono->GetActorLocation() + Fwd * ForwardOffset + FVector(0, 0, StartLift);
    const FVector End   = Start - FVector(0, 0, DownCast);

    FHitResult Hit;
    FCollisionQueryParams Params(TEXT("SimonGroundTrace"), false, Mono);
    const bool bHit = Mono->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    FVector SpawnLoc;
    if (bHit)
    {
        SpawnLoc = Hit.ImpactPoint + FVector(0, 0, FinalLift);
    }
    else
    {
        // Fallback if no ground: just place in front with a small lift
        SpawnLoc = Mono->GetActorLocation() + Fwd * ForwardOffset + FVector(0, 0, FinalLift);
    }

    if (bDebug)
    {
        DrawDebugLine(Mono->GetWorld(), Start, End, FColor::Yellow, false, 1.5f, 0, 1.5f);
        DrawDebugSphere(Mono->GetWorld(), SpawnLoc, 8.f, 12, bHit ? FColor::Green : FColor::Red, false, 1.5f);
    }

    OutXform = FTransform(Mono->GetActorRotation(), SpawnLoc);
    return true;
}

ASimonPuzzle::ASimonPuzzle()
{
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    ActivationPad = CreateDefaultSubobject<UBoxComponent>(TEXT("ActivationPad"));
    ActivationPad->SetupAttachment(RootComponent);
    ActivationPad->InitBoxExtent(FVector(80, 80, 30));
    ActivationPad->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ActivationPad->SetCollisionResponseToAllChannels(ECR_Ignore);
    ActivationPad->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ASimonPuzzle::BeginPlay()
{
    Super::BeginPlay();
    ActivationPad->OnComponentBeginOverlap.AddDynamic(this, &ASimonPuzzle::HandleActivation);

    // Link pads back to this puzzle (optional if set in editor)
    for (ASimonStepPad* Pad : StepPads)
    {
        if (Pad) { Pad->Puzzle = this; }
    }

    AllOff();
}

void ASimonPuzzle::HandleActivation(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!Cast<ACharacter>(OtherActor)) return;
    if (State == ESimonState::Showing || State == ESimonState::AwaitInput || State == ESimonState::Solved) return;

    GenerateSequence();
    StartShowing();
}

void ASimonPuzzle::GenerateSequence()
{
    Sequence.Empty();

    const int32 N = Monoliths.Num();
    if (N <= 0) return;

    // Don’t allow more steps than distinct monoliths
    const int32 L = FMath::Clamp(SequenceLength, 1, N);

    // Build a pool [0..N-1], shuffle it, then take the first L
    TArray<int32> Pool;
    Pool.Reserve(N);
    for (int32 i = 0; i < N; ++i) Pool.Add(i);

    // Fisher–Yates shuffle
    for (int32 i = N - 1; i > 0; --i)
    {
        const int32 j = FMath::RandRange(0, i);
        Pool.Swap(i, j);
    }

    Sequence.Append(Pool.GetData(), L);  // take first L unique indices
    Expect = 0;
}

void ASimonPuzzle::StartShowing()
{
    State = ESimonState::Showing;
    AllOff();
    ShowIdx = -1;

    GetWorldTimerManager().SetTimer(ShowTimer, this, &ASimonPuzzle::ShowTick, ShowOnTime + ShowOffGap, true);
    ShowTick(); // immediate first tick
}

void ASimonPuzzle::ShowTick()
{
    // Turn previous off
    if (ShowIdx >= 0 && ShowIdx < Monoliths.Num() && Monoliths[ShowIdx])
    {
        Monoliths[ShowIdx]->SetState(EMonolithState::Off);
    }

    // Advance
    ShowIdx++;
    if (!Sequence.IsValidIndex(ShowIdx))
    {
        // Done showing → allow input
        GetWorldTimerManager().ClearTimer(ShowTimer);
        State = ESimonState::AwaitInput;
        AllOff();
        Expect = 0;
        return;
    }

    // Light current
    const int32 Idx = Sequence[ShowIdx];
    if (Monoliths.IsValidIndex(Idx) && Monoliths[Idx])
    {
        Monoliths[Idx]->SetState(EMonolithState::Showing);

        // Auto-turn-off after ShowOnTime (so the visible pulse matches your ShowOnTime)
        FTimerHandle Tmp;
        ASimonMonolith* M = Monoliths[Idx];
        GetWorldTimerManager().SetTimer(Tmp, [M]()
        {
            if (M && M->GetState() == EMonolithState::Showing)
            {
                M->SetState(EMonolithState::Off);
            }
        }, ShowOnTime, false);
    }
}

void ASimonPuzzle::AllOff()
{
    for (ASimonMonolith* M : Monoliths)
    {
        if (M) { M->SetState(EMonolithState::Off); }
    }
}

void ASimonPuzzle::OnPlayerStep(int32 PadIndex)
{
    if (State != ESimonState::AwaitInput) return;

    const double Now = GetWorld()->GetTimeSeconds();
    if (Now < NextAllowedInputTime) return;

    // ✅ If this pad’s monolith is already locked, ignore the press
    if (Monoliths.IsValidIndex(PadIndex) && Monoliths[PadIndex] &&
        Monoliths[PadIndex]->GetState() == EMonolithState::LockedOn)
    {
        return; // do nothing, don’t count as wrong, don’t change lights
    }

    if (!Sequence.IsValidIndex(Expect)) return;

    const int32 Correct = Sequence[Expect];
    if (PadIndex == Correct)
    {
        LockMonolith(PadIndex);
        SpawnCandyAt(PadIndex);
        Expect++;

        if (Expect >= Sequence.Num())
        {
            State = ESimonState::Solved;
            OnPuzzleSolved.Broadcast(PuzzleId);
        }
    }
    else
    {
        // Only flash red if it’s NOT already locked (extra safety)
        if (Monoliths.IsValidIndex(PadIndex) && Monoliths[PadIndex] &&
            Monoliths[PadIndex]->GetState() != EMonolithState::LockedOn)
        {
            Monoliths[PadIndex]->SetState(EMonolithState::ErrorRed);

            FTimerHandle Tmp;
            ASimonMonolith* M = Monoliths[PadIndex];
            GetWorldTimerManager().SetTimer(Tmp, [M]()
            {
                if (M && M->GetState() == EMonolithState::ErrorRed)
                    M->SetState(EMonolithState::Off);
            }, 0.5f, false);
        }

        SpawnEnemyAt(PadIndex);
        NextAllowedInputTime = Now + WrongInputLockout;
    }
}



void ASimonPuzzle::LockMonolith(int32 Index)
{
    if (Monoliths.IsValidIndex(Index) && Monoliths[Index])
        Monoliths[Index]->SetState(EMonolithState::LockedOn);

    // ✅ disable its pad so it can’t be pressed again
    if (StepPads.IsValidIndex(Index) && StepPads[Index])
        StepPads[Index]->SetPadEnabled(false);
}

// Small helper that uses the monolith's SpawnArrow as the basis.
static FTransform GetArrowGroundedXf(ASimonMonolith* Mono,
                                     float ForwardNudge = 20.f,
                                     float UpNudge = 6.f,
                                     float DownCast = 120.f,
                                     bool bDebug = false)
{
    check(Mono);

    // 1) Start from SpawnArrow (or actor) transform
    FTransform BaseXf = Mono->SpawnArrow
        ? Mono->SpawnArrow->GetComponentTransform()
        : FTransform(Mono->GetActorRotation(), Mono->GetActorLocation());

    // 2) Nudge a bit forward & up so we don't clip the monolith
    const FVector Fwd = BaseXf.GetRotation().GetForwardVector();
    FVector Start = BaseXf.GetLocation() + Fwd * ForwardNudge + FVector(0, 0, UpNudge);

    // 3) Grounding trace
    const FVector End = Start - FVector(0, 0, DownCast);
    FHitResult Hit;
    FCollisionQueryParams Params(TEXT("ArrowGroundTrace"), false, Mono);
    const bool bHit = Mono->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    FVector FinalLoc = bHit ? Hit.ImpactPoint + FVector(0, 0, UpNudge) : Start;

    if (bDebug)
    {
        DrawDebugLine(Mono->GetWorld(), Start, End, FColor::Yellow, false, 2.f, 0, 1.5f);
        DrawDebugSphere(Mono->GetWorld(), FinalLoc, 8.f, 12, bHit ? FColor::Green : FColor::Red, false, 2.f);
    }

    return FTransform(BaseXf.GetRotation(), FinalLoc);
}

void ASimonPuzzle::SpawnCandyAt(int32 Index)
{
    if (!Monoliths.IsValidIndex(Index) || !Monoliths[Index] || !CandyClass) return;

    FTransform SpawnXf = GetArrowGroundedXf(Monoliths[Index], /*ForwardNudge*/20.f, /*UpNudge*/6.f);

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    ACandyCollectible* Candy = GetWorld()->SpawnActorDeferred<ACandyCollectible>(
        CandyClass, SpawnXf, this, nullptr, Params.SpawnCollisionHandlingOverride);

    if (Candy)
    {
        // Optional tiny pop if your candy has ProjectileMovement
        // if (auto* Proj = Candy->FindComponentByClass<UProjectileMovementComponent>())
        //     Proj->Velocity = SpawnXf.GetRotation().GetUpVector() * 180.f;

        UGameplayStatics::FinishSpawningActor(Candy, SpawnXf);
    }
}

void ASimonPuzzle::SpawnEnemyAt(int32 Index)
{
    if (!Monoliths.IsValidIndex(Index) || !Monoliths[Index] || !EnemyClass) return;

    FTransform SpawnXf = GetArrowGroundedXf(Monoliths[Index], /*ForwardNudge*/20.f, /*UpNudge*/6.f);

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AActor* Enemy = GetWorld()->SpawnActor<AActor>(
        EnemyClass, SpawnXf.GetLocation(), SpawnXf.Rotator(), Params);

    if (!Enemy)
    {
        UE_LOG(LogTemp, Error, TEXT("[Simon] SpawnEnemyAt: failed for class %s"),
            *GetNameSafe(EnemyClass));
    }
}
