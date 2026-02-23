// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/InputWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "GameMode/BrackeysGM.h"

UInputWidget::UInputWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInputWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InputOverlay && InputOverlay->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		InputOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (BlackImage && BlackImage->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		BlackImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (InputText && InputText->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		InputText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (ActionText && ActionText->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		ActionText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UInputWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (InputOverlay)
	{
		InputOverlay->RemoveFromParent();
		InputOverlay = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (InputText)
	{
		InputText->RemoveFromParent();
		InputText = nullptr;
	}

	if (ActionText)
	{
		ActionText->RemoveFromParent();
		ActionText = nullptr;
	}
}