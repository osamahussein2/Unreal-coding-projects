// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PlayerCharacter.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	// Add all weapons into the weapons map
	TArray<AActor*> pistol, shotgun, rifle;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, AWeapon::StaticClass(), TEXT("Pistol"), pistol);
	UGameplayStatics::GetAllActorsOfClassWithTag(this, AWeapon::StaticClass(), TEXT("Shotgun"), shotgun);
	UGameplayStatics::GetAllActorsOfClassWithTag(this, AWeapon::StaticClass(), TEXT("Rifle"), rifle);

	// Add pistol, shotgun and rifle to the weapons map
	if (Cast<AWeapon>(pistol[0])) weapons.Add(Cast<AWeapon>(pistol[0]));
	if (Cast<AWeapon>(shotgun[0])) weapons.Add(Cast<AWeapon>(shotgun[0]));
	if (Cast<AWeapon>(rifle[0])) weapons.Add(Cast<AWeapon>(rifle[0]));

	if (pistol[0]) pistol[0] = nullptr;
	if (shotgun[0]) shotgun[0] = nullptr;
	if (rifle[0]) rifle[0] = nullptr;
}

// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Only update this component if the owner of this component is locking on to an enemy
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (lockedOnTarget && lockedOnEnemy && characterRef)
	{
		const FRotator& lookAtEnemy = UKismetMathLibrary::FindLookAtRotation(characterRef->GetActorLocation(),
			lockedOnEnemy->GetActorLocation());

		const FRotator& characterRotation = FRotator(lookAtEnemy.Pitch, lookAtEnemy.Yaw,
			characterRef->GetController()->GetControlRotation().Roll);

		// Set the character owning this component to look at the target they're locked on to
		characterRef->GetController()->SetControlRotation(characterRotation);

		// Stop locking on to an enemy after either the enemy is destroyed or the player isn't in the enemy's view anymore
		if (lockedOnEnemy->IsActorBeingDestroyed())
		{
			DetectNextEnemy();
		}

		if (GetWorld()->LineTraceTestByChannel(characterRef->GetActorLocation() + locationOffset,
			lockedOnEnemy->GetActorLocation(), ECC_Visibility, Params) ||
			FVector::Distance(characterRef->GetActorLocation(), lockedOnEnemy->GetActorLocation()) >= globalNearestTarget)
		{
			StopDetectingEnemies();
		}
	}

	else if (!lockedOnTarget && lockedOnEnemy && characterRef)
	{
		lockedOnEnemy = nullptr;
		characterRef = nullptr;
	}
}

AWeapon* UWeaponComponent::IsWeaponValid(const FName& tagName_) const
{
	for (int i = 0; i < weapons.Num(); i++)
	{
		if (weapons[i] && weapons[i]->ActorHasTag(tagName_))
		{
			return weapons[i];
		}
	}

	return nullptr;
}

bool UWeaponComponent::IsHoldingWeapon(FName tagName_) const
{
	for (int i = 0; i < weapons.Num(); i++)
	{
		if (weapons[i] && weapons[i]->ActorHasTag(tagName_))
		{
			return weapons[i] && weapons[i]->GetPlayerIsOnWeapon();
		}
	}

	return false;
}

void UWeaponComponent::PickUpWeapon(ACharacter* playerCharacter_, FName tagName_, FName socketName_)
{
	for (int i = 0; i < weapons.Num(); i++)
	{
		if (!weapons[i] && !playerCharacter_) return;

		if (weapons[i]->ActorHasTag(tagName_) && !weapons[i]->IsAttachedTo(playerCharacter_))
		{
			// Disable weapon collisions for that weapon
			//weapons[i]->DisableCollisions();

			weapons[i]->SetActorScale3D(FVector(100.0f, 100.0f, 100.0f));

			// Attach the weapon onto the player
			weapons[i]->AttachToActor(playerCharacter_, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socketName_);

			weapons[i]->SetActorRotation(FRotator(playerCharacter_->GetControlRotation().Pitch,
				weapons[i]->GetActorRotation().Yaw, 90.0f));

			weapons[i]->HideGlowingMesh(); // Hide glowing mesh effect when weapon picked up
		}
	}
}

void UWeaponComponent::AttachWeaponToSocket(ACharacter* playerCharacter_, FName tagName_, FName socketName_)
{
	for (int i = 0; i < weapons.Num(); i++)
	{
		if (!weapons[i] && !playerCharacter_) return;

		// Check if the player character mesh has passed in socket name
		if (weapons[i]->ActorHasTag(tagName_))
		{
			if (weapons[i]->IsAttachedTo(playerCharacter_) && playerCharacter_->GetMesh()->DoesSocketExist(socketName_))
			{
				// Set the weapon location to where the socket is found in the player character mesh
				weapons[i]->SetActorLocation(playerCharacter_->GetMesh()->GetSocketLocation(socketName_));

				//UE_LOG(LogTemp, Warning, TEXT("Weapon is at socket!"));
			}
		}
	}
}

void UWeaponComponent::RotateWeaponWhenHeld(ACharacter* playerCharacter_, FName tagName_, FName socketName_)
{
	if (playerCharacter_)
	{
		for (int i = 0; i < weapons.Num(); i++)
		{
			if (!weapons[i]) return;

			if (weapons[i]->ActorHasTag(tagName_) && weapons[i]->IsAttachedTo(playerCharacter_))
			{
				weapons[i]->SetActorRotation(FRotator(playerCharacter_->GetControlRotation().Pitch,
					weapons[i]->GetActorRotation().Yaw, 90.0f));
			}
		}
	}
}

void UWeaponComponent::RotateWeaponAtTarget(ACharacter* playerCharacter_, FName tagName_)
{
	if (playerCharacter_)
	{
		for (int i = 0; i < weapons.Num(); i++)
		{
			if (!weapons[i] || !lockedOnEnemy) return;

			if (weapons[i]->ActorHasTag(tagName_))
			{
				const FRotator& lookAtTarget = UKismetMathLibrary::FindLookAtRotation(weapons[i]->GetActorLocation(),
					lockedOnEnemy->GetActorLocation());

				weapons[i]->SetActorRelativeRotation(FRotator(0.0f, -2.0f, 90.0f));
			}
		}
	}
}

void UWeaponComponent::ResetWeaponRotation(ACharacter* playerCharacter_, const FName& tagName_)
{
	if (!playerCharacter_) return;

	for (int i = 0; i < weapons.Num(); i++)
	{
		if (!weapons[i]) return;

		// Set the weapon's rotation back to normal when locked on to a target
		if (weapons[i]->ActorHasTag(tagName_))
		{
			weapons[i]->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
		}
	}
}

void UWeaponComponent::DetectEnemies(UCameraComponent* cameraComp, const FName& tagName_)
{
	if (!characterRef) characterRef = Cast<ACharacter>(GetOwner());

	if (characterRef)
	{
		float nearestTarget = 0.0f;

		for (int i = 0; i < weapons.Num(); i++)
		{
			if (!weapons[i] && !weapons[i]->ActorHasTag(tagName_)) return;

			if (weapons[i]->ActorHasTag(tagName_)) nearestTarget = weapons[i]->GetWeaponRange();
			globalNearestTarget = nearestTarget;
		}

		FVector playerLocation = characterRef->GetActorLocation() + locationOffset;
		FVector playerForwardVector = cameraComp->GetForwardVector();

		cameraVector = playerForwardVector;

		if (lockedOnEnemy != nullptr) lockedOnEnemy = nullptr;

		for (TActorIterator<AEnemy> ActorItr(GetOwner()->GetWorld()); ActorItr; ++ActorItr)
		{
			AEnemy* foundEnemy = *ActorItr;

			FVector DirectionToActor = (foundEnemy->GetActorLocation() - playerLocation).GetSafeNormal();

			float distance = FVector::Dist(foundEnemy->GetActorLocation(), playerLocation);
			float DotProduct = FVector::DotProduct(playerForwardVector, DirectionToActor);

			if (!lockedOnEnemy && DotProduct >= detectionValue && distance < nearestTarget)
			{
				lockedOnEnemy = foundEnemy;
				nearestTarget = distance;

				// Proceed with spawning the enemy crosshair and character rotating to look at enemy if this returns true
				if (lockedOnEnemy && CanContinueUponCheckingLineTraceCollision(foundEnemy, playerLocation))
				{
					lockedOnEnemy->SpawnCrosshair();

					characterRef->GetCharacterMovement()->bUseControllerDesiredRotation = true;

					lockedOnTarget = true;

					//DrawDebugLine(GetWorld(), playerLocation, lockedOnEnemy->GetActorLocation(), FColor::Red, false, 1.0f, 0, 20.0f);
				}
			}

			foundEnemy = nullptr;
		}
	}
}

void UWeaponComponent::DetectEnemiesFromLeftSide(UCameraComponent* cameraComp, const FName& tagName_)
{
	if (lockedOnTarget && characterRef)
	{
		float nearestTarget = 0.0f;

		for (int i = 0; i < weapons.Num(); i++)
		{
			if (!weapons[i] && !weapons[i]->ActorHasTag(tagName_)) return;

			if (weapons[i]->ActorHasTag(tagName_)) nearestTarget = weapons[i]->GetWeaponRange();
			globalNearestTarget = nearestTarget;
		}

		FVector playerLocation = characterRef->GetActorLocation() + locationOffset;
		FVector playerRightVector = cameraComp->GetRightVector();

		for (TActorIterator<AEnemy> ActorItr(GetOwner()->GetWorld()); ActorItr; ++ActorItr)
		{
			AEnemy* foundEnemy = *ActorItr;

			if (!foundEnemy->GetLockedOn())
			{
				FVector DirectionToActor = (foundEnemy->GetActorLocation() - playerLocation).GetSafeNormal();

				float distance = FVector::Dist(foundEnemy->GetActorLocation(), playerLocation);
				float DotProduct = FVector::DotProduct(DirectionToActor, playerRightVector);

				if (DotProduct < 0.0f && distance < nearestTarget)
				{
					if (lockedOnEnemy)
					{
						lockedOnEnemy->DestroyCrosshair();
						lockedOnEnemy = nullptr;
					}

					lockedOnEnemy = foundEnemy;

					// Proceed with spawning the other enemy crosshair if this returns true
					if (CanContinueUponCheckingLineTraceCollision(foundEnemy, playerLocation))
					{
						lockedOnEnemy->SpawnCrosshair();

						//DrawDebugLine(GetWorld(), playerLocation, lockedOnEnemy->GetActorLocation(), FColor::Red, false, 1.0f, 0, 20.0f);
					}
				}
			}

			foundEnemy = nullptr;
		}
	}
}

void UWeaponComponent::DetectEnemiesFromRightSide(UCameraComponent* cameraComp, const FName& tagName_)
{
	if (lockedOnTarget && characterRef)
	{
		float nearestTarget = 0.0f;

		for (int i = 0; i < weapons.Num(); i++)
		{
			if (!weapons[i] && !weapons[i]->ActorHasTag(tagName_)) return;

			if (weapons[i]->ActorHasTag(tagName_)) nearestTarget = weapons[i]->GetWeaponRange();
			globalNearestTarget = nearestTarget;
		}

		FVector playerLocation = characterRef->GetActorLocation() + locationOffset;
		FVector playerRightVector = cameraComp->GetRightVector();

		for (TActorIterator<AEnemy> ActorItr(GetOwner()->GetWorld()); ActorItr; ++ActorItr)
		{
			AEnemy* foundEnemy = *ActorItr;

			if (!foundEnemy->GetLockedOn())
			{
				FVector DirectionToActor = (foundEnemy->GetActorLocation() - playerLocation).GetSafeNormal();

				float distance = FVector::Dist(foundEnemy->GetActorLocation(), playerLocation);
				float DotProduct = FVector::DotProduct(DirectionToActor, playerRightVector);

				if (DotProduct > 0.0f && distance < nearestTarget)
				{
					if (lockedOnEnemy)
					{
						lockedOnEnemy->DestroyCrosshair();
						lockedOnEnemy = nullptr;
					}

					lockedOnEnemy = foundEnemy;

					// Proceed with spawning the other enemy crosshair if this returns true
					if (CanContinueUponCheckingLineTraceCollision(foundEnemy, playerLocation))
					{
						lockedOnEnemy->SpawnCrosshair();

						//DrawDebugLine(GetWorld(), playerLocation, lockedOnEnemy->GetActorLocation(), FColor::Red, false, 1.0f, 0, 20.0f);
					}
				}
			}

			foundEnemy = nullptr;
		}
	}
}

void UWeaponComponent::StopDetectingEnemies()
{
	if (lockedOnEnemy) lockedOnEnemy->DestroyCrosshair();
	if (characterRef) characterRef->GetCharacterMovement()->bUseControllerDesiredRotation = false;

	lockedOnTarget = false;
}

void UWeaponComponent::DetectNextEnemy()
{
	TArray<AActor*> foundEnemies;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), foundEnemies);

	// If no enemies are found, set locked on target to false and disable character reference's controller desired rotation
	if (foundEnemies.IsEmpty())
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(GetOwner()))
		{
			player->OnCompleteQuest();
		}

		allEnemiesKilled = true;

		StopDetectingEnemies();
		return;
	}

	for (AActor* enemy : foundEnemies) if (enemy) enemy = nullptr; // Release all memory from the found enemy actors

	FVector playerLocation = characterRef->GetActorLocation() + locationOffset;
	FVector playerForwardVector = cameraVector;

	for (TActorIterator<AEnemy> ActorItr(GetOwner()->GetWorld()); ActorItr; ++ActorItr)
	{
		AEnemy* foundEnemy = *ActorItr;

		FVector DirectionToActor = (foundEnemy->GetActorLocation() - playerLocation).GetSafeNormal();

		float distance = FVector::Dist(foundEnemy->GetActorLocation(), playerLocation);
		float DotProduct = FVector::DotProduct(playerForwardVector, DirectionToActor);

		if (distance < globalNearestTarget)
		{
			if (lockedOnEnemy)
			{
				lockedOnEnemy->DestroyCrosshair();
				lockedOnEnemy = nullptr;
			}

			lockedOnEnemy = foundEnemy; // Set this to the next enemy actor

			// Proceed with spawning the other enemy crosshair if this returns true
			if (CanContinueUponCheckingLineTraceCollision(foundEnemy, playerLocation))
			{
				lockedOnEnemy->SpawnCrosshair();
			}
		}

		else
		{
			StopDetectingEnemies();
		}

		foundEnemy = nullptr;
	}
}

bool UWeaponComponent::CanContinueUponCheckingLineTraceCollision(AEnemy* enemy_, FVector location_)
{
	FHitResult Hit;
	Params.AddIgnoredActor(characterRef);
	Params.AddIgnoredActor(lockedOnEnemy);

	// If the player hit something other than the locked on enemy using a line trace, don't lock on and release all memory
	if (GetWorld()->LineTraceSingleByChannel(Hit, location_, lockedOnEnemy->GetActorLocation(), ECC_Visibility, Params))
	{
		enemy_ = nullptr;
		characterRef = nullptr;
		lockedOnEnemy = nullptr;

		return false;
	}

	return true;
}