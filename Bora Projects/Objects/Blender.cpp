// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/Blender.h"
#include "Characters/Enemies/Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABlender::ABlender()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	skeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	skeletalMeshComponent->SetupAttachment(RootComponent);

	blenderSound = CreateDefaultSubobject<UAudioComponent>(TEXT("BlenderSound"));
	blenderSound->SetupAttachment(skeletalMeshComponent);

	blenderSound->bOverrideAttenuation = true;
	blenderSound->AttenuationOverrides.bSpatialize = true;
	blenderSound->AttenuationOverrides.SpatializationAlgorithm = ESoundSpatializationAlgorithm::SPATIALIZATION_Default;
	blenderSound->AttenuationOverrides.AttenuationShape = EAttenuationShape::Sphere;
	blenderSound->AttenuationOverrides.FalloffDistance = 100000.0f;

	// Set animation mode to use animation asset
	skeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	hasBlenderAnimationChanged = false;
}

// Called when the game starts or when spawned
void ABlender::BeginPlay()
{
	Super::BeginPlay();

	if (hasBlenderAnimationChanged != false) hasBlenderAnimationChanged = false;

	if (skeletalMeshComponent)
	{
		// Assert that blender animations and meshes are set in the editor
		check(!blenderAnimations.IsEmpty() && !blenderMeshes.IsEmpty());
		check(blenderAnimations.Num() == blenderMeshes.Num());

		skeletalMeshComponent->SetSkeletalMeshAsset(blenderMeshes[0]);
		SetActorLocation(FVector(101490.0f, 101730.0f, 538.0f)); // Hardcoded location for now

		// Set animation to the blender neutral animation
		skeletalMeshComponent->SetAnimation(blenderAnimations[0]);
		skeletalMeshComponent->Play(true);
	}
}

void ABlender::PlayBlenderAlarmedAnimation()
{
	if (!blenderAnimations[1] || !blenderMeshes[1] || hasBlenderAnimationChanged || !skeletalMeshComponent ||
		!blenderSound) return;

	blenderSound->SetSound(LoadObject<USoundBase>(this, TEXT("/Game/Sounds/blender_scream")));
	blenderSound->Play();

	skeletalMeshComponent->SetSkeletalMeshAsset(blenderMeshes[1]);
	SetActorLocation(FVector(101490.0f, 101730.0f, 538.0f)); // Hardcoded location for now

	// Set animation to the blender attacked animation
	skeletalMeshComponent->SetAnimation(blenderAnimations[1]);
	skeletalMeshComponent->Play(true);

	FTimerHandle enemyDetectionTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(enemyDetectionTimerHandle, this, &ABlender::MakeEnemiesDetectPlayer,
		enemyDetectPlayerDelay, false);

	FTimerHandle audioTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(audioTimerHandle, this, &ABlender::PlayBlenderSoundAgain, playSoundDelay, true);

	hasBlenderAnimationChanged = true;
}

void ABlender::PlayBlenderSoundAgain()
{
	if (blenderSound && blenderSound->GetSound()) blenderSound->Play();
}

void ABlender::MakeEnemiesDetectPlayer()
{
	// Find multiple enemies in the level
	TArray<AActor*> foundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), foundEnemies);

	for (AActor* enemyActor : foundEnemies)
	{
		// Add found enemy actors into the enemies array only if the cast to enemy class is valid
		if (Cast<AEnemy>(enemyActor))
		{
			enemies.Add(Cast<AEnemy>(enemyActor));
		}

		enemyActor = nullptr; // Release actor memory either way
	}

	for (AEnemy* enemyObj : enemies)
	{
		// Make enemy detect the player if it hasn't already
		if (enemyObj && enemyObj->GetEnemyState() != EnemyState::EDetectedPlayer)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%d"), sizeof(enemyObj));
			
			enemyObj->SetEnemyState(EnemyState::EDetectedPlayer);
		}

		// Release enemy actor memory afterwards
		enemyObj = nullptr;
	}

	// Clear all enemies from the array
	if (!enemies.IsEmpty()) enemies.Empty();
}
