// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/DetectionOverlay.h"
#include "Components/ProgressBar.h"

UDetectionOverlay::UDetectionOverlay(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDetectionOverlay::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDetectionOverlay::NativeDestruct()
{
	Super::NativeDestruct();

	if (DetectionBar)
	{
		DetectionBar->RemoveFromParent();
		DetectionBar = nullptr;
	}
}
