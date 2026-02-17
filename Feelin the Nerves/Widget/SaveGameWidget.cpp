// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/SaveGameWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/PlayerSaveGame.h"
#include "GameMode/GoedWareCommunityGM.h"

USaveGameWidget::USaveGameWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void USaveGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SaveGameCanvasPanel && SaveGameCanvasPanel->GetVisibility() != ESlateVisibility::Visible)
	{
		SemiTransparentImage->SetVisibility(ESlateVisibility::Visible);
	}

	if (SemiTransparentImage && SemiTransparentImage->GetVisibility() != ESlateVisibility::Visible)
	{
		SemiTransparentImage->SetVisibility(ESlateVisibility::Visible);
	}

	if (SaveGameOverlay && SaveGameOverlay->GetVisibility() != ESlateVisibility::Visible)
	{
		SaveGameOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	if (SaveSlotButton1 && SaveSlotButton1->GetVisibility() != ESlateVisibility::Visible)
	{
		SaveSlotButton1->SetVisibility(ESlateVisibility::Visible);
	}

	if (SaveSlotText1 && SaveSlotText1->GetVisibility() != ESlateVisibility::Visible)
	{
		SaveSlotText1->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton && BackButton->GetVisibility() != ESlateVisibility::Visible)
	{
		BackButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton) BackButton->OnPressed.AddUniqueDynamic(this, &USaveGameWidget::GoBackToPauseMenu);
	if (SaveSlotButton1) SaveSlotButton1->OnPressed.AddUniqueDynamic(this, &USaveGameWidget::SaveInSlot1);

	if (SaveSlotText1)
	{
		if (Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSave1", 0)))
		{
			SaveSlotText1->SetText(FText::FromString("Slot 1 save found!"));
		}

		else
		{
			SaveSlotText1->SetText(FText::FromString("Save Slot 1 not found!"));
		}
	}
}

void USaveGameWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
	}

	if (SaveSlotText1)
	{
		SaveSlotText1->RemoveFromParent();
		SaveSlotText1 = nullptr;
	}

	if (SaveSlotButton1)
	{
		SaveSlotButton1->RemoveFromParent();
		SaveSlotButton1 = nullptr;
	}

	if (SaveGameOverlay)
	{
		SaveGameOverlay->RemoveFromParent();
		SaveGameOverlay = nullptr;
	}

	if (SemiTransparentImage)
	{
		SemiTransparentImage->RemoveFromParent();
		SemiTransparentImage = nullptr;
	}

	if (SaveGameCanvasPanel)
	{
		SaveGameCanvasPanel->RemoveFromParent();
		SaveGameCanvasPanel = nullptr;
	}
}

void USaveGameWidget::GoBackToPauseMenu()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetSaveGameClass());
		gameMode->PushWidget(gameMode->GetPauseMenuClass());
	}

	RemoveBindedButtonDelegates();
}

void USaveGameWidget::SaveInSlot1()
{
	if (UPlayerSaveGame* playerSave = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass())))
	{
		if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// Save the player's max played, correct and current complete sequences values
			playerSave->SetPlayerMaxPlayedSequences(gameMode->GetMaxPlayedSequences());
			playerSave->SetPlayerMaxCorrectSequences(gameMode->GetMaxCorrectSequences());
			playerSave->SetPlayerCurrentCompletedSequences(gameMode->GetCurrentCompletedSequences());

			// If necessary, clear all the saved random nerves from an earlier save to prevent in-game errors/bugs
			playerSave->EmptySavedRandomNerves();

			// Save the random nerves values from the random nerve index value from the game mode class
			for (int i = 0; i < gameMode->GetRandomNervesSize(); ++i)
			{
				playerSave->SetSavedRandomNerves(gameMode->GetRandomNerveIndex(i));
			}

			// Update text to saved if the slot is found
			if (UGameplayStatics::SaveGameToSlot(playerSave, "PlayerSave1", 0))
			{
				if (SaveSlotText1) SaveSlotText1->SetText(FText::FromString("Slot 1 saved successfully!"));
			}

			// Otherwise, it can't save
			else
			{
				if (SaveSlotText1) SaveSlotText1->SetText(FText::FromString("Save Slot 1 save unsuccessful!"));
			}
		}
	}
}

void USaveGameWidget::RemoveBindedButtonDelegates()
{
	if (BackButton) BackButton->OnPressed.RemoveDynamic(this, &USaveGameWidget::GoBackToPauseMenu);
	if (SaveSlotButton1) SaveSlotButton1->OnPressed.RemoveDynamic(this, &USaveGameWidget::SaveInSlot1);
}
