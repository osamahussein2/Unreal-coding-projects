#include "LootSplitter.h"
#include "CandyCollectible.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ALootSplitter::ALootSplitter()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);
}

void ALootSplitter::BeginPlay()
{
    Super::BeginPlay();

    ResolvedMuzzles.Reset();
    for (const FComponentReference& Ref : Muzzles)
    {
        if (UActorComponent* C = Ref.GetComponent(this))
        {
            if (USceneComponent* SC = Cast<USceneComponent>(C))
            {
                ResolvedMuzzles.Add(SC);
            }
        }
    }
}

bool ALootSplitter::ComputeBallisticVelocity(const FVector& Start, const FVector& Target, float Time, FVector& OutVelocity) const
{
    if (Time <= KINDA_SMALL_NUMBER) return false;

    const FVector Delta = Target - Start;                // displacement
    const FVector Gravity = FVector(0, 0, GetWorld()->GetGravityZ()); // negative Z
    // v = (Δx - 0.5 g t^2) / t
    OutVelocity = (Delta - 0.5f * Gravity * Time * Time) / Time;
    return true;
}

void ALootSplitter::SpawnOne(USceneComponent* Muzzle)
{
    if (!CandyClass || !Muzzle) return;

    const FVector Forward   = Muzzle->GetForwardVector();
    const FVector MuzzleLoc = Muzzle->GetComponentLocation();

    // 🔑 Start a little in front & above the muzzle so we don't immediately hit foliage
    const float StartForward = 25.f;   // 25 cm forward
    const float StartUp      = 10.f;   // 10 cm up
    const FVector SpawnLoc   = MuzzleLoc + Forward * StartForward + FVector(0,0,StartUp);

    // Build landing target (same as before)
    const FVector Right   = FVector::CrossProduct(Forward, FVector::UpVector).GetSafeNormal();
    const float   Lateral = FMath::FRandRange(-LateralSpread, LateralSpread);
    FVector Target        = MuzzleLoc + Forward * DesiredRange + Right * Lateral;
    Target.Z += LandingZBias;

    FVector Velocity;
    if (!ComputeBallisticVelocity(SpawnLoc, Target, FlightTime, Velocity))
        return;

    // Use "AlwaysSpawn" so offset isn't adjusted back into the bush
    FTransform T(Muzzle->GetComponentRotation(), SpawnLoc);
    ACandyCollectible* Candy = GetWorld()->SpawnActorDeferred<ACandyCollectible>(
        CandyClass, T, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    if (Candy)
    {
        UGameplayStatics::FinishSpawningActor(Candy, T);

        // 🔑 Make the candy ignore the spitter (bush actor) to prevent instant hits
        Candy->IgnoreActorWhenMoving(this, /*bShouldIgnore=*/true);

        // Kick it
        Candy->LaunchWithVelocity(Velocity);

#if !UE_BUILD_SHIPPING
        UE_LOG(LogTemp, Log, TEXT("Spit: V=(%.1f, %.1f, %.1f)  From=%s  To=%s"),
            Velocity.X, Velocity.Y, Velocity.Z,
            *SpawnLoc.ToString(), *Target.ToString());
        DrawDebugLine(GetWorld(), SpawnLoc, SpawnLoc + Velocity * 0.05f, FColor::Cyan, false, 2.f, 0, 2.f);
#endif
    }
}

void ALootSplitter::KickStagger()
{
    if (PendingMuzzleIndex >= ResolvedMuzzles.Num()) return;
    if (PendingPerMuzzle >= CandiesPerMuzzle)
    {
        PendingMuzzleIndex++;
        PendingPerMuzzle = 0;
    }

    if (PendingMuzzleIndex < ResolvedMuzzles.Num())
    {
        SpawnOne(ResolvedMuzzles[PendingMuzzleIndex].Get());
        PendingPerMuzzle++;

        const float Delay = FMath::FRandRange(SpawnDelayRange.X, SpawnDelayRange.Y);
        GetWorld()->GetTimerManager().SetTimer(
            StaggerTimerHandle, this, &ALootSplitter::KickStagger, FMath::Max(0.f, Delay), false);
    }
}

void ALootSplitter::Spit()
{
    if (!CandyClass || Muzzles.Num() == 0) return;

    if (!bAllowRepeatSpit && bHasSpit)
    {
        UE_LOG(LogTemp, Warning, TEXT("[LootSplitter %s] Ignored Spit (already fired)"),
               *GetName());
        return;
    }
    bHasSpit = true;

    UE_LOG(LogTemp, Warning, TEXT("[LootSplitter %s] Spit() called. Muzzles=%d, CandiesPerMuzzle=%d"),
           *GetName(), Muzzles.Num(), CandiesPerMuzzle);

    // Start staggered spawning
    PendingMuzzleIndex = 0;
    PendingPerMuzzle = 0;
    KickStagger();
}
