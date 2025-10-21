#include "RandomRewardPad.h"
#include "RewardPadGroup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "LootSplitter.h"
#include "CandyCollectible.h"
#include "Characters/Enemies/BaseEnemy.h"

ARandomRewardPad::ARandomRewardPad()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    PadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadMesh"));
    PadMesh->SetupAttachment(RootComponent);
    PadMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    PadMesh->SetCollisionResponseToAllChannels(ECR_Block);

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(RootComponent);
    Trigger->InitBoxExtent(FVector(50,50,20));
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Trigger->SetGenerateOverlapEvents(true);

    SpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("EnemySpawn"));
    SpawnArrow->SetupAttachment(RootComponent);
    SpawnArrow->SetRelativeLocation(FVector(60,0,0)); // a bit in front

    patrolRoute = FVector::Zero();
}

void ARandomRewardPad::BeginPlay()
{
    Super::BeginPlay();

    if (PadMesh)
    {
        MID = PadMesh->CreateDynamicMaterialInstance(MaterialElementIndex);
        if (MID && EmissiveStrengthParam != NAME_None)
        {
            MID->SetScalarParameterValue(EmissiveStrengthParam, EmissiveOff);
        }
    }

    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ARandomRewardPad::HandleBeginOverlap);
    Trigger->OnComponentEndOverlap.AddDynamic(this, &ARandomRewardPad::HandleEndOverlap);
}

void ARandomRewardPad::HandleBeginOverlap(UPrimitiveComponent* Overlapped, AActor* Other,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
    if (!Other || Other == this) return;
    if (!Cast<ACharacter>(Other)) return;           // only players (or use an interface/team check)

    if (Group) { Group->NotifyPadStepped(this); }

    const double Now = GetWorld()->GetTimeSeconds();
    if (bLocked) return;
    if (!bOneShot && Now < NextAllowedTime) return;

    const bool bGreen = (FMath::FRand() <= GreenChance);

    if (bGreen) DoGreen();
    else        DoRed();

    if (bOneShot)
    {
        bLocked = true;
    }
    else
    {
        NextAllowedTime = Now + CooldownSeconds;
    }
}

void ARandomRewardPad::SetPadColor(const FLinearColor& Color)
{
    if (!MID && PadMesh)
    {
        MID = PadMesh->CreateDynamicMaterialInstance(0);
    }

    if (MID)
    {
        MID->SetVectorParameterValue(TEXT("PadColor"), Color);
    }
}

void ARandomRewardPad::DoGreen()
{
    UE_LOG(LogTemp, Log, TEXT("[%s] DoGreen called"), *GetName());

    // Visual feedback — your emissive/indicator logic here
    SetPadColor(FLinearColor::Green);
    

    // --- Option B: Direct spawn fallback ---
    if (CandyClass)
    {
        const FTransform SpawnTransform =
            SpawnArrow ? SpawnArrow->GetComponentTransform() : GetActorTransform();

        ACandyCollectible* Candy = GetWorld()->SpawnActorDeferred<ACandyCollectible>(
            CandyClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

        if (Candy)
        {
            UGameplayStatics::FinishSpawningActor(Candy, SpawnTransform);

            // Inform group to track this candy
            if (Group)
            {
                Group->RegisterCandy(Candy);
            }

            UE_LOG(LogTemp, Log, TEXT("[%s] Spawned Candy %s"), *GetName(), *Candy->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] CandyClass not set"), *GetName());
    }
}

void ARandomRewardPad::DoRed()
{
    UE_LOG(LogTemp, Log, TEXT("[%s] DoRed called"), *GetName());

    // Visual feedback — your emissive/indicator logic here
    SetPadColor(FLinearColor::Red);

    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] EnemyClass not set"), *GetName());
        return;
    }

    const FTransform SpawnTransform =
        SpawnArrow ? SpawnArrow->GetComponentTransform() : GetActorTransform();

    AActor* Enemy = GetWorld()->SpawnActorDeferred<AActor>(
        EnemyClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

    ABaseEnemy* spawnedEnemy = Cast<ABaseEnemy>(Enemy);

    if (Enemy)
    {
        UGameplayStatics::FinishSpawningActor(Enemy, SpawnTransform);

        if (spawnedEnemy)
        {
            patrolRoute = SpawnArrow->GetComponentLocation();
            
            spawnedEnemy->patrolRoutes.Add(patrolRoute);
            
            for (int i = 0; i < spawnedEnemy->patrolRoutes.Num(); i++)
            {
                spawnedEnemy->patrolRoutes[i] = patrolRoute;
            }
        }

        // Inform group to track this enemy
        if (Group)
        {
            Group->RegisterEnemy(Enemy);
        }

        UE_LOG(LogTemp, Log, TEXT("[%s] Spawned Enemy %s"), *GetName(), *Enemy->GetName());
    }
}

void ARandomRewardPad::SetPadVisual(const FLinearColor& Color, float Strength)
{
    if (!MID) return;

    if (EmissiveStrengthParam != NAME_None)
    {
        MID->SetScalarParameterValue(EmissiveStrengthParam, Strength);
    }
    if (EmissiveColorParam != NAME_None)
    {
        MID->SetVectorParameterValue(EmissiveColorParam, Color);
    }
}

void ARandomRewardPad::ResetPadVisual()
{
    if (!MID) return;
    if (EmissiveStrengthParam != NAME_None)
    {
        MID->SetScalarParameterValue(EmissiveStrengthParam, EmissiveOff);
    }
    if (EmissiveColorParam != NAME_None)
    {
        // optional: set back to neutral; leave as last color if you prefer
        MID->SetVectorParameterValue(EmissiveColorParam, FLinearColor::Black);
    }
}
