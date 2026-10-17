// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/CameraSnapshot.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetRenderingLibrary.h"

UCameraSnapshot::UCameraSnapshot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ProjectionType = ECameraProjectionMode::Perspective;
	FOVAngle = 90.0f;
	
	CaptureSource = ESceneCaptureSource::SCS_FinalColorHDR;
	
	bCaptureEveryFrame = true;
	bCaptureOnMovement = true;
	bAlwaysPersistRenderingState = false;
}

void UCameraSnapshot::BeginPlay()
{
	Super::BeginPlay();

	if (TextureTarget)
	{
		UKismetRenderingLibrary::ReleaseRenderTarget2D(TextureTarget);
		TextureTarget = nullptr;
	}

	// Create the texture target as a render target
	TextureTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, renderTargetWidth, renderTargetHeight);

	if (!TextureTarget) return;

	CaptureScene();
}

void UCameraSnapshot::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UCameraSnapshot::ReleaseRenderTarget()
{
	if (!TextureTarget) return;

	UKismetRenderingLibrary::ReleaseRenderTarget2D(TextureTarget);
	TextureTarget = nullptr;
}

UTextureRenderTarget2D* UCameraSnapshot::GetTextureRenderTarget2D() const
{
	return TextureTarget;
}
