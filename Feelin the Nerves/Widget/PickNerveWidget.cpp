// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PickNerveWidget.h"
#include "GameMode/GoedWareCommunityGM.h"
#include "Kismet/GameplayStatics.h"

UPickNerveWidget::UPickNerveWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPickNerveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PickNerveCanvas && PickNerveCanvas->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		PickNerveCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (PickNerveOverlay && PickNerveOverlay->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		PickNerveOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (BlackImage && BlackImage->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		BlackImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UPickNerveWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (PickNerveOverlay)
	{
		PickNerveOverlay->RemoveFromParent();
		PickNerveOverlay = nullptr;
	}

	if (PickNerveCanvas)
	{
		PickNerveCanvas->RemoveFromParent();
		PickNerveCanvas = nullptr;
	}
}

void UPickNerveWidget::RemoveBindedButtonDelegates()
{
	// Override in child classes of UPickNerveWidget
}