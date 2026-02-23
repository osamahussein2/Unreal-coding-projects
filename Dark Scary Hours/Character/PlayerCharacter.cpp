// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/EhGameJamGM.h"
#include "Widget/PlayerHUDWidget.h"
#include "Component/WeaponComponent.h"
#include "EhGameJam/EhGameJam.h"
#include "Character/NPC.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Widget/DialogueWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_PLAYER, ECR_Ignore);
	GetMesh()->SetCollisionObjectType(COLLISION_PLAYER);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetCapsuleComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Set its pawn control rotation to true
	springArm->bUsePawnControlRotation = true;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetupAttachment(springArm);

	minimapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArm"));
	minimapSpringArm->SetupAttachment(GetCapsuleComponent());

	sceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapSceneCapture"));
	sceneCaptureComponent->SetupAttachment(minimapSpringArm);

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
	spriteComponent->SetupAttachment(GetMesh());

	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (cameraComponent) normalCameraLocation = cameraComponent->GetRelativeLocation();

	if (spriteComponent)
	{
		spriteComponent->bVisibleInSceneCaptureOnly = true;
	}
	
	if (idleAnimation)
	{
		GetMesh()->SetAnimation(idleAnimation);
		GetMesh()->Play(true);
	}

	currentHealth = maxHealth;

	InitializePlayerHUD();

	for (TActorIterator<ANPC> NPCs(GetWorld()); NPCs; ++NPCs)
	{
		npc = *NPCs;
	}

	if (!weaponComponent)
	{
		// Create a new weapon component object
		weaponComponent = NewObject<UWeaponComponent>(this, UWeaponComponent::StaticClass(), TEXT("WeaponComponent"));

		// Register the created component during runtime
		weaponComponent->RegisterComponent();
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (currentArmor <= 0.0f)
	{
		currentHealth -= DamageAmount;

		if (playerHUD) playerHUD->SetHealthBar(currentHealth, maxHealth);
	}

	else
	{
		currentArmor -= DamageAmount;

		if (playerHUD) playerHUD->SetArmorBar(currentArmor, maxArmor);
	}

	if (currentHealth <= 0.0f)
	{
		DestroyHUD();

		if (AEhGameJamGM* gameMode = Cast<AEhGameJamGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			if (UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), false);

			gameMode->UnloadPlayingLevel();
			gameMode->EnableMouseCursor();
			gameMode->PushWidget(gameMode->GetMainMenuClass());
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Enemy taken damage: %f"), currentHealth);

	return DamageAmount;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::InitializePlayerHUD()
{
	if (playerHUDClass)
	{
		// Create player HUD
		playerHUD = CreateWidget<UPlayerHUDWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), playerHUDClass);
	}

	// Add player HUD to viewport only if it's valid
	if (playerHUD)
	{
		playerHUD->AddToViewport();
		playerHUD->SetHealthBar(currentHealth, maxHealth);
		playerHUD->SetArmorBar(currentArmor, maxArmor);

		UpdateWeaponHUD(); // Don't show the weapon overlay
		UpdateWeaponHUD(pistol, TEXT("/Game/ModelAssets/Weapons/Textures/PistolImage"));
		UpdateWeaponHUD(shotgun, TEXT("/Game/ModelAssets/Weapons/Textures/ShotgunImage"));
		UpdateWeaponHUD(rifle, TEXT("/Game/ModelAssets/Weapons/Textures/RifleImage"));
	}
}

void APlayerCharacter::DestroyHUD()
{
	if (playerHUD)
	{
		playerHUD->RemoveFromParent();
		playerHUD = nullptr;
	}
}

void APlayerCharacter::ApplyArmor()
{
	if (playerHUD)
	{
		currentArmor = maxArmor;

		playerHUD->SetArmorBar(currentArmor, maxArmor);
	}
}

void APlayerCharacter::OnCompleteQuest()
{
	if (npc && npc->GetNPCDialogueState() == NPCDialogueState::Given)
	{
		npc->SetNPCDialogueState(NPCDialogueState::Completed);
		npc->SetDialogueIndex(npc->DialogueGivenDone());
		npc->SpawnMarkerWidget();
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!inputMapping) return;

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>((Cast<APlayerController>(
			GetController())->GetLocalPlayer()));

	// Add the input mapping context
	enhancedInputSubsystem->AddMappingContext(inputMapping, 0);

	// Get the EnhancedInputComponent
	UEnhancedInputComponent* playerEnhancedInputcomponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (IA_Move)
	{
		playerEnhancedInputcomponent->BindAction(IA_Move, ETriggerEvent::Triggered, this,
			&APlayerCharacter::MoveCharacter);

		playerEnhancedInputcomponent->BindAction(IA_Move, ETriggerEvent::None, this,
			&APlayerCharacter::IdleCharacter);
	}

	if (IA_Camera)
	{
		playerEnhancedInputcomponent->BindAction(IA_Camera, ETriggerEvent::Triggered, this,
			&APlayerCharacter::MoveCamera);
	}

	if (IA_PauseGame)
	{
		playerEnhancedInputcomponent->BindAction(IA_PauseGame, ETriggerEvent::Triggered, this,
			&APlayerCharacter::PauseGame);
	}

	if (IA_FireWeapon)
	{
		playerEnhancedInputcomponent->BindAction(IA_FireWeapon, ETriggerEvent::Triggered, this,
			&APlayerCharacter::StartFiring);

		playerEnhancedInputcomponent->BindAction(IA_FireWeapon, ETriggerEvent::Canceled, this,
			&APlayerCharacter::CancelWeaponFire);
	}

	if (IA_ReloadWeapon)
	{
		playerEnhancedInputcomponent->BindAction(IA_ReloadWeapon, ETriggerEvent::Started, this,
			&APlayerCharacter::ReloadWeapon);
	}

	if (IA_PreviousWeapon)
	{
		playerEnhancedInputcomponent->BindAction(IA_PreviousWeapon, ETriggerEvent::Started, this,
			&APlayerCharacter::SwitchToPreviousWeapon);
	}

	if (IA_NextWeapon)
	{
		playerEnhancedInputcomponent->BindAction(IA_NextWeapon, ETriggerEvent::Started, this,
			&APlayerCharacter::SwitchToNextWeapon);
	}

	if (IA_Aiming)
	{
		playerEnhancedInputcomponent->BindAction(IA_Aiming, ETriggerEvent::Started, this,
			&APlayerCharacter::TriggerLockOn);

		playerEnhancedInputcomponent->BindAction(IA_Aiming, ETriggerEvent::Triggered, this,
			&APlayerCharacter::CheckForEnemies);

		playerEnhancedInputcomponent->BindAction(IA_Aiming, ETriggerEvent::None, this,
			&APlayerCharacter::StopLockOn);
	}

	if (IA_TargetLeft)
	{
		playerEnhancedInputcomponent->BindAction(IA_TargetLeft, ETriggerEvent::Started, this,
			&APlayerCharacter::SwitchToLeftTarget);
	}

	if (IA_TargetRight)
	{
		playerEnhancedInputcomponent->BindAction(IA_TargetRight, ETriggerEvent::Started, this,
			&APlayerCharacter::SwitchToRightTarget);
	}

	if (IA_PickUpWeapon)
	{
		playerEnhancedInputcomponent->BindAction(IA_PickUpWeapon, ETriggerEvent::Started, this,
			&APlayerCharacter::PickUpWeapon);
	}

	if (IA_TriggerDialogue)
	{
		playerEnhancedInputcomponent->BindAction(IA_TriggerDialogue, ETriggerEvent::Started, this,
			&APlayerCharacter::TriggerDialogue);
	}

	if (IA_NextDialogue)
	{
		playerEnhancedInputcomponent->BindAction(IA_NextDialogue, ETriggerEvent::Started, this,
			&APlayerCharacter::ProceedToNextDialogue);
	}

	if (IA_RefillAmmo)
	{
		playerEnhancedInputcomponent->BindAction(IA_RefillAmmo, ETriggerEvent::Started, this,
			&APlayerCharacter::GrabAmmo);
	}
}

void APlayerCharacter::MoveCharacter(const FInputActionValue& value_)
{
	if (HasActionTag("Talking")) return;

	AddMovementInput(GetActorRightVector(), value_.Get<FVector2D>().X);
	AddMovementInput(GetActorForwardVector(), value_.Get<FVector2D>().Y);

	const FRotator& controlRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, Controller->GetControlRotation().Roll);
	const FRotator& rotationDirection = (controlRotation - GetActorRotation()).GetNormalized() * GetWorld()->GetDeltaSeconds() * controlRotationSpeed;

	AddActorWorldRotation(rotationDirection);

	if (walkingAnimation && locomotionState != ELocomotionState::EWalking)
	{
		GetMesh()->SetAnimation(walkingAnimation);
		GetMesh()->Play(true);

		locomotionState = ELocomotionState::EWalking;
	}

	if (weaponComponent->IsHoldingWeapon(pistol) && weaponComponent->IsWeaponValid(pistol)->IsAttachedTo(this))
	{
		weaponComponent->AttachWeaponToSocket(this, pistol, FName("hand_rSocket"));
	}

	if (weaponComponent->IsHoldingWeapon(shotgun) && weaponComponent->IsWeaponValid(shotgun)->IsAttachedTo(this))
	{
		weaponComponent->AttachWeaponToSocket(this, shotgun, FName("hand_rSocket"));
	}

	if (weaponComponent->IsHoldingWeapon(rifle) && weaponComponent->IsWeaponValid(rifle)->IsAttachedTo(this))
	{
		weaponComponent->AttachWeaponToSocket(this, rifle, FName("hand_rSocket"));
	}
}

void APlayerCharacter::IdleCharacter()
{
	if (idleAnimation && locomotionState != ELocomotionState::EIdle)
	{
		GetMesh()->SetAnimation(idleAnimation);
		GetMesh()->Play(true);

		locomotionState = ELocomotionState::EIdle;
	}

	if (weaponComponent->IsHoldingWeapon(pistol) && weaponComponent->IsWeaponValid(pistol)->IsAttachedTo(this))
	{
		weaponComponent->AttachWeaponToSocket(this, pistol, FName("hand_rSocket"));
	}

	if (weaponComponent->IsHoldingWeapon(shotgun) && weaponComponent->IsWeaponValid(shotgun)->IsAttachedTo(this))
	{
		weaponComponent->AttachWeaponToSocket(this, shotgun, FName("hand_rSocket"));
	}

	if (weaponComponent->IsHoldingWeapon(rifle) && weaponComponent->IsWeaponValid(rifle)->IsAttachedTo(this))
	{
		weaponComponent->AttachWeaponToSocket(this, rifle, FName("hand_rSocket"));
	}
}

void APlayerCharacter::MoveCamera(const FInputActionValue& value_)
{
	// Fixes the spinning balls effect by checking if the game isn't actually paused
	if (!UGameplayStatics::IsGamePaused(GetWorld()) && !HasActionTag("Talking"))
	{
		AddControllerYawInput(value_.Get<FVector2D>().X);
		AddControllerPitchInput(value_.Get<FVector2D>().Y);

		RotateWeaponUsingTag(pistol);
		RotateWeaponUsingTag(shotgun);
		RotateWeaponUsingTag(rifle);
	}
}

void APlayerCharacter::TriggerDialogue()
{
	if (npc->GetPlayerTriggerFound() && !HasActionTag("Talking"))
	{
		if (!dialogueUI)
		{
			dialogueUI = CreateWidget<UDialogueWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), dialogueClass);
			if (!dialogueUI) return;

			DestroyHUD();

			dialogueUI->GoToNextDialogue(npc->GetNPCName(), npc->GetDialogueText(), npc->GetTypingSpeed());
			dialogueUI->AddToViewport();

			npc->DestroyMarkerWidget();

			AddActionTag("Talking");
		}
	}
}

void APlayerCharacter::PauseGame()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()) && !HasActionTag("Aiming") && !HasActionTag("Talking"))
	{
		DestroyHUD();

		if (AEhGameJamGM* gameMode = Cast<AEhGameJamGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->EnableMouseCursor();
			gameMode->PushWidget(gameMode->GetPauseMenuClass());
		}
	}
}

void APlayerCharacter::PickUpWeapon()
{
	if (HasActionTag("Talking") || HasActionTag("Aiming")) return;

	if (weaponComponent)
	{
		if (playerHUD && weaponComponent->IsHoldingWeapon(pistol))
		{
			if (!weaponComponent->IsWeaponValid(pistol)->IsAttachedTo(this))
			{
				// Hide the other weapons upon picking up the pistol if the other weapons were picked up earlier
				HideWeapon(shotgun);

				weaponIndex = 1; // Set it to pistol index

				//playerHUD->InitializeWeaponAmmoText();
				//playerHUD->InitializeWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/PistolImage")));

				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/PistolImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(pistol)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(pistol)->GetTotalAmmo());
			}

			weaponComponent->PickUpWeapon(this, pistol, FName("hand_rSocket"));
			weaponComponent->AttachWeaponToSocket(this, pistol, FName("hand_rSocket"));
		}

		if (playerHUD && weaponComponent->IsHoldingWeapon(shotgun))
		{
			if (!weaponComponent->IsWeaponValid(shotgun)->IsAttachedTo(this))
			{
				// Hide the other weapons upon picking up the shotgun if the other weapons were picked up earlier
				HideWeapon(pistol);

				weaponIndex = 2; // Set it to shotgun index

				//playerHUD->InitializeWeaponAmmoText();
				//playerHUD->InitializeWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/PistolImage")));

				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/ShotgunImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(shotgun)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(shotgun)->GetTotalAmmo());

				weaponComponent->PickUpWeapon(this, shotgun, FName("hand_rSocket"));
				weaponComponent->AttachWeaponToSocket(this, shotgun, FName("hand_rSocket"));
			}
		}

		if (playerHUD && weaponComponent->IsHoldingWeapon(rifle))
		{
			if (!weaponComponent->IsWeaponValid(rifle)->IsAttachedTo(this))
			{
				// Hide the other weapons upon picking up the rifle if the other weapons were picked up earlier
				HideWeapon(pistol);
				HideWeapon(shotgun);

				weaponIndex = 3; // Set it to rifle index

				//playerHUD->InitializeWeaponAmmoText();
				//playerHUD->InitializeWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/PistolImage")));

				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/RifleImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(rifle)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(rifle)->GetTotalAmmo());

				weaponComponent->PickUpWeapon(this, rifle, FName("hand_rSocket"));
				weaponComponent->AttachWeaponToSocket(this, rifle, FName("hand_rSocket"));
			}
		}
	}
}

void APlayerCharacter::GrabAmmo()
{
	if (HasActionTag("Picking Up Ammo"))
	{
		if (weaponComponent->IsWeaponValid(pistol) &&
			!weaponComponent->IsWeaponValid(pistol)->IsHidden() &&
			weaponComponent->IsWeaponValid(pistol)->IsAttachedTo(this))
		{
			weaponComponent->IsWeaponValid(pistol)->RefillAmmo();

			playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(pistol)->GetClipsInMagazine(),
				weaponComponent->IsWeaponValid(pistol)->GetTotalAmmo());
		}

		else if (weaponComponent->IsWeaponValid(shotgun) &&
			!weaponComponent->IsWeaponValid(shotgun)->IsHidden() &&
			weaponComponent->IsWeaponValid(shotgun)->IsAttachedTo(this))
		{
			weaponComponent->IsWeaponValid(shotgun)->RefillAmmo();

			playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(shotgun)->GetClipsInMagazine(),
				weaponComponent->IsWeaponValid(shotgun)->GetTotalAmmo());
		}

		else if (weaponComponent->IsWeaponValid(rifle) &&
			!weaponComponent->IsWeaponValid(rifle)->IsHidden() &&
			weaponComponent->IsWeaponValid(rifle)->IsAttachedTo(this))
		{
			weaponComponent->IsWeaponValid(rifle)->RefillAmmo();

			playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(rifle)->GetClipsInMagazine(),
				weaponComponent->IsWeaponValid(rifle)->GetTotalAmmo());
		}
	}
}

void APlayerCharacter::StartFiring()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(timerHandle) && !HasActionTag("Talking"))
	{
		if (canFireImmediately) // Start firing the weapon right away
		{
			FireWeapon();
		}

		else // Otherwise, use set timer method to create a fire delay until weapon can fire again
		{
			// Pistol is being held and not hidden
			TriggerWeaponFireTimerHandle(pistol);

			// Shotgun is being held and not hidden
			TriggerWeaponFireTimerHandle(shotgun);

			// Rifle is being held and not hidden
			TriggerWeaponFireTimerHandle(rifle);
		}
	}
}

void APlayerCharacter::SwitchToPreviousWeapon()
{
	// If weapon index is greater than 0, go to the previous weapon
	if (weaponIndex > 0 && !HasActionTag("Aiming"))
	{
		--weaponIndex;
		GoToPreviousWeaponBasedOnIndex();
	}
}

void APlayerCharacter::SwitchToNextWeapon()
{
	// If the player currently isn't talking to one of the NPCs nor aiming, check if they can select the next weapon
	if (!HasActionTag("Aiming") && !HasActionTag("Talking"))
	{
		GoToNextWeaponBasedOnIndex();
	}
}

void APlayerCharacter::FireWeapon()
{
	if (HasActionTag("Talking")) return;

	// Pistol check if statement
	FireWeaponUsingTag(pistol);

	// Shotgun check if statement
	FireWeaponUsingTag(shotgun);

	// Rifle check if statement
	FireWeaponUsingTag(rifle);
}

void APlayerCharacter::CancelWeaponFire()
{
	// Clear the weapon fire timer handle if it's still active
	if (GetWorld()->GetTimerManager().IsTimerActive(timerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	}
}

void APlayerCharacter::ReloadWeapon()
{
	if (HasActionTag("Talking")) return;

	ReloadWeaponUsingTag(pistol);
	ReloadWeaponUsingTag(shotgun);
	ReloadWeaponUsingTag(rifle);
}

void APlayerCharacter::TriggerLockOn()
{
	if (HasActionTag("Talking")) return;

	TriggerLockOnUsingTag(pistol);
	TriggerLockOnUsingTag(shotgun);
	TriggerLockOnUsingTag(rifle);
}

void APlayerCharacter::SwitchToLeftTarget()
{
	if (HasActionTag("Talking")) return;

	SwitchToLeftTargetUsingTag(pistol);
	SwitchToLeftTargetUsingTag(shotgun);
	SwitchToLeftTargetUsingTag(rifle);
}

void APlayerCharacter::SwitchToRightTarget()
{
	if (HasActionTag("Talking")) return;

	SwitchToRightTargetUsingTag(pistol);
	SwitchToRightTargetUsingTag(shotgun);
	SwitchToRightTargetUsingTag(rifle);
}

void APlayerCharacter::StopLockOn()
{
	if (weaponComponent && weaponComponent->GetLockedOnTarget())
	{
		weaponComponent->StopDetectingEnemies();

		if (cameraComponent->GetRelativeLocation() != normalCameraLocation)
			cameraComponent->SetRelativeLocation(normalCameraLocation);

		if (cameraComponent->GetRelativeRotation() != FRotator::ZeroRotator)
			cameraComponent->SetRelativeRotation(FRotator::ZeroRotator);

		if (weaponComponent->IsWeaponValid(pistol) &&
			!weaponComponent->IsWeaponValid(pistol)->IsHidden() &&
			weaponComponent->IsWeaponValid(pistol)->IsAttachedTo(this))
		{
			weaponComponent->ResetWeaponRotation(this, pistol);
		}

		if (weaponComponent->IsWeaponValid(shotgun) &&
			!weaponComponent->IsWeaponValid(shotgun)->IsHidden() &&
			weaponComponent->IsWeaponValid(shotgun)->IsAttachedTo(this))
		{
			weaponComponent->ResetWeaponRotation(this, shotgun);
		}

		if (weaponComponent->IsWeaponValid(rifle) &&
			!weaponComponent->IsWeaponValid(rifle)->IsHidden() &&
			weaponComponent->IsWeaponValid(rifle)->IsAttachedTo(this))
		{
			weaponComponent->ResetWeaponRotation(this, rifle);
		}

		RemoveActionTag("Aiming");
	}
}

void APlayerCharacter::ProceedToNextDialogue()
{
	if (HasActionTag("Talking"))
	{
		NPCNotGivenDialogue();
		NPCGivenDialogue();
		NPCCompletedDialogue();
	}
}

void APlayerCharacter::TriggerWeaponFireTimerHandle(FName tagName_)
{
	if (weaponComponent && weaponComponent->IsWeaponValid(tagName_) &&
		weaponComponent->IsHoldingWeapon(tagName_) && !weaponComponent->IsWeaponValid(tagName_)->IsHidden() &&
		weaponComponent->IsWeaponValid(tagName_)->IsAttachedTo(this))
	{
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &APlayerCharacter::FireWeapon,
			1.0f / weaponComponent->IsWeaponValid(tagName_)->GetFiringSpeed(), false);
	}
}

void APlayerCharacter::TriggerLockOnUsingTag(FName tagName_)
{
	if (weaponComponent && weaponComponent->IsHoldingWeapon(tagName_) && weaponComponent->IsWeaponValid(tagName_) &&
		!weaponComponent->IsWeaponValid(tagName_)->IsHidden() && weaponComponent->IsWeaponValid(tagName_)->IsAttachedTo(this))
	{
		weaponComponent->DetectEnemies(cameraComponent, tagName_);

		if (weaponComponent->GetLockedOnTarget())
		{
			if (cameraComponent->GetRelativeLocation() != lockedOnCameraLocation)
				cameraComponent->SetRelativeLocation(lockedOnCameraLocation);

			weaponComponent->RotateWeaponAtTarget(this, tagName_);

			AddActionTag("Aiming");
		}
	}
}

void APlayerCharacter::CheckForEnemies()
{
	if (weaponComponent && !weaponComponent->GetLockedOnTarget() && HasActionTag("Aiming"))
	{
		if (cameraComponent->GetRelativeLocation() != normalCameraLocation)
			cameraComponent->SetRelativeLocation(normalCameraLocation);

		if (cameraComponent->GetRelativeRotation() != FRotator::ZeroRotator)
			cameraComponent->SetRelativeRotation(FRotator::ZeroRotator);

		if (weaponComponent->IsWeaponValid(pistol) &&
			!weaponComponent->IsWeaponValid(pistol)->IsHidden() &&
			weaponComponent->IsWeaponValid(pistol)->IsAttachedTo(this))
		{
			weaponComponent->ResetWeaponRotation(this, pistol);
		}

		if (weaponComponent->IsWeaponValid(shotgun) &&
			!weaponComponent->IsWeaponValid(shotgun)->IsHidden() &&
			weaponComponent->IsWeaponValid(shotgun)->IsAttachedTo(this))
		{
			weaponComponent->ResetWeaponRotation(this, shotgun);
		}

		if (weaponComponent->IsWeaponValid(rifle) &&
			!weaponComponent->IsWeaponValid(rifle)->IsHidden() &&
			weaponComponent->IsWeaponValid(rifle)->IsAttachedTo(this))
		{
			weaponComponent->ResetWeaponRotation(this, rifle);
		}

		RemoveActionTag("Aiming");
	}

	
}

void APlayerCharacter::SwitchToLeftTargetUsingTag(FName tagName_)
{
	if (weaponComponent && weaponComponent->IsHoldingWeapon(tagName_) && weaponComponent->IsWeaponValid(tagName_) &&
		!weaponComponent->IsWeaponValid(tagName_)->IsHidden() && weaponComponent->IsWeaponValid(tagName_)->IsAttachedTo(this))
	{
		weaponComponent->DetectEnemiesFromLeftSide(cameraComponent, tagName_);
		weaponComponent->RotateWeaponAtTarget(this, tagName_);
	}
}

void APlayerCharacter::SwitchToRightTargetUsingTag(FName tagName_)
{
	if (weaponComponent && weaponComponent->IsHoldingWeapon(tagName_) && weaponComponent->IsWeaponValid(tagName_) &&
		!weaponComponent->IsWeaponValid(tagName_)->IsHidden() && weaponComponent->IsWeaponValid(tagName_)->IsAttachedTo(this))
	{
		weaponComponent->DetectEnemiesFromRightSide(cameraComponent, tagName_);
		weaponComponent->RotateWeaponAtTarget(this, tagName_);
	}
}

void APlayerCharacter::RotateWeaponUsingTag(FName tagName_)
{
	if (weaponComponent && weaponComponent->IsHoldingWeapon(tagName_) && !weaponComponent->IsWeaponValid(tagName_)->IsHidden()
		&& !weaponComponent->GetLockedOnEnemy())
	{
		weaponComponent->RotateWeaponWhenHeld(this, tagName_, FName("hand_rSocket"));
	}
}

void APlayerCharacter::FireWeaponUsingTag(FName tagName_)
{
	if (weaponComponent && weaponComponent->IsHoldingWeapon(tagName_) && weaponComponent->IsWeaponValid(tagName_) &&
		!weaponComponent->IsWeaponValid(tagName_)->IsHidden() && weaponComponent->IsWeaponValid(tagName_)->IsAttachedTo(this))
	{
		weaponComponent->IsWeaponValid(tagName_)->FireWeapon(weaponComponent->IsWeaponValid(tagName_)->GetDestinationLocation());

		if (playerHUD) // Update the weapon counter HUD
		{
			playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(tagName_)->GetClipsInMagazine(),
				weaponComponent->IsWeaponValid(tagName_)->GetTotalAmmo());
		}

		if (canFireImmediately) canFireImmediately = false; // Set this to false to start fire delay if necessary

		// Clear the timer to allow player to re-fire the weapon
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	}
}

void APlayerCharacter::ReloadWeaponUsingTag(FName tagName_)
{
	if (weaponComponent && weaponComponent->IsHoldingWeapon(tagName_) && !weaponComponent->IsWeaponValid(tagName_)->IsHidden())
	{
		weaponComponent->IsWeaponValid(tagName_)->ReloadWeapon(); // Reload the weapon

		if (playerHUD) // Update the weapon counter HUD
		{
			playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(tagName_)->GetClipsInMagazine(),
				weaponComponent->IsWeaponValid(tagName_)->GetTotalAmmo());
		}
	}
}

void APlayerCharacter::ShowWeapon(FName tagName_)
{
	if (weaponComponent->IsWeaponValid(tagName_) && weaponComponent->IsWeaponValid(tagName_)->IsHidden() &&
		weaponComponent->IsWeaponValid(tagName_)->IsAttachedTo(this))
	{
		weaponComponent->IsWeaponValid(tagName_)->SetActorHiddenInGame(false);
	}
}

void APlayerCharacter::HideWeapon(FName tagName_)
{
	if (weaponComponent->IsWeaponValid(tagName_) && !weaponComponent->IsWeaponValid(tagName_)->IsHidden() &&
		weaponComponent->IsWeaponValid(tagName_)->IsAttachedTo(this))
	{
		weaponComponent->IsWeaponValid(tagName_)->SetActorHiddenInGame(true);
	}
}

void APlayerCharacter::UpdateWeaponHUD()
{
	// If the player is on the first weapon index
	if (weaponIndex == 0)
	{
		// Set the weapon HUD to show fist image and empty weapon ammo text
		playerHUD->CollapseWeaponOverlay();
		playerHUD->SetWeaponAmmoText();
	}
}

void APlayerCharacter::UpdateWeaponHUD(FName tagName_, FString textureFile_)
{
	if (weaponComponent && weaponComponent->IsHoldingWeapon(tagName_) && !weaponComponent->IsWeaponValid(tagName_)->IsHidden())
	{
		playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, textureFile_));

		playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(tagName_)->GetClipsInMagazine(),
			weaponComponent->IsWeaponValid(tagName_)->GetTotalAmmo());
	}
}

void APlayerCharacter::GoToNextWeaponBasedOnIndex()
{
	switch (weaponIndex)
	{
	case 0: // Fist
		if (weaponComponent->IsWeaponValid(pistol) && weaponComponent->IsWeaponValid(pistol)->IsAttachedTo(this))
		{
			++weaponIndex;

			ShowWeapon(pistol); // Make the pistol visible

			RotateWeaponUsingTag(pistol);

			// Update the pistol counter HUD
			if (playerHUD)
			{
				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/PistolImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(pistol)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(pistol)->GetTotalAmmo());
			}

			break;
		}

		if (weaponComponent->IsWeaponValid(shotgun) && weaponComponent->IsWeaponValid(shotgun)->IsAttachedTo(this))
		{
			weaponIndex += 2;

			HideWeapon(pistol); // Make pistol invisible
			ShowWeapon(shotgun); // Make shotgun visible

			RotateWeaponUsingTag(shotgun);

			// Update the shotgun counter HUD
			if (playerHUD)
			{
				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/ShotgunImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(shotgun)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(shotgun)->GetTotalAmmo());
			}

			break;
		}

		if (weaponComponent->IsWeaponValid(rifle) && weaponComponent->IsWeaponValid(rifle)->IsAttachedTo(this))
		{
			weaponIndex += 3;

			HideWeapon(pistol); // Make pistol invisible
			HideWeapon(shotgun);
			ShowWeapon(rifle); // Make shotgun visible

			RotateWeaponUsingTag(rifle);

			// Update the shotgun counter HUD
			if (playerHUD)
			{
				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/RifleImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(rifle)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(rifle)->GetTotalAmmo());
			}

			break;
		}
	case 1: // Pistol

		if (weaponComponent->IsWeaponValid(shotgun) && weaponComponent->IsWeaponValid(shotgun)->IsAttachedTo(this))
		{
			++weaponIndex;

			HideWeapon(pistol); // Make pistol invisible
			ShowWeapon(shotgun); // Make shotgun visible

			RotateWeaponUsingTag(shotgun);

			// Update the shotgun counter HUD
			if (playerHUD)
			{
				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/ShotgunImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(shotgun)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(shotgun)->GetTotalAmmo());
			}

			break;
		}

		if (weaponComponent->IsWeaponValid(rifle) && weaponComponent->IsWeaponValid(rifle)->IsAttachedTo(this) &&
			!weaponComponent->IsWeaponValid(shotgun)->IsAttachedTo(this))
		{
			weaponIndex += 2;

			HideWeapon(pistol); // Make pistol invisible
			HideWeapon(shotgun);
			ShowWeapon(rifle); // Make shotgun visible

			RotateWeaponUsingTag(rifle);

			// Update the shotgun counter HUD
			if (playerHUD)
			{
				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/RifleImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(rifle)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(rifle)->GetTotalAmmo());
			}

			break;
		}

	case 2: // Shotgun

		if (weaponComponent->IsWeaponValid(rifle) && weaponComponent->IsWeaponValid(rifle)->IsAttachedTo(this))
		{
			HideWeapon(pistol); // Make pistol invisible
			HideWeapon(shotgun);
			ShowWeapon(rifle); // Make shotgun visible

			RotateWeaponUsingTag(rifle);

			// Update the shotgun counter HUD
			if (playerHUD)
			{
				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/RifleImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(rifle)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(rifle)->GetTotalAmmo());
			}

			++weaponIndex;
		}

		break;

	case 3: // Rifle
		break;

	default:
		weaponIndex = 3;
		break;
	}
}

void APlayerCharacter::GoToPreviousWeaponBasedOnIndex()
{
	switch (weaponIndex)
	{
	case 0: // Fist
		HideWeapon(pistol); // Hide pistol
		HideWeapon(shotgun); // Hide shotgun
		HideWeapon(rifle);

		// Update the weapon counter HUD to show fist and no ammo text
		if (playerHUD)
		{
			playerHUD->CollapseWeaponOverlay();
			playerHUD->SetWeaponAmmoText();
		}

		break;

	case 1: // Pistol

		// If the player is holding pistol
		if (weaponComponent->IsWeaponValid(pistol) && weaponComponent->IsWeaponValid(pistol)->IsAttachedTo(this))
		{
			HideWeapon(shotgun); // Hide shotgun
			HideWeapon(rifle);
			ShowWeapon(pistol); // Make pistol visible

			RotateWeaponUsingTag(pistol);

			// Update the pistol counter HUD
			if (playerHUD)
			{
				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/PistolImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(pistol)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(pistol)->GetTotalAmmo());
			}
		}

		else
		{
			--weaponIndex;
			GoToPreviousWeaponBasedOnIndex();
		}

		break;

	case 2: // Shotgun

		// If the player is holding shotgun
		if (weaponComponent->IsWeaponValid(shotgun) && weaponComponent->IsWeaponValid(shotgun)->IsAttachedTo(this))
		{
			HideWeapon(pistol); // Hide pistol
			HideWeapon(rifle); // Hide rifle
			ShowWeapon(shotgun); // Make shotgun visible

			RotateWeaponUsingTag(shotgun);

			// Update the shotgun counter HUD
			if (playerHUD)
			{
				playerHUD->SetWeaponImage(LoadObject<UTexture2D>(this, TEXT("/Game/ModelAssets/Weapons/Textures/ShotgunImage")));

				playerHUD->SetWeaponAmmoText(weaponComponent->IsWeaponValid(shotgun)->GetClipsInMagazine(),
					weaponComponent->IsWeaponValid(shotgun)->GetTotalAmmo());
			}
		}

		else
		{
			--weaponIndex;
			GoToPreviousWeaponBasedOnIndex();
		}

		break;

	default:
		weaponIndex = 0;
		break;
	}
}

// NPC
void APlayerCharacter::NPCNotGivenDialogue()
{
	// If the player is on the NPC's box trigger and that NPC's dialogue is less than their total dialogue
			// If NPC hasn't done giving dialogue to the player
	if (npc->GetPlayerTriggerFound() && npc->GetNPCDialogueState() == NPCDialogueState::NotGiven &&
		npc->GetDialogueIndex() < npc->DialogueNotGivenDone() - 1)
	{
		if (dialogueUI && !dialogueUI->IsTyping())
		{
			// Go to the next dialogue
			npc->GoToNextDialogue(1);

			// And update any dialogue and NPC name texts
			dialogueUI->GoToNextDialogue(npc->GetNPCName(), npc->GetDialogueText(),
				npc->GetTypingSpeed());
		}

		else if (dialogueUI && dialogueUI->IsTyping())
		{
			dialogueUI->SkipTyping();
		}
	}

	else if (npc->GetPlayerTriggerFound() && npc->GetNPCDialogueState() == NPCDialogueState::NotGiven
		&& npc->GetDialogueIndex() >= npc->DialogueNotGivenDone() - 1)
	{
		if (dialogueUI && !dialogueUI->IsTyping())
		{
			dialogueUI->RemoveFromParent();
			dialogueUI = nullptr;

			InitializePlayerHUD();

			if (weaponComponent && !weaponComponent->AreAllEnemiesKilled())
			{
				npc->SetNPCDialogueState(NPCDialogueState::Given);

				// Go to the next dialogue
				npc->GoToNextDialogue(1);
			}

			else if (weaponComponent && weaponComponent->AreAllEnemiesKilled())
			{
				npc->SetNPCDialogueState(NPCDialogueState::Completed);

				// Go to completed quest dialogue index
				npc->SetDialogueIndex(npc->DialogueGivenDone());
			}

			RemoveActionTag("Talking");
		}

		else if (dialogueUI && dialogueUI->IsTyping())
		{
			dialogueUI->SkipTyping();
		}
	}
}

void APlayerCharacter::NPCGivenDialogue()
{
	// If the player is on the NPC's box trigger and that NPC's dialogue is less than their total dialogue
			// If NPC hasn't done giving dialogue to the player
	if (npc->GetPlayerTriggerFound() && npc->GetNPCDialogueState() == NPCDialogueState::Given &&
		npc->GetDialogueIndex() < npc->DialogueGivenDone() - 1)
	{
		if (dialogueUI && !dialogueUI->IsTyping())
		{
			// Go to the next dialogue
			npc->GoToNextDialogue(1);

			// And update any dialogue and NPC name texts
			dialogueUI->GoToNextDialogue(npc->GetNPCName(), npc->GetDialogueText(),
				npc->GetTypingSpeed());
		}

		else if (dialogueUI && dialogueUI->IsTyping())
		{
			dialogueUI->SkipTyping();
		}
	}

	else if (npc->GetPlayerTriggerFound() && npc->GetNPCDialogueState() == NPCDialogueState::Given
		&& npc->GetDialogueIndex() >= npc->DialogueGivenDone() - 1)
	{
		if (dialogueUI && !dialogueUI->IsTyping())
		{
			//npc->SetNPCDialogueState(NPCDialogueState::Completed);
			npc->SetDialogueIndex(npc->DialogueNotGivenDone());

			RemoveActionTag("Talking");

			dialogueUI->RemoveFromParent();
			dialogueUI = nullptr;

			InitializePlayerHUD();
		}

		else if (dialogueUI && dialogueUI->IsTyping())
		{
			dialogueUI->SkipTyping();
		}
	}
}

void APlayerCharacter::NPCCompletedDialogue()
{
	// If the player is on the NPC's box trigger and that NPC's dialogue is less than their total dialogue
	// If player has completed dialogue with the NPC
	if (npc->GetPlayerTriggerFound() && npc->GetNPCDialogueState() == NPCDialogueState::Completed &&
		npc->GetDialogueIndex() >= npc->DialogueGivenDone() &&
		npc->GetDialogueIndex() < npc->GetDialogueLinesCount() - 1)
	{
		if (dialogueUI && !dialogueUI->IsTyping())
		{
			// Go to the next dialogue
			npc->GoToNextDialogue(1);

			dialogueUI->GoToNextDialogue(npc->GetNPCName(), npc->GetDialogueText(),
				npc->GetTypingSpeed());
		}

		else if (dialogueUI && dialogueUI->IsTyping())
		{
			dialogueUI->SkipTyping();
		}
	}

	else if (npc->GetPlayerTriggerFound() && npc->GetNPCDialogueState() == NPCDialogueState::Completed
		&& npc->GetDialogueIndex() >= npc->DialogueGivenDone() &&
		npc->GetDialogueIndex() >= npc->GetDialogueLinesCount() - 1)
	{
		if (dialogueUI && !dialogueUI->IsTyping())
		{
			dialogueUI->RemoveFromParent();
			dialogueUI = nullptr;

			npc->SetDialogueIndex(npc->DialogueGivenDone());

			npc->SetNPCDialogueState(NPCDialogueState::Finished);

			InitializePlayerHUD();

			RemoveActionTag("Talking");
		}

		else if (dialogueUI && dialogueUI->IsTyping())
		{
			dialogueUI->SkipTyping();
		}
	}

	// If the player is on the NPC's box trigger and that NPC's dialogue is less than their total dialogue
	// If player has finished all dialogues with the NPC
	else if (npc->GetPlayerTriggerFound() && npc->GetNPCDialogueState() == NPCDialogueState::Finished &&
		npc->GetDialogueIndex() >= npc->DialogueGivenDone() &&
		npc->GetDialogueIndex() < npc->GetDialogueLinesCount() - 1)
	{
		if (dialogueUI && !dialogueUI->IsTyping())
		{
			// Go to the next dialogue
			npc->GoToNextDialogue(1);

			dialogueUI->GoToNextDialogue(npc->GetNPCName(), npc->GetDialogueText(),
				npc->GetTypingSpeed());
		}

		else if (dialogueUI && dialogueUI->IsTyping())
		{
			dialogueUI->SkipTyping();
		}
	}

	else if (npc->GetPlayerTriggerFound() && npc->GetNPCDialogueState() == NPCDialogueState::Finished
		&& npc->GetDialogueIndex() >= npc->DialogueGivenDone() &&
		npc->GetDialogueIndex() >= npc->GetDialogueLinesCount() - 1)
	{
		if (dialogueUI && !dialogueUI->IsTyping())
		{
			dialogueUI->RemoveFromParent();
			dialogueUI = nullptr;

			npc->SetDialogueIndex(npc->DialogueGivenDone());

			npc->SetNPCDialogueState(NPCDialogueState::Finished);

			InitializePlayerHUD();

			RemoveActionTag("Talking");
		}

		else if (dialogueUI && dialogueUI->IsTyping())
		{
			dialogueUI->SkipTyping();
		}
	}
}

void APlayerCharacter::PlayerIsInAmmoCrateTrigger()
{
	AddActionTag("Picking Up Ammo");
}

void APlayerCharacter::PlayerIsOutOfAmmoCrateTrigger()
{
	RemoveActionTag("Picking Up Ammo");
}

void APlayerCharacter::RemoveActionTag(FString tagName_)
{
	if (HasActionTag(tagName_)) PlayerActions.Remove(tagName_);
}

void APlayerCharacter::AddActionTag(FString tagName_)
{
	if (!HasActionTag(tagName_)) PlayerActions.Emplace(tagName_);
}

bool APlayerCharacter::HasActionTag(FString tagName_)
{
	return PlayerActions.Contains(tagName_);
}