// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/PlayerInputDA.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/WidgetManager.h"
#include "Widget/PauseMenuWidget.h"
#include "Widget/PlayerHUDWidget.h"
#include "ArcadeMachine.h"
#include "EngineUtils.h"
#include "Coin.h"
#include "Guard.h"
#include "PrizeMachine.h"
#include "Components/AudioComponent.h"
#include "ShroomJam2026/ShroomJam2026.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetupAttachment(GetCapsuleComponent());

	magnetComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MagnetComponent"));
	magnetComponent->SetupAttachment(GetCapsuleComponent());

	footstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootstepAudioComponent"));
	footstepAudioComponent->SetupAttachment(GetCapsuleComponent());

	GetCapsuleComponent()->SetCollisionObjectType(COLLISION_PLAYER);
	GetMesh()->SetCollisionObjectType(COLLISION_PLAYER);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	for (TActorIterator<AGuard> guardIt(GetWorld()); guardIt; ++guardIt)
	{
		guardIt->SetPlayerCharacter(this);
	}

	for (TActorIterator<ACoin> coinIt(GetWorld()); coinIt; ++coinIt)
	{
		++totalCoins;
	}
	
	if (magnetComponent)
	{
		magnetComponent->IgnoreActorWhenMoving(this, true);
		magnetRelativeLocation = magnetComponent->GetRelativeLocation();
		magnetRelativeRotation = magnetComponent->GetRelativeRotation();
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (magnetState)
	{
	case EMagnetState::CollectCoin:

		if (magnetComponent && magnetComponent->IsValidLowLevel())
		{
			if (nearestCoin && nearestCoin->IsValidLowLevel() && arcadeMachine && arcadeMachine->IsValidLowLevel())
			{
				const FVector& MoveToCoin = FMath::VInterpTo(magnetComponent->GetComponentLocation(),
					nearestCoin->GetActorLocation(), DeltaTime, magnetMoveSpeed);

				magnetComponent->SetWorldLocation(MoveToCoin);
				magnetComponent->SetRelativeRotation(magnetRelativeRotation);

				if (FVector::Distance(magnetComponent->GetComponentLocation(), nearestCoin->GetActorLocation()) <= 5.0f)
				{
					arcadeMachine->RemoveCoinFromArray(nearestCoin);
					nearestCoin->Destroy();

					if (!coinCollectedSounds.IsEmpty())
					{
						const int& randomIndex = FMath::RandRange(0, coinCollectedSounds.Num() - 1);

						if (coinCollectedSounds[randomIndex])
						{
							UGameplayStatics::PlaySound2D(this, coinCollectedSounds[randomIndex]);
						}
					}

					++coinsAmount;
					if (OnCoinCollected.IsBound()) OnCoinCollected.Broadcast(coinsAmount);
					
					FindNearestCoin();
				}
			}
		}

		break;

	case EMagnetState::ResetBackIntoPlace:

		if (magnetComponent && magnetComponent->IsValidLowLevel())
		{
			const FVector& MoveBackInPlace = FMath::VInterpTo(magnetComponent->GetRelativeLocation(),
				magnetRelativeLocation, DeltaTime, magnetMoveSpeed);

			magnetComponent->SetRelativeLocation(MoveBackInPlace);
			magnetComponent->SetRelativeRotation(magnetRelativeRotation);

			if (FVector::Distance(magnetComponent->GetRelativeLocation(), magnetRelativeLocation) <= 5.0f)
			{
				magnetComponent->SetRelativeLocation(magnetRelativeLocation);
				magnetComponent->SetRelativeRotation(magnetRelativeRotation);

				magnetState = EMagnetState::WithPlayer;
				SetActorTickEnabled(false);
			}
		}

		break;

	default:
		break;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!playerInputDA || !playerInputDA->GetInputMappingContext()) return;

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (!playerController->GetLocalPlayer()) return;

		// Get the local player enhanced input subsystem
		UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			playerController->GetLocalPlayer());

		// Add the input mapping context
		enhancedInputSubsystem->AddMappingContext(playerInputDA->GetInputMappingContext(), 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind move action to the enhanced input component
		if (playerInputDA->GetMoveAction())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetMoveAction(), ETriggerEvent::Triggered, this,
				&APlayerCharacter::Move);
		}

		// Bind look action to the enhanced input component
		if (playerInputDA->GetLookAction())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetLookAction(), ETriggerEvent::Triggered, this,
				&APlayerCharacter::Look);
		}

		// Bind pause action to the enhanced input component
		if (playerInputDA->GetPauseAction())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetPauseAction(), ETriggerEvent::Started, this,
				&APlayerCharacter::PauseGame);
		}

		// Bind magnet action to the enhanced input component
		if (playerInputDA->GetMagnetAction())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetMagnetAction(), ETriggerEvent::Started, this,
				&APlayerCharacter::UseMagnet);
		}
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (magnetState != EMagnetState::WithPlayer) return;

	AddMovementInput(GetActorForwardVector(), Value.Get<FVector2D>().Y);
	AddMovementInput(GetActorRightVector(), Value.Get<FVector2D>().X);

	if (footstepAudioComponent && footstepAudioComponent->IsValidLowLevel())
	{
		if (!footstepAudioComponent->IsPlaying())
		{
			const int& randomIndex = FMath::RandRange(0, footstepSounds.Num() - 1);

			footstepAudioComponent->SetSound(footstepSounds[randomIndex]);
			footstepAudioComponent->Play();
		}
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	AddControllerPitchInput(cameraSensitivity * -Value.Get<FVector2D>().Y);
	AddControllerYawInput(cameraSensitivity * Value.Get<FVector2D>().X);
}

void APlayerCharacter::PauseGame()
{
	if (!UGameplayStatics::IsGamePaused(this))
	{
		if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
			{
				widgetManager->PopWidget(widgetManager->GetPlayerHUDClass());
				widgetManager->PushWidget(widgetManager->GetPauseMenuClass());

				widgetManager->EnableMouseCursor();

				UGameplayStatics::SetGamePaused(this, true);
			}
		}
	}
}

void APlayerCharacter::UseMagnet()
{
	if (magnetState != EMagnetState::WithPlayer) return;

	float nearestTarget = FLT_MAX;

	for (TActorIterator<AArcadeMachine> arcadeMachineIt(GetWorld()); arcadeMachineIt; ++arcadeMachineIt)
	{
		if (!arcadeMachineIt->GetPlayerInTrigger()) continue;

		float distance = FVector::Dist(GetActorLocation(), arcadeMachineIt->GetActorLocation());

		if (distance < nearestTarget)
		{
			nearestTarget = distance;
			arcadeMachine = *arcadeMachineIt;
		}
	}

	if (arcadeMachine)
	{
		float dotProduct = FVector::DotProduct(GetActorForwardVector(), (arcadeMachine->GetActorLocation() - GetActorLocation()).GetSafeNormal());
		if (dotProduct < 0.6f) return;

		float nearestCoinTarget = FLT_MAX;

		for (ACoin* coin : arcadeMachine->GetCoins())
		{
			float distance = FVector::Dist(GetActorLocation(), coin->GetActorLocation());

			if (!coin || distance >= nearestCoinTarget) continue;

			nearestCoinTarget = distance;
			nearestCoin = coin;
		}

		if (nearestCoin)
		{
			SetActorTickEnabled(true);
			magnetState = EMagnetState::CollectCoin;
		}
	}
}

void APlayerCharacter::FindNearestCoin()
{
	if (!arcadeMachine) return;

	if (nearestCoin) nearestCoin = nullptr;

	float nearestCoinTarget = FLT_MAX;

	for (ACoin* coin : arcadeMachine->GetCoins())
	{
		if (!coin) continue;

		float distance = FVector::Dist(GetActorLocation(), coin->GetActorLocation());

		if (distance < nearestCoinTarget)
		{
			nearestCoinTarget = distance;
			nearestCoin = coin;
		}
	}

	if (nearestCoin)
	{
		if (!IsActorTickEnabled()) SetActorTickEnabled(true);
		if (magnetState != EMagnetState::CollectCoin) magnetState = EMagnetState::CollectCoin;
	}

	else
	{
		magnetState = EMagnetState::ResetBackIntoPlace;

		arcadeMachine->StopFlashing();
		arcadeMachine = nullptr;

		if (coinsAmount < totalCoins || !GetWorld()) return;

		for (TActorIterator<APrizeMachine> prizeMachineIt(GetWorld()); prizeMachineIt; ++prizeMachineIt)
		{
			prizeMachineIt->TurnFlashOn();
		}
	}
}