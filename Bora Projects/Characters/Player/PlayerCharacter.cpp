// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SceneCapture2D.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize the camera component
	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComponent"));
	cameraComponent->SetupAttachment(RootComponent);

	minimapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArm"));
	minimapSpringArm->SetupAttachment(GetCapsuleComponent());

	sceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapSceneCapture"));
	sceneCaptureComponent->SetupAttachment(minimapSpringArm);

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
	spriteComponent->SetupAttachment(GetMesh());

	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	gameplayTagComponent = CreateDefaultSubobject<UGameplayTagComponent>(TEXT("GameplayTagComponent"));

	healthRegenerationComponent = CreateDefaultSubobject<UHealthRegenerationComponent>(TEXT("HealthRegenerationComponent"));

	musicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicComponent"));
	musicComponent->SetupAttachment(GetCapsuleComponent());

	playerHUD = nullptr;
	pauseMenu = nullptr;

	startingWalkSpeed = 0.0f;

	// Mobile related variables should be null and false upon construction if we're building for PC
	gameTouchInterface = nullptr;

	isTouchInterfaceEnabled = false;
	canDragCamera = false;

	startTouchLocation = FVector2D::Zero();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Find a scene capture actor in the level
	TArray<AActor*> FoundSceneCaptureActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASceneCapture2D::StaticClass(), FoundSceneCaptureActors);
	
	// Assign a scene capture actor at found scene capture actor
	sceneCapture = Cast<ASceneCapture2D>(FoundSceneCaptureActors[0]);

	// Once a scene capture is valid, attach it to the camera component to render it on TV
	if (sceneCapture)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Scene capture found!"));
		sceneCapture->AttachToComponent(cameraComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	// Find a cine camera actor in the level
	TArray<AActor*> FoundCineCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACineCameraActor::StaticClass(), FoundCineCameras);

	// Assign the cine camera to the found cine camera actor
	cineCamera = Cast<ACineCameraActor>(FoundCineCameras[0]);

	if (gameplayTagComponent)
	{
		// Bind all gameplay tag component delegates
		gameplayTagComponent->GameplayTagChanged.AddDynamic(this, &APlayerCharacter::ChangeGameplayTag);
		gameplayTagComponent->GameplayTagAdded.AddDynamic(this, &APlayerCharacter::AddToGameplayTag);
		gameplayTagComponent->GameplayTagRemoved.AddDynamic(this, &APlayerCharacter::RemoveGameplayTag);

		// Start broadcaasting the add gameplay tag logic and pass in an initialized Idle state
		gameplayTagComponent->GameplayTagAdded.Broadcast("PlayerActions.Idle");
	}

	// Set the camera component's z position based on set max standing position by the designer in blueprint
	if (cameraComponent)
	{
		cameraComponent->SetRelativeLocation(FVector(cameraComponent->GetRelativeLocation().X, 
			cameraComponent->GetRelativeLocation().Y, maxStandingPosition));
	}

	if (healthRegenerationComponent)
	{
		healthRegenerationComponent->StartHealing.AddDynamic(this, &APlayerCharacter::HealPlayer);
		healthRegenerationComponent->StartDamaged.AddDynamic(this, &APlayerCharacter::SetPlayerDamaged);
	}

	if (musicComponent)
	{
		musicComponent->SetSound(LoadObject<USoundBase>(this, TEXT("/Game/Music/gameplay_track")));
		musicComponent->Play();
	}

	// Start with default max walk speed
	startingWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the scene capture's rotation if it's valid that is
	if (sceneCapture && sceneCapture->GetActorRotation() != GetControlRotation())
	{
		sceneCapture->SetActorRotation(GetControlRotation());
	}

	/* Check if gameplay tag component is valid and doesn't have the idle action tag, as well as character's
	movement being less than 0.01 which is close to 0 (stopping movement) */
	if (gameplayTagComponent && UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity) <= 0.01f &&
		!gameplayTagComponent->HasActionTag("PlayerActions.Idle"))
	{
		gameplayTagComponent->GameplayTagChanged.Broadcast("PlayerActions.Idle", "PlayerActions.Walking");
	}

	RegeneratePlayerHealth(DeltaTime);
	UpdatePlayerHealthInformation(DeltaTime);

	HidePauseMenu();

	if (healthRegenerationComponent->IsDead())
	{
		SetGameOverScreen();
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the player controller
	playerController = Cast<APlayerController>(GetController());

	if (!playerController) return;

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());

	// Add the input mapping context
	enhancedInputSubsystem->AddMappingContext(inputMapping, 0);

	// Get the EnhancedInputComponent
	UEnhancedInputComponent* playerEnhancedInputcomponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//Bind Move() to the mapping
	//BindAction for enhanced system takes Action, ETriggerEvent, object, and function
	//ETriggerEvent is an enum, where Triggered means "button is held down".

	if (PlayerDataAsset && PlayerDataAsset->IA_Move)
	{
		playerEnhancedInputcomponent->BindAction(PlayerDataAsset->IA_Move, ETriggerEvent::Triggered, this,
			&APlayerCharacter::MoveCharacter);
	}

	if (PlayerDataAsset && PlayerDataAsset->IA_Camera)
	{
		playerEnhancedInputcomponent->BindAction(PlayerDataAsset->IA_Camera, ETriggerEvent::Triggered, this,
			&APlayerCharacter::MoveCamera);
	}

	if (PlayerDataAsset && PlayerDataAsset->IA_Pause)
	{
		playerEnhancedInputcomponent->BindAction(PlayerDataAsset->IA_Pause, ETriggerEvent::Triggered, this,
			&APlayerCharacter::ShowPauseMenu);
	}

	// Hold/release logic for crouch
	if (PlayerDataAsset && PlayerDataAsset->IA_Crouch)
	{
		playerEnhancedInputcomponent->BindAction(PlayerDataAsset->IA_Crouch, ETriggerEvent::Ongoing, this,
			&APlayerCharacter::StartCrouch);

		playerEnhancedInputcomponent->BindAction(PlayerDataAsset->IA_Crouch, ETriggerEvent::None, this,
			&APlayerCharacter::EndCrouch);
	}

	// Mobile camera controls
	if (PlayerDataAsset && PlayerDataAsset->IA_Touch && gameTouchInterface)
	{
		playerEnhancedInputcomponent->BindAction(PlayerDataAsset->IA_Touch, ETriggerEvent::Started, this,
			&APlayerCharacter::StartTouchInput);

		playerEnhancedInputcomponent->BindAction(PlayerDataAsset->IA_Touch, ETriggerEvent::Ongoing, this,
			&APlayerCharacter::MobileControlCamera);
	}
}

void APlayerCharacter::MoveCharacter(const FInputActionValue& value_)
{
	AddMovementInput(GetActorRightVector(), value_.Get<FVector2D>().X);
	AddMovementInput(GetActorForwardVector(), value_.Get<FVector2D>().Y);

	// Change the gameplay tag from idle to walking
	if (gameplayTagComponent)
	{
		gameplayTagComponent->GameplayTagChanged.Broadcast("PlayerActions.Walking", "PlayerActions.Idle");
	}
}

void APlayerCharacter::MoveCamera(const FInputActionValue& value_)
{
	AddControllerYawInput(value_.Get<FVector2D>().X);
	AddControllerPitchInput(value_.Get<FVector2D>().Y);
}

UHealthRegenerationComponent* APlayerCharacter::GetHealthRegenerationComponent() const
{
	return healthRegenerationComponent;
}

void APlayerCharacter::ShowPauseMenu()
{
	HidePlayerHUD();

	// Hide mobile interface when pausing game
	if (gameTouchInterface && playerController)
	{
		playerController->ActivateTouchInterface(nullptr);
		isTouchInterfaceEnabled = false;
		canDragCamera = false;
	}

	if (!pauseMenu && playerController)
	{
		pauseMenu = CreateWidget<UPauseMenuWidget>(playerController, pauseMenuClass);

		//EnableMouseCursor();
	}

	if (pauseMenu) pauseMenu->AddToViewport();
}

void APlayerCharacter::HidePauseMenu()
{
	if (pauseMenu && !pauseMenu->IsInViewport())
	{
		// Show mobile interface when resuming the game
		if (gameTouchInterface && playerController)
		{
			playerController->ActivateTouchInterface(gameTouchInterface);
			isTouchInterfaceEnabled = true;
			canDragCamera = true;
		}

		//DisableMouseCursor();

		ShowPlayerHUD();

		pauseMenu = nullptr;
	}
}

void APlayerCharacter::StartCrouch()
{
	if (!gameplayTagComponent->HasActionTag("PlayerActions.Crouching"))
	{
		// Set actual max walk speed to the max crouched speed
		GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;

		// Start broadcaasting the add gameplay tag logic and pass in crouch state
		gameplayTagComponent->GameplayTagAdded.Broadcast("PlayerActions.Crouching");
	}

	else
	{
		// Move the camera down when crouching
		if (cameraComponent && cameraComponent->GetRelativeLocation().Z > maxCrouchPosition)
		{
			cameraComponent->AddRelativeLocation(FVector(0.0f, 0.0f, -cameraMoveSpeed));
		}
	}
}

void APlayerCharacter::EndCrouch()
{
	if (gameplayTagComponent->HasActionTag("PlayerActions.Crouching"))
	{
		// Set actual max walk speed to back to the starting walk speed set in BeginPlay()
		GetCharacterMovement()->MaxWalkSpeed = startingWalkSpeed;

		// Start broadcaasting the remove gameplay tag logic and pass in crouch state
		gameplayTagComponent->GameplayTagRemoved.Broadcast("PlayerActions.Crouching");
	}

	else
	{
		// Move the camera up when standing
		if (cameraComponent && cameraComponent->GetRelativeLocation().Z < maxStandingPosition)
		{
			cameraComponent->AddRelativeLocation(FVector(0.0f, 0.0f, cameraMoveSpeed));
		}
	}
}

void APlayerCharacter::AssignCineCameraToPlayer()
{
	// If cine camera is valid, change the player's view target to the found cine camera in level
	if (cineCamera)
	{
		playerController->SetViewTargetWithBlend(cineCamera);
		//UE_LOG(LogTemp, Warning, TEXT("View Target set"));
	}
}

// This function will be used inside other classes when getting the cast to this character
void APlayerCharacter::ShouldPauseMusic(bool paused_)
{
	musicComponent->SetPaused(paused_);
}

void APlayerCharacter::ShowPlayerHUD()
{
	if (!playerHUD && playerController)
	{
		playerHUD = CreateWidget<UPlayerHUDWidget>(playerController, playerHUDClass);
	}

	if (playerHUD)
	{
		playerHUD->AddToViewport();

		// Also update the health bars on player's HUD
		if (healthRegenerationComponent)
		{
			playerHUD->SetHealthBar(healthRegenerationComponent->GetCurrentHealth(),
				healthRegenerationComponent->GetMaxHealth());

			playerHUD->UpdateHealthBar(healthRegenerationComponent->GetUpdatedHealth(),
				healthRegenerationComponent->GetMaxHealth());
		}
	}
}

void APlayerCharacter::HidePlayerHUD()
{
	if (playerHUD)
	{
		playerHUD->RemoveFromParent();
		playerHUD = nullptr;
	}
}

void APlayerCharacter::SetGameOverScreen()
{
	HidePlayerHUD();

	if (!playerController) return;

	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_None)
	{
		GetCharacterMovement()->DisableMovement();
		DisableInput(playerController);
	}

	FTimerHandle gameOverTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(gameOverTimerHandle, this, &APlayerCharacter::ShowGameOverScreen, 1.0f, false);
}

void APlayerCharacter::ShowGameOverScreen()
{
	UGameOverWidget* gameOver = CreateWidget<UGameOverWidget>(playerController, gameOverClass);

	if (gameOver)
	{
		//EnableMouseCursor();

		gameOver->AddToViewport();
		gameOver = nullptr;
	}
}

void APlayerCharacter::EnableMouseCursor()
{
	// Exclusive to PC
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false); // Don't hide the cursor during capture

	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Don't worry about mouse lock

	if (playerController)
	{
		playerController->SetInputMode(InputMode);

		if (playerController->bShowMouseCursor != true) playerController->bShowMouseCursor = true;
	}
}

void APlayerCharacter::DisableMouseCursor()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

	if (playerController)
	{
		playerController->SetInputMode(GameInputMode);

		// Make sure the mouse cursor is turned off when resuming the game
		if (playerController->bShowMouseCursor != false) playerController->bShowMouseCursor = false;
	}
}

void APlayerCharacter::SetPlayerDamaged(float newValue)
{
	if (healthRegenerationComponent && playerHUD)
	{
		healthRegenerationComponent->SetDamaged(newValue);

		playerHUD->SetHealthBar(healthRegenerationComponent->GetCurrentHealth(),
			healthRegenerationComponent->GetMaxHealth());
	}
}

void APlayerCharacter::HealPlayer()
{
	if (healthRegenerationComponent && playerHUD)
	{
		healthRegenerationComponent->SetHealing();

		playerHUD->UpdateHealthBar(healthRegenerationComponent->GetUpdatedHealth(),
			healthRegenerationComponent->GetMaxHealth());
	}
}

void APlayerCharacter::ShowGameWonScreen()
{
	HidePlayerHUD();

	if (!playerController) return;

	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_None)
	{
		GetCharacterMovement()->DisableMovement();
		DisableInput(playerController);
	}

	UGameWinWidget* gameWin = CreateWidget<UGameWinWidget>(playerController, gameWinClass);

	if (gameWin)
	{
		//EnableMouseCursor();

		gameWin->AddToViewport();
		gameWin = nullptr;
	}
}

void APlayerCharacter::RegeneratePlayerHealth(float& DeltaTime)
{
	if (healthRegenerationComponent && !healthRegenerationComponent->IsHealing() && 
		!healthRegenerationComponent->IsDamaged() && 
		healthRegenerationComponent->GetCurrentHealth() < healthRegenerationComponent->GetMaxHealth())
	{
		if (healTimer < healthRegenerationComponent->GetTimeUntilHeal())
		{
			healTimer += DeltaTime;
		}

		else
		{
			healthRegenerationComponent->StartHealing.Broadcast();
			healTimer = 0.0f;
		}
	}
}

void APlayerCharacter::UpdatePlayerHealthInformation(float& DeltaTime)
{
	if (healthRegenerationComponent && playerHUD)
	{
		if (healthRegenerationComponent->IsDamaged())
		{
			healthRegenerationComponent->UpdateHealthDamaged(DeltaTime);

			playerHUD->UpdateHealthBar(healthRegenerationComponent->GetUpdatedHealth(),
				healthRegenerationComponent->GetMaxHealth());
		}

		if (healthRegenerationComponent->IsHealing())
		{
			healthRegenerationComponent->UpdateHealthRenegeneration(DeltaTime);

			playerHUD->SetHealthBar(healthRegenerationComponent->GetCurrentHealth(),
				healthRegenerationComponent->GetMaxHealth());
		}

		// Scene capture should change color saturation
		if (sceneCapture && !healthRegenerationComponent->IsDead())
		{
			// Change camera's color saturation based on player's current health
			sceneCapture->GetCaptureComponent2D()->PostProcessSettings.ColorSaturation = 
				FVector4(healthRegenerationComponent->GetCurrentHealth() / 
					healthRegenerationComponent->GetMaxHealth(), healthRegenerationComponent->GetCurrentHealth() /
					healthRegenerationComponent->GetMaxHealth(), healthRegenerationComponent->GetCurrentHealth() /
					healthRegenerationComponent->GetMaxHealth(), 1.0f);

			// Also override the color saturation
			sceneCapture->GetCaptureComponent2D()->PostProcessSettings.bOverride_ColorSaturation = true;
		}
	}
}

void APlayerCharacter::InitializeMobileInterface()
{
	// Only for developing the build on mobile devices
	if (gameTouchInterface && playerController)
	{
		playerController->ActivateTouchInterface(gameTouchInterface);
		canDragCamera = true;
		isTouchInterfaceEnabled = true;
	}
}

void APlayerCharacter::StartTouchInput(const FInputActionValue& Value)
{
	if (isTouchInterfaceEnabled && canDragCamera)
	{
		startTouchLocation = Value.Get<FVector2D>();
	}
}

void APlayerCharacter::MobileControlCamera(const FInputActionValue& Value)
{
	// Make sure the camera touch and drag only occurs during the press and drag camera is true
	if (isTouchInterfaceEnabled && canDragCamera)
	{
		// Calculate the delta touch location based on difference between current value and the start touch location
		FVector2D deltaTouchLocation = Value.Get<FVector2D>() - startTouchLocation;

		// Normalize delta touch location to prevent really fast swipe movement
		deltaTouchLocation.Normalize();

		// Move the camera based on swipe location
		AddControllerYawInput(deltaTouchLocation.X);
		AddControllerPitchInput(deltaTouchLocation.Y);
	}
}

// Functions here will be used to bind to the gameplay tag component delegates
void APlayerCharacter::ChangeGameplayTag(FName AddedTag, FName RemoveTag)
{
	gameplayTagComponent->RemoveActionTag(RemoveTag);
	gameplayTagComponent->AddActionTag(AddedTag);
}

void APlayerCharacter::AddToGameplayTag(FName AddedTag)
{
	gameplayTagComponent->AddActionTag(AddedTag);
}

void APlayerCharacter::RemoveGameplayTag(FName RemoveTag)
{
	gameplayTagComponent->RemoveActionTag(RemoveTag);
}