// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/CollapsibleInventoryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/DungeonCrawlerGM.h"
#include "Player/PlayerCharacter.h"
#include "Actor/Door.h"
#include "EngineUtils.h"

UCollapsibleInventoryWidget::UCollapsibleInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCollapsibleInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateInventorySlot();
}

void UCollapsibleInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (InventoryText1)
	{
		InventoryText1->RemoveFromParent();
		InventoryText1 = nullptr;
	}

	if (InventoryText2)
	{
		InventoryText2->RemoveFromParent();
		InventoryText2 = nullptr;
	}

	if (InventoryText3)
	{
		InventoryText3->RemoveFromParent();
		InventoryText3 = nullptr;
	}

	if (InventorySlot1)
	{
		InventorySlot1->RemoveFromParent();
		InventorySlot1 = nullptr;
	}

	if (InventorySlot2)
	{
		InventorySlot2->RemoveFromParent();
		InventorySlot2 = nullptr;
	}

	if (InventorySlot3)
	{
		InventorySlot3->RemoveFromParent();
		InventorySlot3 = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (InventoryOverlay)
	{
		InventoryOverlay->RemoveFromParent();
		InventoryOverlay = nullptr;
	}
}

void UCollapsibleInventoryWidget::UpdateInventorySlot()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;

	UpdateInventoryText(0, player->GetCollectedSmallHealthPotions());
	UpdateInventoryText(1, player->GetCollectedMediumHealthPotions());
	UpdateInventoryText(2, player->GetCollectedLargeHealthPotions());
}

/*void UCollapsibleInventoryWidget::DestroyInventoryText(int inventoryTextIndex_)
{
	switch (inventoryTextIndex_)
	{
	case 0:

		if (InventoryText1)
		{
			InventoryText1->RemoveFromParent();
			InventoryText1 = nullptr;
		}

		break;

	case 1:

		if (InventoryText2)
		{
			InventoryText2->RemoveFromParent();
			InventoryText2 = nullptr;
		}

		break;

	case 2:
		
		if (InventoryText3)
		{
			InventoryText3->RemoveFromParent();
			InventoryText3 = nullptr;
		}

		break;

	default:
		break;
	}
}*/

void UCollapsibleInventoryWidget::UpdateInventoryText(int inventoryTextIndex_, const int& collectedItems_)
{
	switch (inventoryTextIndex_)
	{
	case 0:
		//if (InventorySlot1) InventorySlot1->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		//if (InventoryText1) InventoryText1->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		if (InventoryText1) InventoryText1->SetText(FText::FromString(FString::FromInt(collectedItems_)));
		break;

	case 1:
		//if (InventorySlot2) InventorySlot2->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		//if (InventoryText2) InventoryText2->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		if (InventoryText2) InventoryText2->SetText(FText::FromString(FString::FromInt(collectedItems_)));
		break;

	case 2:
		//if (InventorySlot3) InventorySlot3->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		//if (InventoryText3) InventoryText3->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
		if (InventoryText3) InventoryText3->SetText(FText::FromString(FString::FromInt(collectedItems_)));
		break;

	default:
		break;
	}
}