// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "InputMappingContext.h"
#include "Player/PlayerInputDA.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/BrackeysGM.h"
#include "Character/InteractableFish.h"
#include "EngineUtils.h"
#include "Engine/World.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetCapsuleComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Set its pawn control rotation to true
	springArm->bUsePawnControlRotation = true;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetupAttachment(springArm);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Start playing the idle animation
	if (idleAnimation)
	{
		GetMesh()->SetAnimation(idleAnimation);
		GetMesh()->Play(true);
	}
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlayerCharacter::OnPlayerBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddUniqueDynamic(this, &APlayerCharacter::OnPlayerEndOverlap);

	//GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(this, &APlayerCharacter::OnPlayerHit);

	if (!actionComponent)
	{
		// Create a new action component object
		actionComponent = NewObject<UActionComponent>(this, UActionComponent::StaticClass(), TEXT("ActionComponent"));

		// Register the created component during runtime
		actionComponent->RegisterComponent();

		if (actionComponent->OnAddAction.IsBound()) actionComponent->OnAddAction.Broadcast(freeRoam);
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
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_Move(), ETriggerEvent::Triggered, this,
				&APlayerCharacter::MoveCharacter);

			EnhancedInputComponent->BindAction(playerInputDA->GetIA_Move(), ETriggerEvent::Completed, this,
				&APlayerCharacter::PlayIdleAnimation);
		}

		if (playerInputDA && playerInputDA->GetIA_Look())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_Look(), ETriggerEvent::Triggered, this,
				&APlayerCharacter::MoveCamera);
		}

		if (playerInputDA && playerInputDA->GetIA_InteractWithFish())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_InteractWithFish(), ETriggerEvent::Started, this,
				&APlayerCharacter::InteractWithFish);
		}

		if (playerInputDA && playerInputDA->GetIA_Pause())
		{
			EnhancedInputComponent->BindAction(playerInputDA->GetIA_Pause(), ETriggerEvent::Started, this,
				&APlayerCharacter::PauseGame);
		}
	}
}

void APlayerCharacter::MoveCharacter(const FInputActionValue& value_)
{
	if (!actionComponent || !actionComponent->HasAction(freeRoam)) return;

	FVector2D InputDirection = value_.Get<FVector2D>();

	if (!InputDirection.IsNearlyZero())
	{
		UpdateWalkingAnimations(InputDirection);

		FVector RightDirection = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y);

		AddMovementInput(RightDirection.GetSafeNormal(), InputDirection.X);
		AddMovementInput(GetActorForwardVector(), InputDirection.Y);

		SetActorRotation(FRotator(0.0f, GetControlRotation().Yaw, GetControlRotation().Roll));

		PlayFootstepSounds();
	}
}

void APlayerCharacter::PlayIdleAnimation()
{
	if (!actionComponent || !actionComponent->HasAction(freeRoam)) return;

	if (idleAnimation && locomotionState != ELocomotionState::EIdle)
	{
		moveTimer = 1.0f;

		// Play the idle animation
		GetMesh()->SetAnimation(idleAnimation);
		GetMesh()->Play(true);

		locomotionState = ELocomotionState::EIdle;
	}
}

void APlayerCharacter::PlayFootstepSounds()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		switch (footstepState)
		{
		case EFootstepState::EMovingOnLand:

			moveTimer += GetWorld()->GetDeltaSeconds();

			if (moveTimer >= timeToPlayFootstepSoundAgain)
			{
				PlayDryFootstepSound();
				moveTimer = 0.0f;
			}

			break;

		case EFootstepState::EMovingOnWater:

			moveTimer += GetWorld()->GetDeltaSeconds();

			if (moveTimer >= timeToPlayFootstepSoundAgain)
			{
				PlayWaterFootstepSound();
				moveTimer = 0.0f;
			}

			break;

		default:
			break;
		}
	}

	else if (GetCharacterMovement()->IsFalling() && moveTimer != 1.0f)
	{
		moveTimer = 1.0f;
	}
}

void APlayerCharacter::MoveCamera(const FInputActionValue& value_)
{
	// Fixes the spinning balls effect by checking if the game isn't actually paused
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (actionComponent && !actionComponent->HasAction(freeRoam)) return;

		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// Building for mobile (controlling player camera)
			if (gameMode->GetIsForMobile())
			{
				// Calculate the delta touch location based on difference between current value and the start touch location
				FVector2D deltaTouchLocation = value_.Get<FVector2D>() - startTouchLocation;

				// Normalize delta touch location to prevent really fast swipe movement
				deltaTouchLocation.Normalize();

				// Move the camera based on swipe location
				AddControllerYawInput(deltaTouchLocation.X);
				AddControllerPitchInput(deltaTouchLocation.Y);

				// Update start touch location as well to swipe properly again
				startTouchLocation = value_.Get<FVector2D>();

				return;
			}
		}

		//UE_LOG(LogTemp, Warning, TEXT("Moving camera in PC"));

		AddControllerYawInput(value_.Get<FVector2D>().X);
		AddControllerPitchInput(value_.Get<FVector2D>().Y);
	}
}

void APlayerCharacter::PauseGame()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()) && actionComponent && !actionComponent->HasAction(checkingOutFinalScore))
	{
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->EnableMouseCursor();
			gameMode->PushWidget(gameMode->GetPauseMenuClass());
		}
	}
}

void APlayerCharacter::InteractWithFish()
{
	if (interactableFish && actionComponent && actionComponent->HasAction(freeRoam))
	{
		// Start dancing minigame
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->PushWidget(gameMode->GetRhythmMinigameClass());

			if (actionComponent && actionComponent->OnRemoveAction.IsBound())
			{
				StopAllAnimations();
				actionComponent->OnRemoveAction.Broadcast(freeRoam);
			}

			if (actionComponent && actionComponent->OnAddAction.IsBound())
			{
				actionComponent->OnAddAction.Broadcast(playingRhythmMinigame);
			}

			GetCharacterMovement()->DisableMovement();
			if (GetCharacterMovement()->GravityScale != 0.0f) GetCharacterMovement()->GravityScale = 0.0f;

			lastPosition = GetActorLocation();
			lastActorRotation = GetActorRotation();
			lastControlRotation = GetControlRotation();

			gameMode->StartRhythmMinigame();

			Cast<AInteractableFish>(interactableFish)->InvalidateDynamicMaterial();
		}
	}
}

void APlayerCharacter::SpawnBoxTriggerForFish()
{
	for (TActorIterator<AInteractableFish> fishIt(GetWorld()); fishIt; ++fishIt)
	{
		AInteractableFish* interactable_Fish = *fishIt;

		interactable_Fish->SpawnBoxTrigger();
		interactable_Fish = nullptr;
	}
}

void APlayerCharacter::UpdateWalkingAnimations(FVector2D& direction)
{
	if (walkingAnimations.IsEmpty()) return;

	// Start playing the walking forward animation
	if (direction.X > -0.01f && direction.X < 0.01f && direction.Y > 0.01f && walkingAnimations[0] &&
		locomotionState != ELocomotionState::EMovingForward)
	{
		GetMesh()->SetAnimation(walkingAnimations[0]);
		GetMesh()->Play(true);

		locomotionState = ELocomotionState::EMovingForward;
	}
	
	// Start playing the walking backward animation
	if (direction.X > -0.01f && direction.X < 0.01f && direction.Y < -0.01f && walkingAnimations[1] &&
		locomotionState != ELocomotionState::EMovingBackward)
	{
		GetMesh()->SetAnimation(walkingAnimations[1]);
		GetMesh()->Play(true);

		locomotionState = ELocomotionState::EMovingBackward;
	}
	
	// Start playing the walking left animation
	if (direction.Y > -0.01f && direction.Y < 0.01f && direction.X < -0.01f && walkingAnimations[2] &&
		locomotionState != ELocomotionState::EMovingLeft)
	{
		GetMesh()->SetAnimation(walkingAnimations[2]);
		GetMesh()->Play(true);

		locomotionState = ELocomotionState::EMovingLeft;
	}
	
	// Start playing the walking right animation
	if (direction.Y > -0.01f && direction.Y < 0.01f && direction.X > 0.01f && walkingAnimations[3] &&
		locomotionState != ELocomotionState::EMovingRight)
	{
		GetMesh()->SetAnimation(walkingAnimations[3]);
		GetMesh()->Play(true);

		locomotionState = ELocomotionState::EMovingRight;
	}
}

void APlayerCharacter::ResetPlayerActions()
{
	if (!actionComponent) return;

	if (GetWorld()->GetTimerManager().IsTimerActive(randomDanceHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(randomDanceHandle);
	}

	// Play the idle animation
	if (idleAnimation)
	{
		GetMesh()->SetAnimation(idleAnimation);
		GetMesh()->Play(true);
	}

	// Remove all other actions
	if (actionComponent->OnRemoveAction.IsBound()) actionComponent->OnRemoveAction.Broadcast(checkingOutFinalScore);
	if (actionComponent->OnRemoveAction.IsBound()) actionComponent->OnRemoveAction.Broadcast(playingRhythmMinigame);

	// Add the free roam action again
	if (actionComponent->OnAddAction.IsBound()) actionComponent->OnAddAction.Broadcast(freeRoam);

	if (GetCharacterMovement()->GravityScale != 1.0f) GetCharacterMovement()->GravityScale = 1.0f;

	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void APlayerCharacter::SetPlayerActionToPlayingRhythmMinigame()
{
	// Remove all other actions
	if (actionComponent->OnRemoveAction.IsBound()) actionComponent->OnRemoveAction.Broadcast(freeRoam);
	if (actionComponent->OnRemoveAction.IsBound()) actionComponent->OnRemoveAction.Broadcast(checkingOutFinalScore);

	// Add the free roam action again
	if (actionComponent->OnAddAction.IsBound()) actionComponent->OnAddAction.Broadcast(playingRhythmMinigame);
}

void APlayerCharacter::SetPlayerActionToFinalScore()
{
	if (!actionComponent) return;

	// Remove all other actions
	if (actionComponent->OnRemoveAction.IsBound()) actionComponent->OnRemoveAction.Broadcast(freeRoam);
	if (actionComponent->OnRemoveAction.IsBound()) actionComponent->OnRemoveAction.Broadcast(playingRhythmMinigame);

	// Add checking out final score action
	if (actionComponent->OnAddAction.IsBound()) actionComponent->OnAddAction.Broadcast(checkingOutFinalScore);
}

void APlayerCharacter::ResetPlayerBackToWorld()
{
	if (GetCharacterMovement()->GravityScale != 1.0f) GetCharacterMovement()->GravityScale = 1.0f;

	SetActorLocation(lastPosition);
	SetActorRotation(lastActorRotation);

	if (GetController()) GetController()->SetControlRotation(lastControlRotation);

	if (GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Walking)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void APlayerCharacter::InvalidateFishActor()
{
	interactableFish = nullptr;
}

void APlayerCharacter::PlayDanceAnimation()
{
	const int& randomDanceIndex = FMath::RandRange(0, danceAnimations.Num() - 1);
	
	if (danceAnimations.Num() >= randomDanceIndex + 1 && danceAnimations[randomDanceIndex])
	{
		GetMesh()->SetAnimation(danceAnimations[randomDanceIndex]);
		GetMesh()->Play(true);

		GetWorld()->GetTimerManager().SetTimer(randomDanceHandle, this, &APlayerCharacter::PlayDanceAnimation,
			danceAnimations[randomDanceIndex]->GetPlayLength(), true);
	}
}

void APlayerCharacter::PlayMissedNoteDanceAnimation()
{
	if (missNoteDanceAnimation)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(randomDanceHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(randomDanceHandle);
		}

		GetMesh()->SetAnimation(missNoteDanceAnimation);
		GetMesh()->Play(false);

		FTimerHandle danceHandle;
		GetWorld()->GetTimerManager().SetTimer(danceHandle, this, &APlayerCharacter::PlayDanceAnimation,
			0.85f, false);
	}
}

void APlayerCharacter::PlayWinDanceAnimation()
{
	if (winDanceAnimation)
	{
		GetMesh()->SetAnimation(winDanceAnimation);
		GetMesh()->Play(false);
	}
}

void APlayerCharacter::StopAllAnimations()
{
	moveTimer = 1.0f;

	if (GetWorld()->GetTimerManager().IsTimerActive(randomDanceHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(randomDanceHandle);
	}

	GetMesh()->SetAnimation(nullptr);
	GetMesh()->Stop();
}

void APlayerCharacter::SetGameVolumes()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (currentMusicVolume != gameMode->GetMusicVolume()) currentMusicVolume = gameMode->GetMusicVolume();
		if (currentSFXVolume != gameMode->GetSFXVolume()) currentSFXVolume = gameMode->GetSFXVolume();
	}
}

void APlayerCharacter::PlayDryFootstepSound()
{
	if (!footstepSounds.IsEmpty() && footstepSounds[0])
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), footstepSounds[0], GetActorLocation(), currentSFXVolume);
	}
}

void APlayerCharacter::PlayWaterFootstepSound()
{
	if (footstepSounds.Num() >= 2 && footstepSounds[1])
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), footstepSounds[1], GetActorLocation(), currentSFXVolume);
	}
}

void APlayerCharacter::OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AInteractableFish::StaticClass()) && !interactableFish)
	{
		interactableFish = Cast<AInteractableFish>(OtherActor);
	}

	if (OtherActor->ActorHasTag(waterTag) && footstepState != EFootstepState::EMovingOnWater)
	{
		moveTimer = 1.0f;
		footstepState = EFootstepState::EMovingOnWater;
	}
}

void APlayerCharacter::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AInteractableFish::StaticClass()) && interactableFish &&
		actionComponent && actionComponent->HasAction(freeRoam))
	{
		interactableFish = nullptr;
	}

	if (OtherActor->ActorHasTag(waterTag) && footstepState != EFootstepState::EMovingOnLand)
	{
		moveTimer = 1.0f;
		footstepState = EFootstepState::EMovingOnLand;
	}
}

void APlayerCharacter::OnPlayerHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, const FHitResult& Hit)
{

}
