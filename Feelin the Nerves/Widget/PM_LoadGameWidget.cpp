// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PM_LoadGameWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/PlayerSaveGame.h"
#include "GameMode/GoedWareCommunityGM.h"

UPM_LoadGameWidget::UPM_LoadGameWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPM_LoadGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PM_LoadGameCanvasPanel && PM_LoadGameCanvasPanel->GetVisibility() != ESlateVisibility::Visible)
	{
		PM_LoadGameCanvasPanel->SetVisibility(ESlateVisibility::Visible);
	}

	if (SemiTransparentImage && SemiTransparentImage->GetVisibility() != ESlateVisibility::Visible)
	{
		SemiTransparentImage->SetVisibility(ESlateVisibility::Visible);
	}

	if (PM_LoadGameOverlay && PM_LoadGameOverlay->GetVisibility() != ESlateVisibility::Visible)
	{
		PM_LoadGameOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	if (PM_LoadSlotButton1 && PM_LoadSlotButton1->GetVisibility() != ESlateVisibility::Visible)
	{
		PM_LoadSlotButton1->SetVisibility(ESlateVisibility::Visible);
	}

	if (PM_LoadSlotText1 && PM_LoadSlotText1->GetVisibility() != ESlateVisibility::Visible)
	{
		PM_LoadSlotText1->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton && BackButton->GetVisibility() != ESlateVisibility::Visible)
	{
		BackButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton) BackButton->OnPressed.AddUniqueDynamic(this, &UPM_LoadGameWidget::GoBackToPauseMenu);
	if (PM_LoadSlotButton1) PM_LoadSlotButton1->OnPressed.AddUniqueDynamic(this, &UPM_LoadGameWidget::LoadInSlot1);

	ValidateSaveSlot1();
}

void UPM_LoadGameWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
	}

	if (PM_LoadSlotText1)
	{
		PM_LoadSlotText1->RemoveFromParent();
		PM_LoadSlotText1 = nullptr;
	}

	if (PM_LoadSlotButton1)
	{
		PM_LoadSlotButton1->RemoveFromParent();
		PM_LoadSlotButton1 = nullptr;
	}

	if (PM_LoadGameOverlay)
	{
		PM_LoadGameOverlay->RemoveFromParent();
		PM_LoadGameOverlay = nullptr;
	}

	if (SemiTransparentImage)
	{
		SemiTransparentImage->RemoveFromParent();
		SemiTransparentImage = nullptr;
	}

	if (PM_LoadGameCanvasPanel)
	{
		PM_LoadGameCanvasPanel->RemoveFromParent();
		PM_LoadGameCanvasPanel = nullptr;
	}
}

void UPM_LoadGameWidget::GoBackToPauseMenu()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetPM_LoadGameClass());
		gameMode->PushWidget(gameMode->GetPauseMenuClass());
	}

	RemoveBindedButtonDelegates();
}

void UPM_LoadGameWidget::LoadInSlot1()
{
	if (UPlayerSaveGame* playerSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSave1", 0)))
	{
		if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// Unpause the game for a frame to update player location and rotation
			UGameplayStatics::SetGamePaused(GetWorld(), false);

			// Load the game
			gameMode->LoadGame(playerSaveGame);
		}
	}
}

void UPM_LoadGameWidget::ValidateSaveSlot1()
{
	if (PM_LoadSlotText1)
	{
		if (Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSave1", 0)))
		{
			PM_LoadSlotText1->SetText(FText::FromString("Load Slot 1"));
		}

		else
		{
			PM_LoadSlotText1->SetText(FText::FromString("Slot 1 not available"));
		}
	}
}

void UPM_LoadGameWidget::RemoveBindedButtonDelegates()
{
	if (BackButton) BackButton->OnPressed.RemoveDynamic(this, &UPM_LoadGameWidget::GoBackToPauseMenu);
	if (PM_LoadSlotButton1) PM_LoadSlotButton1->OnPressed.RemoveDynamic(this, &UPM_LoadGameWidget::LoadInSlot1);
}
