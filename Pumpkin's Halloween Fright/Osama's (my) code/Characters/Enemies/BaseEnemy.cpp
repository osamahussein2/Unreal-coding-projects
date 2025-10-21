// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/BaseEnemy.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "Camera/CameraComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBar->SetupAttachment(RootComponent);

	aiPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	
	// Create and configure sight sense
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	// Add sight configuration
	aiPerception->ConfigureSense(*SightConfig);
	aiPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	aiController = Cast<AAIController>(GetController());

	if (aiController)
	{
		// Set perception component from AI Controller class as the primary perception component
		aiController->SetPerceptionComponent(*aiPerception);
	}


	isAttacking = false;
	canMove = true;

	seenPlayer = false;

	alpha = 1.0f;

	patrolIndex = 0;

	waitTimer = 0.0f;

	enemySpawnedProjectile = false;

	attackTimer = 0.0f;

	isEnemyHit = false;

	//projectileSpawnTime = 0.0f; // Initialize before we get garbage data
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Just in case, if it isn't equal to 0 already
	//if (projectileSpawnTime != 0.0f) projectileSpawnTime = 0.0f;

	aiController = Cast<AAIController>(GetController());
	enemyWidget = Cast<UEnemyWidget>(HealthBar->GetWidget());

	if (isAttacking != false) isAttacking = false;
	if (seenPlayer != false) seenPlayer = false;
	if (canMove != true) canMove = true;

	if (alpha != 1.0f) alpha = 1.0f;
	if (patrolIndex != 0) patrolIndex = 0;

	if (waitTimer != 0.0f) waitTimer = 0.0f;

	if (enemySpawnedProjectile != false) enemySpawnedProjectile = false;

	if (attackTimer != 0.0f) attackTimer = 0.0f;

	if (isEnemyHit != false) isEnemyHit = false;

	// Bind target perception update/forgotten delegate to the base enemy target callback functions
	if (aiPerception)
	{
		aiPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseEnemy::OnTargetPerceptionUpdated);
		aiPerception->OnTargetPerceptionForgotten.AddDynamic(this, &ABaseEnemy::OnTargetPerceptionForgotten);
	}

	// Find the player character in the level
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHJPlayerCharacter::StaticClass(), FoundActors);

	for (AActor* player : FoundActors)
	{
		playerCharacter = Cast<AHJPlayerCharacter>(player);
	}

	for (int i = 0; i < materialSlotsSize; i++)
	{
		matInstanceDynamic[i] = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(i), this);
		GetMesh()->SetMaterial(i, matInstanceDynamic[i]); // Assign the dynamic material instance

		if (matInstanceDynamic[i])
		{
			matInstanceDynamic[i]->SetScalarParameterValue(TEXT("OpacityAmount"), alpha);
		}
	}

	// Initialize enemy's health bar if enemy widget is valid
	if (enemyWidget)
	{
		enemyWidget->SetEnemyHealthProgressBar(GetHealthComponent()->GetHealth(), GetHealthComponent()->GetMaxHealth());
	}

	// Initialize spawn rotation upon spawning or placed in world
	spawnRotation = GetActorRotation();
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//projectileSpawnTime += DeltaTime;

	//// If the projectile spawn time exceeds some max time set in the inspector, spawn the projectile
	//if (projectileSpawnTime >= maxTimeToSpawnProjectile)
	//{
	//	// Initialize the projectile's location and rotation from the enemy's location and rotation
	//	FVector Location(GetActorLocation());
	//	FRotator Rotation(GetActorRotation());

	//	FActorSpawnParameters SpawnInfo;
	//	SpawnInfo.Owner = this; // Make sure this enemy owns the projectile actor

	//	// Spawn the projectile
	//	AProjectile* projectileActor = GetWorld()->SpawnActor<AProjectile>(projectile, Location, Rotation, SpawnInfo);

	//	// Reset spawn projectile timer back to 0 to respawn another projectile
	//	projectileSpawnTime = 0.0f;
	//}
	
	// Test enemy death animation
	//healthComponent->Kill();

	if (!GetHealthComponent()->IsDead())
	{
		PerformLogicWhileAlive(DeltaTime);
	}

	else
	{
		// Increase the death timer once this enemy is considered "dead"
		GetHealthComponent()->deathTimer += DeltaTime;

		// Make enemy fade out after the death timer exceeds some time
		if (GetHealthComponent()->deathTimer >= 3.0f)
		{
			ExecuteDeathLogic(DeltaTime);
		}
	}

	if (HealthBar && playerCharacter->GetPlayerCameraComponent())
	{
		FRotator seePlayerCam = UKismetMathLibrary::FindLookAtRotation(HealthBar->GetComponentLocation(),
			playerCharacter->GetPlayerCameraComponent()->GetComponentLocation());

		HealthBar->SetWorldRotation(seePlayerCam);
	}

	// Test enemy hit reaction animation
	//TakeDamage(0.5f, FDamageEvent(), GetInstigatorController(), this);
}

AHJPlayerCharacter* ABaseEnemy::GetPlayerCharacter() const
{
	if (playerCharacter)
	{
		return playerCharacter;
	}

	else
	{
		return nullptr;
	}
}

float ABaseEnemy::GetThrowAngle() const
{
	return ThrowAngle;
}

float ABaseEnemy::GetHomingProjectileSpeed() const
{
	return homingProjectileSpeed;
}

void ABaseEnemy::OnTargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)
{
	// Once in the enemy's radius, set seen player to true so that enemy will chase the player
	if (TargetActor->IsA(playerCharacter->StaticClass()) && Stimulus.WasSuccessfullySensed() && !seenPlayer
		&& playerCharacter && !playerCharacter->GetIsHidden() && playerCharacter->IsAlive())
	{
		seenPlayer = true;
	}

	// Make enemy forgot player whenever stimulus was successful to prevent enemy from glitching out
	else if (TargetActor->IsA(playerCharacter->StaticClass()) && Stimulus.WasSuccessfullySensed() && 
		playerCharacter && playerCharacter->GetIsHidden())
	{
		aiPerception->ForgetActor(playerCharacter);
	}

	else if (TargetActor->IsA(playerCharacter->StaticClass()) && Stimulus.WasSuccessfullySensed() &&
		playerCharacter && playerCharacter->IsDead())
	{
		aiPerception->ForgetActor(playerCharacter);
	}
}

void ABaseEnemy::OnTargetPerceptionForgotten(AActor* TargetActor)
{
	// Once player is actually forgotten, set seen player to false
	if (TargetActor->IsA(playerCharacter->StaticClass()) && playerCharacter && playerCharacter->GetIsHidden())
	{
		seenPlayer = false;
	}

	else if (TargetActor->IsA(playerCharacter->StaticClass()) && playerCharacter && playerCharacter->IsDead())
	{
		seenPlayer = false;
	}
}

void ABaseEnemy::ABaseEnemy::PerformLogicWhileAlive(float& DeltaTime)
{
	if (seenPlayer)
	{
		// If the enemy can move but isn't attacking yet, move towards the player once this enemy sees them
		if (canMove && !isAttacking)
		{
			// Only launch an attack on the player if the player is alive
			if (playerCharacter && playerCharacter->IsAlive() && !isEnemyHit)
			{
				// Make sure the AI moves toward the player to attack them
				if (aiController) aiController->MoveToLocation(playerCharacter->GetActorLocation(), stopAtPlayerRadius);

				// If enemy is close enough to the player, don't move anymore
				if (FVector::Distance(playerCharacter->GetActorLocation(), 
					aiController->GetPawn()->GetActorLocation()) <= distanceUntilAttacking)
				{
					canMove = false;
				}

				// Otherwise if player is far enough from the enemy, make enemy patrol again
				else if (FVector::Distance(playerCharacter->GetActorLocation(),
					aiController->GetPawn()->GetActorLocation()) >= loseSightRadius)
				{
					if (patrolIndex != 0) patrolIndex = 0; // Reset to first patrol route
					seenPlayer = false;
				}
			}

			// If the player is dead, just forgot about the player altogether and go back to patrol state
			else if (playerCharacter && playerCharacter->IsDead())
			{
				aiPerception->ForgetActor(playerCharacter);
				if (patrolIndex != 0) patrolIndex = 0; // Reset to first patrol route if it hasn't already
			}
		}

		// Otherwise, enemy cannot move but isn't attacking yet, start attacking the player
		else if (!canMove && !isAttacking)
		{
			// Only if the player isn't hidden
			if (playerCharacter && !playerCharacter->GetIsHidden())
			{
				// Look at player using find look at rotation
				FRotator seePlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
					playerCharacter->GetActorLocation());

				// Set the AI to look at the player at all times
				SetActorRotation(seePlayer);
			}

			PlayAttackAnimation();

			isAttacking = true;

			if (aiController) { aiController->StopMovement(); }
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->DisableMovement();   
		}

		// Once player is hidden, make the enemy completely forgot about the player
		if (playerCharacter && playerCharacter->GetIsHidden() && !isAttacking)
		{
			aiPerception->ForgetActor(playerCharacter);
			if (patrolIndex != 0) patrolIndex = 0; // Reset to first patrol route if it hasn't already
		}
	}

	else if (!seenPlayer)
	{
		// Prevents Unreal from crashing once patrol routes array is clearly empty
		if (patrolRoutes.IsEmpty()) return;

		// If the AI gets near the patrol route
		if (FVector::Distance(GetActorLocation(), patrolRoutes[patrolIndex]) <= distanceToSwitchPatrolRoute)
		{
			waitTimer += DeltaTime;

			ChangeEnemyPatrolPoint();
		}

		// Otherwise, enemy isn't near enough to its patrol point index
		else
		{
			// Make sure the AI moves to some patrol route at its index
			if (aiController && !isEnemyHit) aiController->MoveToLocation(patrolRoutes[patrolIndex], stopAtPatrolRadius);

			// Check if the enemy has stopped moving to set their rotation back to wherever they spawned at
			if (UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) <= 0.01f && patrolRoutes.Num() <= 1 &&
				!patrolRoutes.IsEmpty() && GetActorRotation() != spawnRotation)
			{
				SetActorRotation(spawnRotation);
			}
		}
	}

	// Check if enemy is attacking so that attacking is set back to false once its attack animation has finished
	if (isAttacking)
	{
		canMove = false;
		if (USkeletalMeshComponent* MeshComp = GetMesh())
		{
			if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
			{
				if (!AnimInstance->Montage_IsPlaying(AttackMontage))
				{
					canMove = true;
					isAttacking = false;
					enemySpawnedProjectile = false;
					attackTimer = 0.0f;

					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // re-enable
				}

				else if (AnimInstance->Montage_IsPlaying(AttackMontage) && !enemySpawnedProjectile)
				{
					attackTimer += DeltaTime;

					if (attackTimer < timeUntilAttackAt)
					{
						if (playerCharacter)
						{
							// Look at player using find look at rotation
							FRotator seePlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
								playerCharacter->GetActorLocation());

							// Set the AI to look at the player at all times
							SetActorRotation(seePlayer);
						}
					}

					else
					{
						SpawnProjectile();
						enemySpawnedProjectile = true;
					}
				}
			}
		}
	}

	EnableMovement();
}

void ABaseEnemy::ChangeEnemyPatrolPoint()
{
	// Execute the next patrol point after its wait timer exceeds its max time
	if (patrolIndex < patrolRoutes.Num() - 1 && waitTimer >= maxWaitTime && patrolRoutes.Num() > 1)
	{
		++patrolIndex;
		waitTimer = 0.0f; // Reset wait timer to trigger waiting again when necessary
	}

	// Reset its patrol index back to 0 if the index reaches the final patrol point
	else if (patrolIndex >= patrolRoutes.Num() - 1 && waitTimer >= maxWaitTime && patrolRoutes.Num() > 1)
	{
		patrolIndex = 0;
		waitTimer = 0.0f; // Reset wait timer to trigger waiting again when necessary
	}
}

void ABaseEnemy::SpawnProjectile()
{
	FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	ABaseEnemyProjectile* Projectile = GetWorld()->SpawnActor<ABaseEnemyProjectile>(ProjectileClass, SpawnLocation,
		SpawnRotation, SpawnParams);
}

void ABaseEnemy::EnableMovement()
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			// Re-enable movement once the hit react animation stops playing
			if (HitReactionMontage && AnimInstance->Montage_GetIsStopped(HitReactionMontage) && isEnemyHit)
			{
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				isEnemyHit = false;
			}
		}
	}
}

void ABaseEnemy::ExecuteDeathLogic(float& DeltaTime)
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			for (int i = 0; i < materialSlotsSize; i++)
			{
				if (matInstanceDynamic[i])
				{
					// Make enemy fade out overtime
					alpha -= DeltaTime;

					matInstanceDynamic[i]->SetScalarParameterValue(TEXT("OpacityAmount"), alpha);

					// Destroy this enemy after it's becomes invisible during death
					if (alpha <= 0.0f) Destroy();
				}
			}
		}
	}
}

void ABaseEnemy::PlayAttackAnimation()
{
	if (!AttackMontage) return;

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->Montage_Play(AttackMontage);
		}
	}
}

void ABaseEnemy::PlayHitReactAnimation()
{
	if (!HitReactionMontage) return;

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->Montage_Play(HitReactionMontage);
		}
	}
}

void ABaseEnemy::OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const UDamageType* DamageType)
{
	Super::OnHealthChanged(HealthComp, Health, HealthDelta, DamageType);
	AHJCharacterBase::OnHealthChanged(HealthComp, Health, HealthDelta, DamageType);

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			// Loop through the hit reaction animation once it's actually taken damage
			if (HealthDelta < 0.0f)
			{
				// If they haven't seen the player already, make them see player now that they're hit
				if (!seenPlayer && playerCharacter && !playerCharacter->GetIsHidden() && playerCharacter->IsAlive())
				{
					seenPlayer = true;
				}

				// Update enemy's health bar
				if (enemyWidget)
				{
					enemyWidget->SetEnemyHealthProgressBar(GetHealthComponent()->GetHealth(), GetHealthComponent()->GetMaxHealth());
				}

				if (!AnimInstance->Montage_IsPlaying(HitReactionMontage))
				{
					PlayHitReactAnimation();

					// Disable enemy movement as well
					GetCharacterMovement()->DisableMovement();
					GetCharacterMovement()->StopMovementImmediately();

					isEnemyHit = true;
				}
			}
		}
	}
}

void ABaseEnemy::OnDeath(UHealthComponent* HealthComp, AActor* Killer, const UDamageType* DamageType)
{
	Super::OnDeath(HealthComp, Killer, DamageType);
	AHJCharacterBase::OnDeath(HealthComp, Killer, DamageType);
}