// Fill out your copyright notice in the Description page of Project Settings.

#include "CineCamera/CutsceneCamera.h"
#include "CineCameraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameMode/GoedWareCutsceneGM.h"
#include "Player/CutscenePlayer.h"

ACutsceneCamera::ACutsceneCamera(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	// Make sure cutscene camera can tick for player to rotate camera around during cutscene
	PrimaryActorTick.bCanEverTick = true;

	// Modify the cine camera component of this actor's parameters
	GetCineCameraComponent()->bConstrainAspectRatio = false;

	hasCutsceneCameraFinishedRotating = true;
	rotationX = 0.0f;
	rotationY = 0.0f;
	deltaTime = 0.0f;
}

void ACutsceneCamera::BeginPlay()
{
	Super::BeginPlay();
}

void ACutsceneCamera::Tick(float DeltaTime)
{
	// Rotate the cutscene camera only when it hasn't finished rotating yet
	if (!hasCutsceneCameraFinishedRotating && cutsceneGM)
	{
		Super::Tick(DeltaTime);

		// Rotate the camera around properly based on current level sequence index
		switch (cutsceneGM->GetLevelSequenceIndex())
		{
		case 0:
		{
			// Get a rotation of the cutscene camera
			FRotator NewRotation = FRotator(rotationY, rotationX, 0.0f);

			// Convert to FQuat to prevent potential rotation issues
			FQuat QuatRotation = FQuat(NewRotation);

			AddActorWorldRotation(QuatRotation);
			break;
		}

		case 1:
		{
			// Get a rotation of the cutscene camera
			FRotator NewRotation = FRotator(rotationY, rotationX, 0.0f);

			// Convert to FQuat to prevent potential rotation issues
			FQuat QuatRotation = FQuat(NewRotation);

			AddActorWorldRotation(QuatRotation);
			break;
		}

		case 2:
		{
			// Get a rotation of the cutscene camera
			FRotator NewRotation = FRotator(-rotationY, rotationX, 0.0f);

			// Convert to FQuat to prevent potential rotation issues
			FQuat QuatRotation = FQuat(NewRotation);

			AddActorWorldRotation(QuatRotation);
			break;
		}

		case 3:
		{
			// Get a rotation of the cutscene camera
			FRotator NewRotation = FRotator(rotationY, rotationX, 0.0f);

			// Convert to FQuat to prevent potential rotation issues
			FQuat QuatRotation = FQuat(NewRotation);

			AddActorWorldRotation(QuatRotation);
			break;
		}

		case 4:
		{
			// Get a rotation of the cutscene camera
			FRotator NewRotation = FRotator(0.0f, rotationX, rotationY);

			// Convert to FQuat to prevent potential rotation issues
			FQuat QuatRotation = FQuat(NewRotation);

			AddActorWorldRotation(QuatRotation);
			break;
		}

		case 5:
		{
			// Get a rotation of the cutscene camera
			FRotator NewRotation = FRotator(rotationY, rotationX, 0.0f);

			// Convert to FQuat to prevent potential rotation issues
			FQuat QuatRotation = FQuat(NewRotation);

			AddActorWorldRotation(QuatRotation);
			break;
		}

		case 6:
		{
			// Get a rotation of the cutscene camera
			FRotator NewRotation = FRotator(rotationY, rotationX, 0.0f);

			// Convert to FQuat to prevent potential rotation issues
			FQuat QuatRotation = FQuat(NewRotation);

			AddActorWorldRotation(QuatRotation);
			break;
		}

		default:
		{
			// Get a rotation of the cutscene camera
			FRotator NewRotation = FRotator(rotationY, rotationX, 0.0f);

			// Convert to FQuat to prevent potential rotation issues
			FQuat QuatRotation = FQuat(NewRotation);

			AddActorWorldRotation(QuatRotation);
			break;
		}
		}
	}

	// Make sure the delta time variable is equal to Unreal's tick delta time value
	if (deltaTime != DeltaTime)
	{
		Super::Tick(DeltaTime);
		deltaTime = DeltaTime;
	}

	/*if (cutscenePlayer.IsValid() && cutscenePlayer->isCutscenePlaying)
	{
		Super::Tick(DeltaTime);
		static_cast<float&>(timer) += DeltaTime;

		UE_LOG(LogTemp, Warning, TEXT("%f"), timer);
	}

	else if (cutscenePlayer.IsValid() && !cutscenePlayer->isCutscenePlaying && timer != 0.0f)
	{
		Super::Tick(DeltaTime);
		timer = 0.0f;
	}*/
}

void ACutsceneCamera::RotateCutsceneCamera(const FVector2D& value_)
{
	// Rotate cutscene camera pitch and yaw
	if (rotationX > -maxRotationX && rotationX < maxRotationX) rotationX += rotationSpeed * value_.X * deltaTime;
	if (rotationY > -maxRotationY && rotationY < maxRotationY) rotationY += rotationSpeed * value_.Y * deltaTime;

	if (hasCutsceneCameraFinishedRotating) hasCutsceneCameraFinishedRotating = false;
	if (!cutsceneGM) cutsceneGM = Cast<AGoedWareCutsceneGM>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ACutsceneCamera::ReverseCutsceneCameraRotation()
{
	// Rotate cutscene camera pitch in reverse
	if (rotationX <= -0.1f * (rotationSpeed / 5.0f)) rotationX += rotationSpeed * deltaTime;
	else if (rotationX >= 0.1f * (rotationSpeed / 5.0f)) rotationX -= rotationSpeed * deltaTime;

	// Rotate cutscene camera yaw in reverse
	if (rotationY <= -0.1f * (rotationSpeed / 5.0f)) rotationY += rotationSpeed * deltaTime;
	else if (rotationY >= 0.1f * (rotationSpeed / 5.0f)) rotationY -= rotationSpeed * deltaTime;

	// Reset rotation values
	if (rotationX > -0.1f * (rotationSpeed / 5.0f) && rotationX < 0.1f * (rotationSpeed / 5.0f) &&
		rotationY > -0.1f * (rotationSpeed / 5.0f) && rotationY < 0.1f * (rotationSpeed / 5.0f))
	{
		ResetCutsceneCameraParameters();
	}
}

void ACutsceneCamera::ResetCutsceneCameraParameters()
{
	// Reset cutscene camera parameters
	if (rotationX != 0.0f) rotationX = 0.0f;
	if (rotationY != 0.0f) rotationY = 0.0f;
	if (!hasCutsceneCameraFinishedRotating) hasCutsceneCameraFinishedRotating = true;
	if (cutsceneGM) cutsceneGM = nullptr;

	/*if (!cutscenePlayer.IsValid())
	{
		TArray<AActor*> foundPlayer;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACutscenePlayer::StaticClass(), foundPlayer);

		cutscenePlayer = Cast<ACutscenePlayer>(foundPlayer[0]);
	}*/
}

void ACutsceneCamera::SetConstrainAspectRatio(bool value_)
{
	GetCineCameraComponent()->bConstrainAspectRatio = value_;
}
