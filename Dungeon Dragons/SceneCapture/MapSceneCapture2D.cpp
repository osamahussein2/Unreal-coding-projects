// Fill out your copyright notice in the Description page of Project Settings.

#include "SceneCapture/MapSceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/TextureRenderTarget2D.h"
#include "EngineUtils.h"

AMapSceneCapture2D::AMapSceneCapture2D(const FObjectInitializer & ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	GetCaptureComponent2D()->ProjectionType = ECameraProjectionMode::Orthographic;

	// Disable atmosphere in scene capture
	GetCaptureComponent2D()->ShowFlags.Atmosphere = 0;
}

void AMapSceneCapture2D::BeginPlay()
{
	Super::BeginPlay();

	GetCaptureComponent2D()->SetTickableWhenPaused(true);

	if (GetCaptureComponent2D()->TextureTarget)
	{
		if (GetCaptureComponent2D()->TextureTarget->SizeX != GEngine->GetGameUserSettings()->GetScreenResolution().X)
		{
			GetCaptureComponent2D()->TextureTarget->SizeX = GEngine->GetGameUserSettings()->GetScreenResolution().X;
		}

		if (GetCaptureComponent2D()->TextureTarget->SizeY != GEngine->GetGameUserSettings()->GetScreenResolution().Y)
		{
			GetCaptureComponent2D()->TextureTarget->SizeY = GEngine->GetGameUserSettings()->GetScreenResolution().Y;
		}
	}

	// Clamp ortho width to be equal to the min and max map ortho width values if one of these statements is true
	if (GetCaptureComponent2D()->OrthoWidth < minFullMapOrthoWidth) GetCaptureComponent2D()->OrthoWidth = minFullMapOrthoWidth;
	else if (GetCaptureComponent2D()->OrthoWidth > maxFullMapOrthoWidth) GetCaptureComponent2D()->OrthoWidth = maxFullMapOrthoWidth;

	// Loop through every actor in the world
	/*for (TActorIterator<AActor> actorIt(GetWorld()); actorIt; ++actorIt)
	{
		// If an actor has the tag of hidden in map capture, add that actor inside the array of actors
		if (actorIt->ActorHasTag(hiddenInMapCaptureTag))
		{
			GetCaptureComponent2D()->HideActorComponents(*actorIt, false);
		}

		// Otherwise, keep going onto the next iteration of actors
		else continue;
	}

	// Loop through every primitive component
	for (TObjectIterator<UPrimitiveComponent> componentIt; componentIt; ++componentIt)
	{
		// If a primitive component has the tag of hidden in map capture, hide that primitive component in scene capture
		if (componentIt->ComponentHasTag(hiddenInMapCaptureTag))
		{
			GetCaptureComponent2D()->HideComponent(*componentIt);
		}

		// Otherwise, keep going onto the next iteration of components
		else continue;
	}*/
}

void AMapSceneCapture2D::Tick(float DeltaTime)
{
}

void AMapSceneCapture2D::MoveMap(EMapMovement mapMovement, float& DeltaTime)
{
	switch (mapMovement)
	{
	case EMapMovement::MoveLeft:
	{
		if (GetCaptureComponent2D()->GetComponentLocation().Y > minPositionY)
		{
			const FVector& left = FVector(0.0f, -moveSpeed * DeltaTime, 0.0f);
			GetCaptureComponent2D()->AddWorldOffset(left);
		}

		break;
	}

	case EMapMovement::MoveRight:
	{
		if (GetCaptureComponent2D()->GetComponentLocation().Y < maxPositionY)
		{
			const FVector& right = FVector(0.0f, moveSpeed * DeltaTime, 0.0f);
			GetCaptureComponent2D()->AddWorldOffset(right);
		}

		break;
	}

	case EMapMovement::MoveDown:
	{
		if (GetCaptureComponent2D()->GetComponentLocation().X > minPositionZ)
		{
			const FVector& down = FVector(-moveSpeed * DeltaTime, 0.0f, 0.0f);
			GetCaptureComponent2D()->AddWorldOffset(down);
		}

		break;
	}

	case EMapMovement::MoveUp:
	{
		if (GetCaptureComponent2D()->GetComponentLocation().X < maxPositionZ)
		{
			const FVector& up = FVector(moveSpeed * DeltaTime, 0.0f, 0.0f);
			GetCaptureComponent2D()->AddWorldOffset(up);
		}

		break;
	}

	default:
		break;
	}
}

void AMapSceneCapture2D::ZoomOutMap(float& DeltaTime)
{
	if (GetWorld() && GetCaptureComponent2D()->OrthoWidth < maxFullMapOrthoWidth)
	{
		GetCaptureComponent2D()->OrthoWidth += zoomSpeed * DeltaTime;
	}
}

void AMapSceneCapture2D::ZoomInMap(float& DeltaTime)
{
	if (GetWorld() && GetCaptureComponent2D()->OrthoWidth > minFullMapOrthoWidth)
	{
		GetCaptureComponent2D()->OrthoWidth -= zoomSpeed * DeltaTime;
	}
}