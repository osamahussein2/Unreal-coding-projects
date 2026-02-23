// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/CrosshairWidget.h"
#include "Components/Image.h"

UCrosshairWidget::UCrosshairWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (crosshairImage && crosshairImage->GetVisibility() != ESlateVisibility::Visible)
	{
		crosshairImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCrosshairWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (crosshairImage)
	{
		crosshairImage->RemoveFromParent();
		crosshairImage = nullptr;
	}
}