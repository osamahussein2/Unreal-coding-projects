// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "GameMode/DungeonCrawlerGM.h"
#include "Player/PlayerCharacter.h"
#include "Actor/Door.h"
#include "EngineUtils.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/HealthComponent.h"

UInventoryWidget::UInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;

	UpdateInventory(0, player->GetCollectedSmallHealthPotions());
	UpdateInventory(1, player->GetCollectedMediumHealthPotions());
	UpdateInventory(2, player->GetCollectedLargeHealthPotions());

	if (InventorySlot1) InventorySlot1->OnHovered.AddUniqueDynamic(this, &UInventoryWidget::HoveredOnSmallHealthPotion);
	if (InventorySlot2) InventorySlot2->OnHovered.AddUniqueDynamic(this, &UInventoryWidget::HoveredOnMediumHealthPotion);
	if (InventorySlot3) InventorySlot3->OnHovered.AddUniqueDynamic(this, &UInventoryWidget::HoveredOnLargeHealthPotion);

	if (InventorySlot1) InventorySlot1->OnClicked.AddUniqueDynamic(this, &UInventoryWidget::UseSmallHealthPotion);
	if (InventorySlot2) InventorySlot2->OnClicked.AddUniqueDynamic(this, &UInventoryWidget::UseMediumHealthPotion);
	if (InventorySlot3) InventorySlot3->OnClicked.AddUniqueDynamic(this, &UInventoryWidget::UseLargeHealthPotion);

	if (BackButton) BackButton->OnClicked.AddUniqueDynamic(this, &UInventoryWidget::GoBackToGame);

	if (SelectedSlotImage && SelectedSlotImage->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		SelectedSlotImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (selectedSlot != 1) selectedSlot = 1;

	if (!player->GetHealthComponent()) return;

	if (HealthBar) HealthBar->SetPercent(player->GetHealthComponent()->GetHealthPercentage());
}

void UInventoryWidget::NativeDestruct()
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

	if (HealthBar)
	{
		HealthBar->RemoveFromParent();
		HealthBar = nullptr;
	}

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (SelectedSlotImage)
	{
		SelectedSlotImage->RemoveFromParent();
		SelectedSlotImage = nullptr;
	}

	if (InventoryOverlay)
	{
		InventoryOverlay->RemoveFromParent();
		InventoryOverlay = nullptr;
	}

	if (InventoryCanvas)
	{
		InventoryCanvas->RemoveFromParent();
		InventoryCanvas = nullptr;
	}
}

void UInventoryWidget::UpdateInventory(int inventoryTextIndex_, const int& collectedItems_)
{
	switch (inventoryTextIndex_)
	{
	case 0:
		if (InventoryText1) InventoryText1->SetText(FText::FromString(FString::FromInt(collectedItems_)));
		break;

	case 1:
		if (InventoryText2) InventoryText2->SetText(FText::FromString(FString::FromInt(collectedItems_)));
		break;

	case 2:
		if (InventoryText3) InventoryText3->SetText(FText::FromString(FString::FromInt(collectedItems_)));
		break;

	default:
		break;
	}
}

void UInventoryWidget::UnbindDelegates()
{
	if (InventorySlot1) InventorySlot1->OnHovered.RemoveDynamic(this, &UInventoryWidget::HoveredOnSmallHealthPotion);
	if (InventorySlot2) InventorySlot2->OnHovered.RemoveDynamic(this, &UInventoryWidget::HoveredOnMediumHealthPotion);
	if (InventorySlot3) InventorySlot3->OnHovered.RemoveDynamic(this, &UInventoryWidget::HoveredOnLargeHealthPotion);

	if (InventorySlot1) InventorySlot1->OnClicked.RemoveDynamic(this, &UInventoryWidget::UseSmallHealthPotion);
	if (InventorySlot2) InventorySlot2->OnClicked.RemoveDynamic(this, &UInventoryWidget::UseMediumHealthPotion);
	if (InventorySlot3) InventorySlot3->OnClicked.RemoveDynamic(this, &UInventoryWidget::UseLargeHealthPotion);

	if (BackButton) BackButton->OnClicked.RemoveDynamic(this, &UInventoryWidget::GoBackToGame);
}

void UInventoryWidget::HoveredOnSmallHealthPotion()
{
	if (selectedSlot != 1)
	{
		selectedSlot = 1;

		if (SelectedSlotImage && InventorySlot1)
		{
			if (UOverlaySlot* CurrentPhotoSelectionSlot = Cast<UOverlaySlot>(SelectedSlotImage->Slot))
			{
				if (UOverlaySlot* snapshotImage1Slot = Cast<UOverlaySlot>(InventorySlot1->Slot))
				{
					CurrentPhotoSelectionSlot->SetPadding(snapshotImage1Slot->GetPadding());
				}
			}
		}
	}
}

void UInventoryWidget::UseSmallHealthPotion()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player || !player->GetHealthComponent()) return;

	if (player->GetCollectedSmallHealthPotions() >= 1 && !player->GetHealthComponent()->IsHealthFull())
	{
		player->HealWithSmallHealthPotion();
		if (InventoryText1) InventoryText1->SetText(FText::FromString(FString::FromInt(player->GetCollectedSmallHealthPotions())));
		if (HealthBar) HealthBar->SetPercent(player->GetHealthComponent()->GetHealthPercentage());
	}
}

void UInventoryWidget::HoveredOnMediumHealthPotion()
{
	if (selectedSlot != 2)
	{
		selectedSlot = 2;

		if (SelectedSlotImage && InventorySlot2)
		{
			if (UOverlaySlot* CurrentPhotoSelectionSlot = Cast<UOverlaySlot>(SelectedSlotImage->Slot))
			{
				if (UOverlaySlot* snapshotImage2Slot = Cast<UOverlaySlot>(InventorySlot2->Slot))
				{
					CurrentPhotoSelectionSlot->SetPadding(snapshotImage2Slot->GetPadding());
				}
			}
		}
	}
}

void UInventoryWidget::UseMediumHealthPotion()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player || !player->GetHealthComponent()) return;

	if (player->GetCollectedMediumHealthPotions() >= 1 && !player->GetHealthComponent()->IsHealthFull())
	{
		player->HealWithMediumHealthPotion();
		if (InventoryText2) InventoryText2->SetText(FText::FromString(FString::FromInt(player->GetCollectedMediumHealthPotions())));
		if (HealthBar) HealthBar->SetPercent(player->GetHealthComponent()->GetHealthPercentage());
	}
}

void UInventoryWidget::HoveredOnLargeHealthPotion()
{
	if (selectedSlot != 3)
	{
		selectedSlot = 3;

		if (SelectedSlotImage && InventorySlot3)
		{
			if (UOverlaySlot* CurrentPhotoSelectionSlot = Cast<UOverlaySlot>(SelectedSlotImage->Slot))
			{
				if (UOverlaySlot* snapshotImage3Slot = Cast<UOverlaySlot>(InventorySlot3->Slot))
				{
					CurrentPhotoSelectionSlot->SetPadding(snapshotImage3Slot->GetPadding());
				}
			}
		}
	}
}

void UInventoryWidget::UseLargeHealthPotion()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player || !player->GetHealthComponent()) return;

	if (player->GetCollectedLargeHealthPotions() >= 1 && !player->GetHealthComponent()->IsHealthFull())
	{
		player->HealWithLargeHealthPotion();
		if (InventoryText3) InventoryText3->SetText(FText::FromString(FString::FromInt(player->GetCollectedLargeHealthPotions())));
		if (HealthBar) HealthBar->SetPercent(player->GetHealthComponent()->GetHealthPercentage());
	}
}

void UInventoryWidget::GoBackToGame()
{
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;

	UnbindDelegates();
	player->CloseInventory();
}
