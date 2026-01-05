// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CutscenePlayer.h"
#include "InputMappingContext.h"
#include "Input/PlayerInputDA.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CineCameraComponent.h"
#include "CineCameraSettings.h"

// Sets default values
ACutscenePlayer::ACutscenePlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACutscenePlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void ACutscenePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
		(Cast<APlayerController>(GetController())->GetLocalPlayer());

	// Add the input mapping context
	enhancedInputSubsystem->AddMappingContext(inputMapping, 0);

	// Get the EnhancedInputComponent
	UEnhancedInputComponent* playerEnhancedInputcomponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (playerDataAsset && playerDataAsset->GetCutsceneInteractionIA())
	{
		playerEnhancedInputcomponent->BindAction(playerDataAsset->GetCutsceneInteractionIA(), ETriggerEvent::Triggered, this,
			&ACutscenePlayer::InteractCutsceneCamera);

		playerEnhancedInputcomponent->BindAction(playerDataAsset->GetCutsceneInteractionIA(), ETriggerEvent::None, this,
			&ACutscenePlayer::ReverseCutsceneCamera);

		// Mobile touch bind action
		/*playerEnhancedInputcomponent->BindAction(playerDataAsset->GetCutsceneInteractionIA(), ETriggerEvent::Started, this,
			&ACutscenePlayer::StartTouchInput);

		playerEnhancedInputcomponent->BindAction(playerDataAsset->GetCutsceneInteractionIA(), ETriggerEvent::Ongoing, this,
			&ACutscenePlayer::MobileCutsceneCamera);*/
	}
}

void ACutscenePlayer::SetCutsceneCamera()
{
	// Find the cutscene camera actor in the level
	TArray<AActor*> cutsceneCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACutsceneCamera::StaticClass(), cutsceneCameraActors);

	cutsceneCamera = Cast<ACutsceneCamera>(cutsceneCameraActors[0]);
	cutsceneCameraActors[0] = nullptr;
}

void ACutscenePlayer::InteractCutsceneCamera(const FInputActionValue& value_)
{
	if (!cutsceneCamera || !isCutscenePlaying) return;

	cutsceneCamera->RotateCutsceneCamera(value_.Get<FVector2D>());
}

void ACutscenePlayer::ReverseCutsceneCamera()
{
	if (!cutsceneCamera || !isCutscenePlaying) return;

	cutsceneCamera->ReverseCutsceneCameraRotation();
}

void ACutscenePlayer::ResetCutsceneCameraTransform(bool changeCameraFocus_)
{
	if (!cutsceneCamera) return;

	// Reset cutscene camera location and rotation to set starting values
	cutsceneCamera->SetActorLocation(cameraLocation);
	cutsceneCamera->SetActorRotation(cameraRotation);

	// Reset the camera's focus settings back to default
	cutsceneCamera->GetCineCameraComponent()->CurrentFocalLength = 40.0f;

	// Modify camera focus if needed
	if (!changeCameraFocus_) cutsceneCamera->GetCineCameraComponent()->FocusSettings.ManualFocusDistance = 450.0f;
	else if (changeCameraFocus_) cutsceneCamera->GetCineCameraComponent()->FocusSettings.ManualFocusDistance = 200.0f;
}

void ACutscenePlayer::SetCreditsCameraTransform()
{
	if (!cutsceneCamera) return;

	// Set cutscene camera location and rotation to set credits location and rotation
	cutsceneCamera->SetActorLocation(cameraCreditsLocation);
	cutsceneCamera->SetActorRotation(cameraCreditsRotation);
}

void ACutscenePlayer::SetBlackAreaCameraTransform()
{
	if (!cutsceneCamera) return;

	// Set cutscene camera location and rotation to set black area location and rotation
	cutsceneCamera->SetActorLocation(cameraBlackLocation);
	cutsceneCamera->SetActorRotation(cameraCreditsRotation);
}

void ACutscenePlayer::MoveToCreditsCamera(float& DeltaTime)
{
	if (!cutsceneCamera) return;

	// Move the camera towards the set credits location
	FVector moveCamera = cutsceneCamera->GetActorLocation();
	moveCamera += (cameraCreditsLocation - cutsceneCamera->GetActorLocation()).GetSafeNormal() * animateCameraMoveSpeed * DeltaTime;

	// Rotate the camera towards the set credits rotation
	FRotator rotateCamera = cutsceneCamera->GetActorRotation();
	rotateCamera += (cameraCreditsRotation - cutsceneCamera->GetActorRotation()).GetNormalized() * animateCameraRotateSpeed * DeltaTime;

	cutsceneCamera->SetActorLocation(moveCamera);
	cutsceneCamera->SetActorRotation(rotateCamera);
}

void ACutscenePlayer::MoveToMainMenuCamera(float& DeltaTime)
{
	if (!cutsceneCamera) return;

	// Move the camera towards the set main menu location
	FVector moveCamera = cutsceneCamera->GetActorLocation();
	moveCamera += (cameraLocation - cutsceneCamera->GetActorLocation()).GetSafeNormal() * animateCameraMoveSpeed * DeltaTime;

	// Rotate the camera towards the set main menu rotation
	FRotator rotateCamera = cutsceneCamera->GetActorRotation();
	rotateCamera += (cameraRotation - cutsceneCamera->GetActorRotation()).GetNormalized() * animateCameraRotateSpeed * DeltaTime;

	cutsceneCamera->SetActorLocation(moveCamera);
	cutsceneCamera->SetActorRotation(rotateCamera);
}

bool ACutscenePlayer::ReachedCreditsBillboard() const
{
	return FVector::Distance(cutsceneCamera->GetActorLocation(), cameraCreditsLocation) <= minDistanceToDestination;
}

bool ACutscenePlayer::ReachedMainMenu() const
{
	return FVector::Distance(cutsceneCamera->GetActorLocation(), cameraLocation) <= minDistanceToDestination;
}

//void ACutscenePlayer::StartTouchInput(const FInputActionValue& Value)
//{
//	if (cutsceneCamera && isCutscenePlaying)
//	{
//		startTouchLocation = Value.Get<FVector2D>();
//	}
//}
//
//void ACutscenePlayer::MobileCutsceneCamera(const FInputActionValue& Value)
//{
//	// Make sure the camera touch and drag only occurs during the press and drag camera is true
//	if (cutsceneCamera && isCutscenePlaying)
//	{
//		// Calculate the delta touch location based on difference between current value and the start touch location
//		FVector2D deltaTouchLocation = Value.Get<FVector2D>() - startTouchLocation;
//
//		// Normalize delta touch location to prevent really fast swipe movement
//		deltaTouchLocation.Normalize();
//
//		// Move the cutscene camera based on swipe location
//		cutsceneCamera->RotateCutsceneCamera(deltaTouchLocation);
//
//		// Update start touch location as well to swipe properly again
//		startTouchLocation = Value.Get<FVector2D>();
//	}
//}