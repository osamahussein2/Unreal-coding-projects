// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MobileWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Player/PlayerCharacter.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Kismet/GameplayStatics.h"

UMobileWidget::UMobileWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMobileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InteractButton && InteractButton->IsVisible()) InteractButton->SetVisibility(ESlateVisibility::Hidden);
	if (DropFoodButton && DropFoodButton->IsVisible()) DropFoodButton->SetVisibility(ESlateVisibility::Hidden);

	if (InteractButton) InteractButton->OnPressed.AddDynamic(this, &UMobileWidget::Interact);
	if (DropFoodButton) DropFoodButton->OnPressed.AddDynamic(this, &UMobileWidget::DropFood);
}

void UMobileWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (DropFoodText)
	{
		DropFoodText->RemoveFromParent();
		DropFoodText = nullptr;
	}

	if (DropFoodButton)
	{
		if (DropFoodButton->OnPressed.IsBound()) DropFoodButton->OnPressed.RemoveDynamic(this, &UMobileWidget::DropFood);

		DropFoodButton->RemoveFromParent();
		DropFoodButton = nullptr;
	}

	if (InteractText)
	{
		InteractText->RemoveFromParent();
		InteractText = nullptr;
	}

	if (InteractButton)
	{
		if (InteractButton->OnPressed.IsBound()) InteractButton->OnPressed.RemoveDynamic(this, &UMobileWidget::Interact);

		InteractButton->RemoveFromParent();
		InteractButton = nullptr;
	}

	if (MobileCanvas)
	{
		MobileCanvas->RemoveFromParent();
		MobileCanvas = nullptr;
	}
}

void UMobileWidget::SetInteractionText(FString text_)
{
	if (InteractText) InteractText->SetText(FText::FromString(text_));
}

void UMobileWidget::SetDropFoodText(FString text_)
{
	if (DropFoodText) DropFoodText->SetText(FText::FromString(text_));
}

void UMobileWidget::ShowInteractionButton()
{
	if (InteractButton && !InteractButton->IsVisible()) InteractButton->SetVisibility(ESlateVisibility::Visible);
}

void UMobileWidget::ShowDropFoodButton()
{
	if (DropFoodButton && !DropFoodButton->IsVisible()) DropFoodButton->SetVisibility(ESlateVisibility::Visible);
}

void UMobileWidget::HideInteractionButton()
{
	if (InteractButton && InteractButton->IsVisible()) InteractButton->SetVisibility(ESlateVisibility::Hidden);
}

void UMobileWidget::HideDropFoodButton()
{
	if (DropFoodButton && DropFoodButton->IsVisible()) DropFoodButton->SetVisibility(ESlateVisibility::Hidden);
}

void UMobileWidget::Interact()
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		player->StartOrEndCooking();
		player->TakeOrderFromCustomer();
	}
}

void UMobileWidget::DropFood()
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		player->StartDroppingFood();
	}
}
