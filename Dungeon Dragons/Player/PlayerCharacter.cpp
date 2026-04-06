// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "InputMappingContext.h"
#include "Player/PlayerInputDA.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "Actor/Floor.h"
#include "Components/ActionComponent.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/DungeonCrawlerGM.h"
#include "SceneCapture/MapSceneCapture2D.h"
#include "Actor/Door.h"
#include "Actor/Chest.h"
#include "DungeonCrawler2026/DungeonCrawler2026.h"
#include "Engine/DamageEvents.h"

// Enemy include files
#include "Enemy/BloodDrinker.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetupAttachment(GetMesh());

	cameraComponent->PostProcessSettings.AutoExposureBias = -1.0f;

	actionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("ActionComponent"));
	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	minimapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArm"));
	minimapSpringArm->SetupAttachment(GetMesh());

	minimapCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapSceneCapture"));
	minimapCaptureComponent->SetupAttachment(minimapSpringArm);

	// Disable atmosphere in scene capture
	minimapCaptureComponent->ShowFlags.Atmosphere = 0;

	spriteMinimapComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
	spriteMinimapComponent->SetupAttachment(minimapSpringArm);

	spriteMinimapComponent->bVisibleInSceneCaptureOnly = true;

	spriteMinimapComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteMinimapComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	GetCapsuleComponent()->SetCollisionObjectType(COLLISION_PLAYER);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_DOOR, ECR_Ignore);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (healthComponent)
	{
		healthComponent->OnHealthChanged.AddUniqueDynamic(this, &APlayerCharacter::UpdateHealthHUD);
		healthComponent->OnDeath.AddUniqueDynamic(this, &APlayerCharacter::DisablePlayerInput);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!inputMappingContext) return;

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
		(Cast<APlayerController>(GetController())->GetLocalPlayer());

	// Add the input mapping context
	enhancedInputSubsystem->AddMappingContext(inputMappingContext, 0);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (playerInputDA && playerInputDA->GetIA_Move())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_Move(), ETriggerEvent::Started, this,
				&APlayerCharacter::MoveCharacter);
		}

		if (playerInputDA && playerInputDA->GetIA_Camera())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_Camera(), ETriggerEvent::Started, this,
				&APlayerCharacter::MoveCamera);
		}

		if (playerInputDA && playerInputDA->GetIA_PauseGame())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_PauseGame(), ETriggerEvent::Started, this,
				&APlayerCharacter::PauseGame);
		}

		if (playerInputDA && playerInputDA->GetIA_OpenMap())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_OpenMap(), ETriggerEvent::Started, this,
				&APlayerCharacter::OpenMap);
		}

		if (playerInputDA && playerInputDA->GetIA_RollDice())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_RollDice(), ETriggerEvent::Started, this,
				&APlayerCharacter::RollDie);
		}

		if (playerInputDA && playerInputDA->GetIA_Interact())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_Interact(), ETriggerEvent::Started, this,
				&APlayerCharacter::Interact);
		}

		if (playerInputDA && playerInputDA->GetIA_Attack())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_Attack(), ETriggerEvent::Started, this,
				&APlayerCharacter::AttackEnemy);
		}

		if (playerInputDA && playerInputDA->GetIA_OpenInventory())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_OpenInventory(), ETriggerEvent::Started, this,
				&APlayerCharacter::OpenInventory);
		}
	}

}

void APlayerCharacter::IncrementPlayerDexterity(const int& value_)
{
	playerStats.dexterity = FMath::Clamp(playerStats.dexterity + value_, 0, 100);
	UpdateStatsHUD();
}

void APlayerCharacter::IncrementPlayerStrength(const int& value_)
{
	playerStats.strength = FMath::Clamp(playerStats.strength + value_, 0, 100);
	UpdateStatsHUD();
}

void APlayerCharacter::IncrementPlayerIntelligence(const int& value_)
{
	playerStats.intelligence = FMath::Clamp(playerStats.intelligence + value_, 0, 100);
	UpdateStatsHUD();
}

void APlayerCharacter::IncrementCollectedSmallHealthPotion()
{
	++collectedSmallHealthPotions;
}

void APlayerCharacter::IncrementCollectedMediumHealthPotion()
{
	++collectedMediumHealthPotions;
}

void APlayerCharacter::IncrementCollectedLargeHealthPotion()
{
	++collectedLargeHealthPotions;
}

void APlayerCharacter::HealWithSmallHealthPotion()
{
	if (!healthComponent) return;

	healthComponent->Heal(25.0f);
	if (healSound) UGameplayStatics::PlaySound2D(GetWorld(), healSound);

	--collectedSmallHealthPotions;
}

void APlayerCharacter::HealWithMediumHealthPotion()
{
	if (!healthComponent) return;

	healthComponent->Heal(50.0f);
	if (healSound) UGameplayStatics::PlaySound2D(GetWorld(), healSound);

	--collectedMediumHealthPotions;
}

void APlayerCharacter::HealWithLargeHealthPotion()
{
	if (!healthComponent) return;

	healthComponent->Heal(75.0f);
	if (healSound) UGameplayStatics::PlaySound2D(GetWorld(), healSound);

	--collectedLargeHealthPotions;
}

void APlayerCharacter::CloseInventory()
{
	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->DisableMouseCursor();

		gameMode->PopWidget(gameMode->GetInventoryClass());
		gameMode->PushWidget(gameMode->GetPlayerHUDClass());

		UpdatePlayerHUD();

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void APlayerCharacter::MoveCharacter(const FInputActionValue& value_)
{
	// Moving forward
	if (value_.Get<FVector2D>().Y > 0.1f)
	{
		MoveToClosestAheadOfPlayer();
	}

	// Moving back
	else if (value_.Get<FVector2D>().Y < -0.1f)
	{
		MoveToClosestFloorBehindThePlayer();
	}

	// Moving right
	else if (value_.Get<FVector2D>().X > 0.1f)
	{
		MoveToClosestFloorToRight();
	}

	// Moving left
	else if (value_.Get<FVector2D>().X < -0.1f)
	{
		MoveToClosestFloorToLeft();
	}
}

void APlayerCharacter::MoveCamera(const FInputActionValue& value_)
{
	// If the player wants to turn the camera right, increase the camera yaw value by 90 degrees
	if (value_.Get<FVector2D>().X > 0.1f)
	{
		cameraYawValue += 90.0f;

		if (Controller) Controller->SetControlRotation(FRotator(0.0f, cameraYawValue, 0.0f));
	}

	// If the player wants to turn the camera left, decrease the camera yaw value by 90 degrees
	else if (value_.Get<FVector2D>().X < -0.1f)
	{
		cameraYawValue -= 90.0f;

		if (Controller) Controller->SetControlRotation(FRotator(0.0f, cameraYawValue, 0.0f));
	}
}

void APlayerCharacter::PauseGame()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->PopWidget(gameMode->GetPlayerHUDClass());
			gameMode->PushWidget(gameMode->GetPauseMenuClass());

			gameMode->EnableMouseCursor();
		}
	}
}

void APlayerCharacter::OpenMap()
{
	if (!fullMapSceneCapture && fullMapSceneCaptureClass && !UGameplayStatics::IsGamePaused(GetWorld()))
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		fullMapSceneCapture = GetWorld()->SpawnActor<AMapSceneCapture2D>(fullMapSceneCaptureClass, GetActorLocation() + 
			mapCameraSpawnOffset, FRotator(-90.0f, 0.0f, 0.0f), spawnParams);

		if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->EnableMouseCursor();

			gameMode->PopWidget(gameMode->GetPlayerHUDClass());
			gameMode->PushWidget(gameMode->GetMapClass());
		}
	}

	else if (fullMapSceneCapture && UGameplayStatics::IsGamePaused(GetWorld()))
	{
		fullMapSceneCapture->Destroy();
		fullMapSceneCapture = nullptr;

		if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->DisableMouseCursor();

			gameMode->PopWidget(gameMode->GetMapClass());
			gameMode->PushWidget(gameMode->GetPlayerHUDClass());

			UpdatePlayerHUD();
		}
	}
}

void APlayerCharacter::OpenInventory()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->EnableMouseCursor();

			gameMode->PopWidget(gameMode->GetPlayerHUDClass());
			gameMode->PushWidget(gameMode->GetInventoryClass());

			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}

	else
	{
		CloseInventory();
	}
}

void APlayerCharacter::RollDie()
{
	if (!actionComponent || actionComponent->HasBlockedTag(rollingDiceAction)) return;

	if (!GetWorld()->GetTimerManager().IsTimerActive(rollDiceHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(rollDiceHandle, this, &APlayerCharacter::UpdateDiceRoll,
			GetWorld()->GetDeltaSeconds(), true);
	}

	else
	{
		GetWorld()->GetTimerManager().ClearTimer(rollDiceHandle);

		GetWorld()->GetTimerManager().SetTimer(rollDiceHandle, this, &APlayerCharacter::UpdateDiceRoll,
			GetWorld()->GetDeltaSeconds(), true);
	}
}

void APlayerCharacter::Interact()
{
	AChest* nearestChest = nullptr;

	for (TActorIterator<AChest> chestIt(GetWorld()); chestIt; ++chestIt)
	{
		const FVector& directionToChest = (chestIt->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		const float& dotProduct = FVector::DotProduct(GetActorForwardVector(), directionToChest);

		// Check if the chest is nearby, the player is looking directly at it to validate the nearest chest actor
		if (IsChestNearby(*chestIt) && dotProduct > dotProductValue)
		{
			nearestChest = *chestIt;
			break;
		}

		else continue;
	}

	if (!actionComponent) return;

	// If the nearest chest is found and if it implements the interactable interface, make the player interact with it
	if (nearestChest && nearestChest->Implements<UInteractable>() && !actionComponent->HasBlockedTag(collectFromChestAction))
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Chest implements the interactable interface"));
#endif

		nearestChest->Interact();

		actionComponent->GameplayBlockedTagsAdded.Broadcast({ collectFromChestAction });

		FTimerHandle handle{};
		GetWorld()->GetTimerManager().SetTimer(handle, this, &APlayerCharacter::EnableInteractionWithChest,
			chestInteractionCooldownTime, false);
	}
}

void APlayerCharacter::AttackEnemy()
{
	DealDamageToBloodDrinker();
}

void APlayerCharacter::EnableDieRoll()
{
	if (actionComponent) actionComponent->GameplayBlockedTagsRemoved.Broadcast({ rollingDiceAction });
}

void APlayerCharacter::CloseMap()
{
	if (fullMapSceneCapture)
	{
		fullMapSceneCapture->Destroy();
		fullMapSceneCapture = nullptr;

		if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->DisableMouseCursor();

			gameMode->PopWidget(gameMode->GetMapClass());
			gameMode->PushWidget(gameMode->GetPlayerHUDClass());

			UpdatePlayerHUD();
		}
	}
}

void APlayerCharacter::UpdatePlayerHUD()
{
	if (healthComponent)
	{
		if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			if (gameMode->DoesWidgetContainPlayerHUD())
			{
				gameMode->UpdatePlayerHealth(healthComponent->GetHealthPercentage());
				gameMode->UpdatePlayerStats(playerStats.dexterity, playerStats.strength, playerStats.intelligence);
			}
		}
	}
}

void APlayerCharacter::UpdateStatsHUD()
{
	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (gameMode->DoesWidgetContainPlayerHUD())
		{
			gameMode->UpdatePlayerStats(playerStats.dexterity, playerStats.strength, playerStats.intelligence);
		}
	}
}

void APlayerCharacter::ResetPlayerValues()
{
	// Reset health values
	if (healthComponent) healthComponent->ResetHealth();

	// Stop all dice rolling timer handles
	if (GetWorld()->GetTimerManager().IsTimerActive(rollDiceHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(rollDiceHandle);
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(landOnDie))
	{
		GetWorld()->GetTimerManager().ClearTimer(landOnDie);
	}

	// Reset die roll back to 1
	if (dieRoll != 1) dieRoll = 1;

	// Reset player stats back to starting values
	if (playerStats.dexterity != 1) playerStats.dexterity = 1;
	if (playerStats.strength != 1) playerStats.strength = 1;
	if (playerStats.intelligence != 1) playerStats.intelligence = 1;

	if (doorsUnlocked != 0) doorsUnlocked = 0;

	if (collectedSmallHealthPotions != 0) collectedSmallHealthPotions = 0;
	if (collectedMediumHealthPotions != 0) collectedMediumHealthPotions = 0;
	if (collectedLargeHealthPotions != 0) collectedLargeHealthPotions = 0;
}

bool APlayerCharacter::IsPlayerDead() const
{
	if (!healthComponent) return false;

	return healthComponent->IsDead();
}

void APlayerCharacter::MoveToClosestAheadOfPlayer()
{
	// Check if there is a door nearby to set nearest door so that player cannot move past it
	ADoor* nearestDoor = nullptr;
	AActor* nearestEnemy = nullptr;

	FHitResult hitResult;
	FCollisionQueryParams queryParams;

	queryParams.AddIgnoredActor(this);

	for (TActorIterator<AFloor> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() && It->GetLevel()->bIsVisible && actionComponent)
		{
			const FVector& directionToFloor = (It->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			const float& dotProduct = FVector::DotProduct(GetActorForwardVector(), directionToFloor);

			for (TActorIterator<ADoor> doorIt(GetWorld()); doorIt; ++doorIt)
			{
				if (doorsUnlocked < (*It)->GetFloorNumberForDoorsUnlocked() && IsDoorNearby(*doorIt) &&
					GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorForwardVector() * 10000.0f,
						ECC_Visibility, queryParams) && hitResult.GetActor() && hitResult.GetActor() == *doorIt)
				{
					nearestDoor = *doorIt;
					break;
				}

				else continue;
			}

			for (TActorIterator<AActor> enemyIt(GetWorld()); enemyIt; ++enemyIt)
			{
				if (enemyIt->GetLevel() && enemyIt->GetLevel()->bIsVisible && enemyIt->ActorHasTag(enemyTag))
				{
					const FVector& directionToEnemy = (enemyIt->GetActorLocation() - GetActorLocation()).GetSafeNormal();
					const float& dotProductToEnemy = FVector::DotProduct(GetActorForwardVector(), directionToEnemy);

					if (IsEnemyNearby(*It, *enemyIt) && IsEnemyNearby(*enemyIt) && dotProductToEnemy > dotProductValue)
					{
						nearestEnemy = *enemyIt;
						break;
					}
				}

				else continue;
			}

			if (dotProduct > dotProductValue && WithinTheRangeOfNextFloor(*It) &&
				!actionComponent->HasBlockedTag(movingAction) && !nearestDoor && !nearestEnemy)
			{
				const FVector& nextFloorLocation = FVector(It->GetActorLocation().X, It->GetActorLocation().Y,
					GetActorLocation().Z);

				SetActorLocation(nextFloorLocation);
				PlayFootstepSounds();

				actionComponent->GameplayBlockedTagsAdded.Broadcast({ movingAction });

				FTimerHandle movementTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(movementTimerHandle, this, &APlayerCharacter::EnableMovement,
					movementCooldownTime, false);

				break;
			}

			else if (nearestDoor || nearestEnemy) break;

			else continue;
		}

		else continue;
	}

	if (nearestDoor)
	{
		// If the door requires dexterity skill to unlock
		if (nearestDoor->GetDoorUnlocked() == EDoorUnlocked::EDexterity &&
			playerStats.dexterity >= nearestDoor->GetUnlockedAfterReachingSkill())
		{
			nearestDoor->StartOpeningDoor();

			if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				if (gameMode->DoesWidgetContainPlayerHUD())
				{
					gameMode->SpawnUnlockDoorWidgetOnHUD();
				}
			}
		}

		// If the door requires strength skill to unlock
		else if (nearestDoor->GetDoorUnlocked() == EDoorUnlocked::EStrength &&
			playerStats.strength >= nearestDoor->GetUnlockedAfterReachingSkill())
		{
			nearestDoor->StartOpeningDoor();

			if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				if (gameMode->DoesWidgetContainPlayerHUD())
				{
					gameMode->SpawnUnlockDoorWidgetOnHUD();
				}
			}
		}

		// If the door requires a intelligence skill to unlock
		else if (nearestDoor->GetDoorUnlocked() == EDoorUnlocked::EIntelligence &&
			playerStats.intelligence >= nearestDoor->GetUnlockedAfterReachingSkill())
		{
			nearestDoor->StartOpeningDoor();

			if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				if (gameMode->DoesWidgetContainPlayerHUD())
				{
					gameMode->SpawnUnlockDoorWidgetOnHUD();
				}
			}
		}

		// If the door requires dexterity, strength intelligence skills to unlock
		else if (nearestDoor->GetDoorUnlocked() == EDoorUnlocked::EEverySkill &&
			playerStats.dexterity >= nearestDoor->GetUnlockedAfterReachingSkill() &&
			playerStats.strength >= nearestDoor->GetUnlockedAfterReachingSkill() &&
			playerStats.intelligence >= nearestDoor->GetUnlockedAfterReachingSkill())
		{
			nearestDoor->StartOpeningDoor();

			if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				if (gameMode->DoesWidgetContainPlayerHUD())
				{
					gameMode->SpawnUnlockDoorWidgetOnHUD();
				}
			}
		}

		// Otherwise the player doesn't have enough skill to unlock any door
		else
		{
			if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				if (gameMode->DoesWidgetContainPlayerHUD())
				{
					gameMode->SpawnUnlockDoorWidgetOnHUD();
				}
			}
		}
	}
}

void APlayerCharacter::MoveToClosestFloorBehindThePlayer()
{
	// Check if there is a door nearby to set nearest door so that player cannot move past it
	ADoor* nearestDoor = nullptr;
	AActor* nearestEnemy = nullptr;

	FHitResult hitResult;
	FCollisionQueryParams queryParams;

	queryParams.AddIgnoredActor(this);

	for (TActorIterator<AFloor> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() && It->GetLevel()->bIsVisible && actionComponent)
		{
			const FVector& directionToFloor = (It->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			const float& dotProduct = FVector::DotProduct(GetActorForwardVector(), directionToFloor);

			for (TActorIterator<ADoor> doorIt(GetWorld()); doorIt; ++doorIt)
			{
				if (doorsUnlocked < (*It)->GetFloorNumberForDoorsUnlocked() && IsDoorNearby(*doorIt) &&
					GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorForwardVector() * -10000.0f,
						ECC_Visibility, queryParams) && hitResult.GetActor() && hitResult.GetActor() == *doorIt)
				{
					nearestDoor = *doorIt;
					break;
				}

				else continue;
			}

			for (TActorIterator<AActor> enemyIt(GetWorld()); enemyIt; ++enemyIt)
			{
				if (enemyIt->GetLevel() && enemyIt->GetLevel()->bIsVisible && enemyIt->ActorHasTag(enemyTag))
				{
					const FVector& directionToEnemy = (enemyIt->GetActorLocation() - GetActorLocation()).GetSafeNormal();
					const float& dotProductToEnemy = FVector::DotProduct(GetActorForwardVector(), directionToEnemy);

					if (IsEnemyNearby(*It, *enemyIt) && IsEnemyNearby(*enemyIt) && dotProductToEnemy < -dotProductValue)
					{
						nearestEnemy = *enemyIt;
						break;
					}
				}

				else continue;
			}

			if (dotProduct < -dotProductValue && WithinTheRangeOfNextFloor(*It) &&
				!actionComponent->HasBlockedTag(movingAction) && !nearestDoor && !nearestEnemy)
			{
				const FVector& nextFloorLocation = FVector(It->GetActorLocation().X, It->GetActorLocation().Y,
					GetActorLocation().Z);

				SetActorLocation(nextFloorLocation);
				PlayFootstepSounds();

				actionComponent->GameplayBlockedTagsAdded.Broadcast({ movingAction });

				FTimerHandle movementTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(movementTimerHandle, this, &APlayerCharacter::EnableMovement,
					movementCooldownTime, false);

				break;
			}

			else if (nearestDoor || nearestEnemy) break;

			else continue;
		}

		else continue;
	}
}

void APlayerCharacter::MoveToClosestFloorToLeft()
{
	// Check if there is a door nearby to set nearest door so that player cannot move past it
	ADoor* nearestDoor = nullptr;
	AActor* nearestEnemy = nullptr;

	FHitResult hitResult;
	FCollisionQueryParams queryParams;

	queryParams.AddIgnoredActor(this);

	for (TActorIterator<AFloor> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() && It->GetLevel()->bIsVisible && actionComponent)
		{
			const FVector& directionToFloor = (It->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			const float& dotProduct = FVector::DotProduct(GetActorRightVector(), directionToFloor);

			for (TActorIterator<ADoor> doorIt(GetWorld()); doorIt; ++doorIt)
			{
				if (doorsUnlocked < (*It)->GetFloorNumberForDoorsUnlocked() && IsDoorNearby(*doorIt) &&
					GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorRightVector() * -10000.0f,
						ECC_Visibility, queryParams) && hitResult.GetActor() && hitResult.GetActor() == *doorIt)
				{
					nearestDoor = *doorIt;
					break;
				}

				else continue;
			}

			for (TActorIterator<AActor> enemyIt(GetWorld()); enemyIt; ++enemyIt)
			{
				if (enemyIt->GetLevel() && enemyIt->GetLevel()->bIsVisible && enemyIt->ActorHasTag(enemyTag))
				{
					const FVector& directionToEnemy = (enemyIt->GetActorLocation() - GetActorLocation()).GetSafeNormal();
					const float& dotProductToEnemy = FVector::DotProduct(GetActorRightVector(), directionToEnemy);

					if (IsEnemyNearby(*It, *enemyIt) && IsEnemyNearby(*enemyIt) && dotProductToEnemy < -dotProductValue)
					{
						nearestEnemy = *enemyIt;
						break;
					}
				}

				else continue;
			}

			if (dotProduct < -dotProductValue && WithinTheRangeOfNextFloor(*It) &&
				!actionComponent->HasBlockedTag(movingAction) && !nearestDoor && !nearestEnemy)
			{
				const FVector& nextFloorLocation = FVector(It->GetActorLocation().X, It->GetActorLocation().Y,
					GetActorLocation().Z);

				SetActorLocation(nextFloorLocation);
				PlayFootstepSounds();

				actionComponent->GameplayBlockedTagsAdded.Broadcast({ movingAction });

				FTimerHandle movementTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(movementTimerHandle, this, &APlayerCharacter::EnableMovement,
					movementCooldownTime, false);

				break;
			}

			else if (nearestDoor || nearestEnemy) break;

			else continue;
		}

		else continue;
	}
}

void APlayerCharacter::MoveToClosestFloorToRight()
{
	// Check if there is a door nearby to set nearest door so that player cannot move past it
	ADoor* nearestDoor = nullptr;
	AActor* nearestEnemy = nullptr;

	FHitResult hitResult;
	FCollisionQueryParams queryParams;

	queryParams.AddIgnoredActor(this);

	for (TActorIterator<AFloor> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() && It->GetLevel()->bIsVisible && actionComponent)
		{
			const FVector& directionToFloor = (It->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			const float& dotProduct = FVector::DotProduct(GetActorRightVector(), directionToFloor);

			for (TActorIterator<ADoor> doorIt(GetWorld()); doorIt; ++doorIt)
			{
				if (doorsUnlocked < (*It)->GetFloorNumberForDoorsUnlocked() && IsDoorNearby(*doorIt) &&
					GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorRightVector() * 10000.0f,
						ECC_Visibility, queryParams) && hitResult.GetActor() && hitResult.GetActor() == *doorIt)
				{
					nearestDoor = *doorIt;
					break;
				}

				else continue;
			}

			for (TActorIterator<AActor> enemyIt(GetWorld()); enemyIt; ++enemyIt)
			{
				if (enemyIt->GetLevel() && enemyIt->GetLevel()->bIsVisible && enemyIt->ActorHasTag(enemyTag))
				{
					const FVector& directionToEnemy = (enemyIt->GetActorLocation() - GetActorLocation()).GetSafeNormal();
					const float& dotProductToEnemy = FVector::DotProduct(GetActorRightVector(), directionToEnemy);

					if (IsEnemyNearby(*It, *enemyIt) && IsEnemyNearby(*enemyIt) && dotProductToEnemy > dotProductValue)
					{
						nearestEnemy = *enemyIt;
						break;
					}
				}

				else continue;
			}

			if (dotProduct > dotProductValue && WithinTheRangeOfNextFloor(*It) &&
				!actionComponent->HasBlockedTag(movingAction) && !nearestDoor && !nearestEnemy)
			{
				const FVector& nextFloorLocation = FVector(It->GetActorLocation().X, It->GetActorLocation().Y,
					GetActorLocation().Z);

				SetActorLocation(nextFloorLocation);
				PlayFootstepSounds();

				actionComponent->GameplayBlockedTagsAdded.Broadcast({ movingAction });

				FTimerHandle movementTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(movementTimerHandle, this, &APlayerCharacter::EnableMovement,
					movementCooldownTime, false);

				break;
			}

			else if (nearestDoor || nearestEnemy) break;

			else continue;
		}

		else continue;
	}
}

bool APlayerCharacter::WithinTheRangeOfNextFloor(AActor* floorActor_)
{
	if (!floorActor_)
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Error: Floor actor is null"));
#endif

		return false;
	}

	const float& distanceToNextFloor = FVector::Dist(GetActorLocation(), floorActor_->GetActorLocation());
	return distanceToNextFloor <= maxDistanceToNextFloor && distanceToNextFloor >= minDistanceToNextFloor;
}

bool APlayerCharacter::IsDoorNearby(AActor* doorActor_)
{
	if (!doorActor_) return false;

	// Check if there is a door nearby to set nearest door so that player cannot move past it
	const float& distanceToDoor = FVector::Dist(GetActorLocation(), doorActor_->GetActorLocation());
	const bool& playerNearDoor = distanceToDoor <= maxDistanceToDoor && distanceToDoor >= minDistanceToDoor;

	if (playerNearDoor) return true;

	return false;
}

bool APlayerCharacter::IsChestNearby(AActor* chestActor_)
{
	if (!chestActor_) return false;

	// Check if there is a chest near the player so that they can interact with it
	const float& distanceToChest = FVector::Dist(GetActorLocation(), chestActor_->GetActorLocation());
	const bool& playerNearChest = distanceToChest <= maxDistanceToChest && distanceToChest >= minDistanceToChest;

	if (playerNearChest) return true;

	return false;
}

bool APlayerCharacter::IsEnemyNearby(AActor* enemyActor_)
{
	if (!enemyActor_)
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Error: Enemy actor is null"));
#endif

		return false;
	}

	const float& distanceToEnemy = FVector::Dist(GetActorLocation(), enemyActor_->GetActorLocation());
	return distanceToEnemy <= maxDistanceToNextFloor;
}

bool APlayerCharacter::IsEnemyNearby(AActor* floorActor_, AActor* enemyActor_)
{
	if (!enemyActor_ || !floorActor_)
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Error: Enemy/Floor actor is null"));
#endif

		return false;
	}

	//const float& distanceToEnemy = FVector::Dist(floorActor_->GetActorLocation(), enemyActor_->GetActorLocation());
	//return distanceToEnemy <= enemyDistanceToFloor;

	const float& distanceAbsX = FMath::Abs(floorActor_->GetActorLocation().X - enemyActor_->GetActorLocation().X);
	const float& distanceAbsY = FMath::Abs(floorActor_->GetActorLocation().Y - enemyActor_->GetActorLocation().Y);

	return distanceAbsX <= enemyAbsDistanceToFloor && distanceAbsY <= enemyAbsDistanceToFloor;
}

void APlayerCharacter::IncrementDoorsUnlocked()
{
	++doorsUnlocked;

	for (TActorIterator<AFloor> floorIt(GetWorld()); floorIt; ++floorIt)
	{
		if (floorIt->GetLevel() && floorIt->GetLevel()->bIsVisible)
		{
			if (doorsUnlocked >= (*floorIt)->GetFloorNumberForDoorsUnlocked())
			{
				if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					if (gameMode->DoesWidgetContainPlayerHUD())
					{
						gameMode->DestroyUnlockDoorWidgetOnHUD();
					}
				}

				break;
			}
		}

		else continue;
	}
}

/*int APlayerCharacter::GetEnemiesToDefeatInRoom() const
{
	if (enemiesToDefeatInRoom.Num() > enemiesDefeatedOnFloor)
		return enemiesToDefeatInRoom[enemiesDefeatedOnFloor];

	return 0;
}

void APlayerCharacter::IncrementEnemiesDefeatedInRoom()
{
	if (enemiesDefeatedInRoom < enemiesToDefeatInRoom[enemiesDefeatedOnFloor] &&
		enemiesToDefeatInRoom.Num() > enemiesDefeatedOnFloor)
	{
		++enemiesDefeatedInRoom;
	}

	if (enemiesDefeatedInRoom >= enemiesToDefeatInRoom[enemiesDefeatedOnFloor] &&
		enemiesToDefeatInRoom.Num() > enemiesDefeatedOnFloor)
	{
		++enemiesDefeatedOnFloor;
		enemiesDefeatedInRoom = 0;
	}
}*/

void APlayerCharacter::EnableMovement()
{
	if (actionComponent) actionComponent->GameplayBlockedTagsRemoved.Broadcast({ movingAction });
}

void APlayerCharacter::UnblockAttackAction()
{
	if (actionComponent) actionComponent->GameplayBlockedTagsRemoved.Broadcast({ attackAction });
}

void APlayerCharacter::EnableInteractionWithChest()
{
	if (actionComponent) actionComponent->GameplayBlockedTagsRemoved.Broadcast({ collectFromChestAction });
}

void APlayerCharacter::UpdateHealthHUD(UHealthComponent* HealthComp, float Health, float HealthDelta, 
	const class UDamageType* DamageType)
{
	if (healthComponent)
	{
		if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			if (gameMode->DoesWidgetContainPlayerHUD())
			{
				gameMode->UpdatePlayerHealth(healthComponent->GetHealthPercentage());
			}
		}
	}
}

void APlayerCharacter::DisablePlayerInput()
{
	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC) DisableInput(PC);

	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetPlayerHUDClass());
	}

	FTimerHandle gameOverTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(gameOverTimerHandle, this, &APlayerCharacter::ShowGameOverScreen, 
		timeToShowGameOver, false);
}

void APlayerCharacter::ShowGameOverScreen()
{
	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->EnableMouseCursor();

		gameMode->PushWidget(gameMode->GetGameOverClass());
	}

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC) EnableInput(PC);
}

void APlayerCharacter::UpdateDiceRoll()
{
	dieRoll = FMath::RandRange(1, 6);

#ifdef UE_EDITOR
	GEngine->AddOnScreenDebugMessage(100, 2.0f, FColor::Green, FString(TEXT("Dice Roll: ") + FString::FromInt(dieRoll)));
#endif

	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->UpdateDiceRoll();
	}

	if (!GetWorld()->GetTimerManager().IsTimerActive(landOnDie))
	{
		GetWorld()->GetTimerManager().SetTimer(landOnDie, this, &APlayerCharacter::LandOnDieNumber, timeToLandOnDieNumber,
			false);
	}
}

void APlayerCharacter::LandOnDieNumber()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(rollDiceHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(rollDiceHandle);
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(landOnDie))
	{
		GetWorld()->GetTimerManager().ClearTimer(landOnDie);
	}

#ifdef UE_EDITOR
	GEngine->AddOnScreenDebugMessage(100, 2.0f, FColor::Green, FString(TEXT("Dice Roll: ") + FString::FromInt(dieRoll)));
#endif

	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->UpdateDiceRoll();
	}
}

void APlayerCharacter::PlayFootstepSounds()
{
	const int& randomFootstepSoundIndex = FMath::RandRange(0, footstepSounds.Num() - 1);

	if (!footstepSounds.IsEmpty() && footstepSounds[randomFootstepSoundIndex])
		UGameplayStatics::PlaySound2D(GetWorld(), footstepSounds[randomFootstepSoundIndex]);
}

void APlayerCharacter::DealDamageToBloodDrinker()
{
	ABloodDrinker* nearestBloodDrinker = nullptr;

	for (TActorIterator<ABloodDrinker> enemyIt(GetWorld()); enemyIt; ++enemyIt)
	{
		const FVector& directionToEnemy = (enemyIt->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		const float& dotProduct = FVector::DotProduct(GetActorForwardVector(), directionToEnemy);

		if (enemyIt->GetLevel() && enemyIt->GetLevel()->bIsVisible && IsEnemyNearby(*enemyIt) &&
			dotProduct > dotProductValue)
		{
			nearestBloodDrinker = *enemyIt;
			break;
		}

		else continue;
	}

	if (nearestBloodDrinker && !actionComponent->HasBlockedTag(attackAction))
	{
		nearestBloodDrinker->TakeDamage(damagedValueForBloodDrinker, FDamageEvent(), nullptr, this);

		actionComponent->GameplayBlockedTagsAdded.Broadcast({ attackAction });

		FTimerHandle attackTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &APlayerCharacter::UnblockAttackAction,
			attackCooldownTime, false);
	}
}