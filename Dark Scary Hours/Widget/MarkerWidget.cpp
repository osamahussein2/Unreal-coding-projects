// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MarkerWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UMarkerWidget::UMarkerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMarkerWidget::NativeConstruct()
{
	if (MarkerVerticalBox && MarkerVerticalBox->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		MarkerVerticalBox->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (MarkerImage && MarkerImage->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		MarkerImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (DistanceToMarkerText && DistanceToMarkerText->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		DistanceToMarkerText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UMarkerWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (DistanceToMarkerText)
	{
		DistanceToMarkerText->RemoveFromParent();
		DistanceToMarkerText = nullptr;
	}

	if (MarkerImage)
	{
		MarkerImage->RemoveFromParent();
		MarkerImage = nullptr;
	}

	if (MarkerVerticalBox)
	{
		MarkerVerticalBox->RemoveFromParent();
		MarkerVerticalBox = nullptr;
	}
}

void UMarkerWidget::UpdateDistanceText(FVector location_, FVector otherLocation_)
{
	if (DistanceToMarkerText)
	{
		float distance = FVector::Distance(location_, otherLocation_);

		DistanceToMarkerText->SetText(FText::FromString(FString::FromInt(static_cast<int>(distance))));
	}
}