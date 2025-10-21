// Fill out your copyright notice in the Description page of Project Settings.

#include "Combat/Enemies/BaseEnemyProjectile.h"
#include "ImpactSystem/SurfaceImpactSubsystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Enemies/BaseEnemy.h"

// Sets default values
ABaseEnemyProjectile::ABaseEnemyProjectile()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetInstigatorActor(GetOwner());

	baseEnemy = Cast<ABaseEnemy>(GetInstigator());

	// Create a timer handle that will destroy the projectile based on time in seconds
	FTimerHandle EnemyProjectileTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EnemyProjectileTimerHandle, this, &ABaseEnemyProjectile::DestroyProjectile,
		LifeTime, false);
}

void ABaseEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (baseEnemy)
	{
		TargetHomingProjectileOnPlayer();
	}

	// Just in case if the enemy projectile doesn't know which enemy owns this actor
	else
	{
		baseEnemy = Cast<ABaseEnemy>(GetInstigator());
	}
}

void ABaseEnemyProjectile::TargetHomingProjectileOnPlayer()
{
	if (baseEnemy->GetPlayerCharacter())
	{
		// Get a safe normalized direction vector
		FVector LaunchDirection = (baseEnemy->GetPlayerCharacter()->GetActorLocation() -
			GetActorLocation()).GetSafeNormal();

		// Launch the projectile upward and towards the direction vector
		FRotator LaunchRotation(LaunchDirection.Rotation().Pitch + baseEnemy->GetThrowAngle(),
			LaunchDirection.Rotation().Yaw, 0.0f);

		// Launch projectile using its homing projectile speed
		LaunchProjectile(LaunchRotation.Vector() * baseEnemy->GetHomingProjectileSpeed());
	}
}

void ABaseEnemyProjectile::DestroyProjectile()
{
	Destroy();

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	//UE_LOG(LogTemp, Warning, TEXT("It worked!"));
}