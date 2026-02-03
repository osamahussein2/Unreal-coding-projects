// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "InputMappingContext.h"
#include "Input/PlayerInputDA.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Components/CustomerInteractionComponent.h"
#include "Components/DropFoodComponent.h"
#include "Components/CookingComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetMesh());

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetupAttachment(springArm);
}

void APlayerCharacter::SetPlayerStateToCooking()
{
	playerState = EPlayerState::ECooking;
}

void APlayerCharacter::SetPlayerStateToCarryingFood()
{
	playerState = EPlayerState::ECarryingFood;

	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->DestroyCustomerOrderWidget();

		if (toggleOverlayVisibility) toggleOverlayVisibility = false;
	}
}

void APlayerCharacter::AllowPlayerToFreeRoamAgain()
{
	playerState = EPlayerState::EFreeRoam;
}

void APlayerCharacter::IncrementFoodsDropped()
{
	++foodsDropped;
}

void APlayerCharacter::ShowOverlayOrder()
{
	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->ShowCustomerOrderOverlay();
	}
}

void APlayerCharacter::ResetValues()
{
	if (customerNumber != 0) customerNumber = 0;
	if (receivedOrderFromCustomer != false) receivedOrderFromCustomer = false;
	if (playerState != EPlayerState::EFreeRoam) playerState = EPlayerState::EFreeRoam;
	if (startTouchLocation != 0.0f) startTouchLocation = 0.0f;

	if (GetActorLocation() != startingLocation) SetActorLocation(startingLocation);
	if (Controller && Controller->GetControlRotation() != startingRotation) Controller->SetControlRotation(startingRotation);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	startingLocation = GetActorLocation();
	startingRotation = GetControlRotation();
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

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
		(Cast<APlayerController>(GetController())->GetLocalPlayer());

	// Add the input mapping context
	enhancedInputSubsystem->AddMappingContext(inputMappingContext, 0);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (playerInputDA && playerInputDA->GetIA_Move())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_Move(), ETriggerEvent::Triggered, this, 
				&APlayerCharacter::MoveCharacter);
		}

		if (playerInputDA && playerInputDA->GetIA_CameraSpin())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_CameraSpin(), ETriggerEvent::Started, this,
				&APlayerCharacter::StartTouchInput);

			EnhancedInputComponent->BindAction(playerInputDA->GetIA_CameraSpin(), ETriggerEvent::Triggered, this, 
				&APlayerCharacter::SpinCamera);
		}

		if (playerInputDA && playerInputDA->GetIA_CameraZoom())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_CameraZoom(), ETriggerEvent::Triggered, this,
				&APlayerCharacter::UpdateCameraZoom);
		}

		if (playerInputDA && playerInputDA->GetIA_ToggleCook())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_ToggleCook(), ETriggerEvent::Started, this,
				&APlayerCharacter::Interact);
		}

		if (playerInputDA && playerInputDA->GetIA_DropFood())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_DropFood(), ETriggerEvent::Started, this,
				&APlayerCharacter::StartDroppingFood);
		}

		if (playerInputDA && playerInputDA->GetIA_PauseGame())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_PauseGame(), ETriggerEvent::Started, this,
				&APlayerCharacter::PauseGame);
		}
	}

}

void APlayerCharacter::MoveCharacter(const FInputActionValue& value_)
{
	// Player shouldn't move while the cine camera is valid to prevent glitch before carrying food
	if (cooking && cooking->GetCineCamera()) return;

	AddMovementInput(GetActorRightVector(), value_.Get<FVector2D>().X);
	AddMovementInput(GetActorForwardVector(), value_.Get<FVector2D>().Y);
}

void APlayerCharacter::StartTouchInput(const FInputActionValue& value_)
{
	if (isForMobile) startTouchLocation = value_.Get<float>();
}

void APlayerCharacter::SpinCamera(const FInputActionValue& value_)
{
	if (cooking && cooking->GetCineCamera()) return;

	// For example, we're building for PC and/or consoles
	if (!isForMobile)
	{
		const float spinValue = value_.Get<float>();

		if (!FMath::IsNearlyZero(spinValue) && Controller)
		{
			FRotator ControlRot = Controller->GetControlRotation();
			ControlRot.Yaw += spinValue * GetWorld()->GetDeltaSeconds() * spinSensitivity;
			Controller->SetControlRotation(ControlRot);
		}
	}

	// Building for mobile
	else
	{
		// Mobile camera
		// Calculate the delta touch location based on difference between current value and the start touch location
		float deltaTouchLocation = value_.Get<float>() - startTouchLocation;

		// Spin the controller camera based on swipe location
		if (!FMath::IsNearlyZero(deltaTouchLocation) && Controller)
		{
			FRotator ControlRot = Controller->GetControlRotation();
			ControlRot.Yaw += deltaTouchLocation * GetWorld()->GetDeltaSeconds() * spinSensitivity;
			Controller->SetControlRotation(ControlRot);
		}

		// Update start touch location as well to swipe properly again
		startTouchLocation = value_.Get<float>();
	}
}

void APlayerCharacter::UpdateCameraZoom(const FInputActionValue& value_)
{
	if (!springArm || cooking && cooking->GetCineCamera()) return;

	springArm->TargetArmLength += value_.Get<float>() * zoomSensitivity;
	springArm->TargetArmLength = FMath::Clamp(springArm->TargetArmLength, minArmTargetLength, maxArmTargetLength);
}

void APlayerCharacter::Interact()
{
	StartOrEndCooking();
	TakeOrderFromCustomer();
}

void APlayerCharacter::StartDroppingFood()
{
	ToggleCustomerOrderOverlay();

	if (dropFood) dropFood->DropFood(playerState);
}

void APlayerCharacter::ToggleCustomerOrderOverlay()
{
	if (playerState == EPlayerState::EDroppingFood || playerState == EPlayerState::ECooking) return;

	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (gameMode->GetCustomerOrderWidget() && gameMode->GetCustomerOrderWidget()->GetHasFinishedUpdatingOrder())
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

			gameMode->DestroyInstructionWidget();
			gameMode->GetCustomerOrderWidget()->SetHasFinishedUpdatingOrderToFalse();
		}

		if (toggleOverlayVisibility) gameMode->HideCustomerOrderOverlay();
		else gameMode->ShowCustomerOrderOverlay();

		toggleOverlayVisibility = !toggleOverlayVisibility;
	}
}

void APlayerCharacter::StartOrEndCooking()
{
	if (cooking)
	{
		cooking->ToggleCookingState(playerState, toggleOverlayVisibility, cameraSpawnLocation, cameraSpawnRotation, this, 
			playerCookingLocation, Cast<APlayerController>(GetController()), FName("RightHandSocket"));
	}
}

void APlayerCharacter::TakeOrderFromCustomer()
{
	if (cooking && cooking->GetCineCamera()) return;

	if (customerInteraction)
	{
		customerInteraction->InteractWithCustomer(receivedOrderFromCustomer, customerNumber,
			toggleOverlayVisibility, foodsDropped, playerState);
	}
}

void APlayerCharacter::PauseGame()
{
	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->InitializePauseMenuWidget();
	}
}

void APlayerCharacter::StartDroppingBurger()
{
	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (playerState != EPlayerState::EDroppingFood) playerState = EPlayerState::EDroppingFood;

		gameMode->InitializeInstructionWidget("Press ENTER to drop food");
		gameMode->InitializeMobileWidget();

		if (gameMode->GetMobileWidget())
		{
			gameMode->GetMobileWidget()->ShowDropFoodButton();
			gameMode->GetMobileWidget()->SetDropFoodText("Drop Food");
		}

		gameMode->SpawnBurger();
		gameMode->DestroyPickFoodWidget();
		gameMode->DestroyPickFoodMobileWidget();

		SpawnDropFoodComponent();
	}
}

void APlayerCharacter::StartDroppingTopping(FString& toppingName_)
{
	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (playerState != EPlayerState::EDroppingFood) playerState = EPlayerState::EDroppingFood;

		gameMode->InitializeInstructionWidget("Press ENTER to drop food");
		gameMode->InitializeMobileWidget();

		if (gameMode->GetMobileWidget())
		{
			gameMode->GetMobileWidget()->ShowDropFoodButton();
			gameMode->GetMobileWidget()->SetDropFoodText("Drop Food");
		}

		gameMode->SpawnTopping(toppingName_);
		gameMode->DestroyPickFoodWidget();
		gameMode->DestroyPickFoodMobileWidget();

		SpawnDropFoodComponent();
	}
}

void APlayerCharacter::StartDroppingTopBun()
{
	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (playerState != EPlayerState::EDroppingFood) playerState = EPlayerState::EDroppingFood;

		gameMode->InitializeInstructionWidget("Press ENTER to drop food");
		gameMode->InitializeMobileWidget();

		if (gameMode->GetMobileWidget())
		{
			gameMode->GetMobileWidget()->ShowDropFoodButton();
			gameMode->GetMobileWidget()->SetDropFoodText("Drop Food");
		}

		gameMode->SpawnTopBun();
		gameMode->DestroyPickFoodWidget();
		gameMode->DestroyPickFoodMobileWidget();

		SpawnDropFoodComponent();
	}
}

void APlayerCharacter::SpawnCustomerInteractionComponent()
{
	if (!customerInteraction)
	{
		// Create a new customer interaction component object
		customerInteraction = NewObject<UCustomerInteractionComponent>(this, 
			UCustomerInteractionComponent::StaticClass(), TEXT("CustomerInteractionComponent"));

		// Register the created component during runtime
		customerInteraction->RegisterComponent();
	}
}

void APlayerCharacter::DestroyCustomerInteractionComponent()
{
	if (customerInteraction)
	{
		customerInteraction->UnregisterComponent();
		customerInteraction->DestroyComponent();
		customerInteraction = nullptr;
	}
}

void APlayerCharacter::SpawnDropFoodComponent()
{
	if (!dropFood)
	{
		// Create a new drop food component object
		dropFood = NewObject<UDropFoodComponent>(this, UDropFoodComponent::StaticClass(), TEXT("DropFoodComponent"));

		// Register the created component during runtime
		dropFood->RegisterComponent();
	}
}

void APlayerCharacter::DestroyDropFoodComponent()
{
	if (dropFood)
	{
		dropFood->UnregisterComponent();
		dropFood->DestroyComponent();
		dropFood = nullptr;
	}
}

void APlayerCharacter::SpawnCookingComponent()
{
	if (!cooking)
	{
		// Create a new cooking component object
		cooking = NewObject<UCookingComponent>(this, UCookingComponent::StaticClass(), TEXT("CookingComponent"));

		// Register the created component during runtime
		cooking->RegisterComponent();
	}
}

void APlayerCharacter::DestroyCookingComponent()
{
	if (cooking)
	{
		cooking->UnregisterComponent();
		cooking->DestroyComponent();
		cooking = nullptr;
	}
}

void APlayerCharacter::DestroyAnyComponentReferences()
{
	DestroyCustomerInteractionComponent();
	if (cooking) cooking->DestroyReferences();
	DestroyCookingComponent();
	DestroyDropFoodComponent();
}
