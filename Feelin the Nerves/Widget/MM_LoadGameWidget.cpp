// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MM_LoadGameWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/PlayerSaveGame.h"
#include "GameMode/GoedWareCommunityGM.h"

UMM_LoadGameWidget::UMM_LoadGameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMM_LoadGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MM_LoadGameCanvasPanel && MM_LoadGameCanvasPanel->GetVisibility() != ESlateVisibility::Visible)
	{
		MM_LoadGameCanvasPanel->SetVisibility(ESlateVisibility::Visible);
	}

	if (MM_LoadGameOverlay && MM_LoadGameOverlay->GetVisibility() != ESlateVisibility::Visible)
	{
		MM_LoadGameOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	if (MM_LoadSlotButton1 && MM_LoadSlotButton1->GetVisibility() != ESlateVisibility::Visible)
	{
		MM_LoadSlotButton1->SetVisibility(ESlateVisibility::Visible);
	}

	if (MM_LoadSlotText1 && MM_LoadSlotText1->GetVisibility() != ESlateVisibility::Visible)
	{
		MM_LoadSlotText1->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton && BackButton->GetVisibility() != ESlateVisibility::Visible)
	{
		BackButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton) BackButton->OnPressed.AddUniqueDynamic(this, &UMM_LoadGameWidget::GoBackToMainMenu);
	if (MM_LoadSlotButton1) MM_LoadSlotButton1->OnPressed.AddUniqueDynamic(this, &UMM_LoadGameWidget::LoadInSlot1);

	ValidateSaveSlot1();
}

void UMM_LoadGameWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
	}

	if (MM_LoadSlotText1)
	{
		MM_LoadSlotText1->RemoveFromParent();
		MM_LoadSlotText1 = nullptr;
	}

	if (MM_LoadSlotButton1)
	{
		MM_LoadSlotButton1->RemoveFromParent();
		MM_LoadSlotButton1 = nullptr;
	}

	if (MM_LoadGameOverlay)
	{
		MM_LoadGameOverlay->RemoveFromParent();
		MM_LoadGameOverlay = nullptr;
	}

	if (MM_LoadGameCanvasPanel)
	{
		MM_LoadGameCanvasPanel->RemoveFromParent();
		MM_LoadGameCanvasPanel = nullptr;
	}
}

void UMM_LoadGameWidget::GoBackToMainMenu()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetMM_LoadGameClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());
	}

	RemoveBindedButtonDelegates();
}

void UMM_LoadGameWidget::LoadInSlot1()
{
	if (UPlayerSaveGame* playerSaveGame = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSave1", 0)))
	{
		if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// Spawn the body actor after loading is successful
			gameMode->SpawnBodyActor();

			// Load the game
			gameMode->LoadGame(playerSaveGame);
		}
	}
}

void UMM_LoadGameWidget::ValidateSaveSlot1()
{
	if (MM_LoadSlotText1)
	{
		if (Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot("PlayerSave1", 0)))
		{
			MM_LoadSlotText1->SetText(FText::FromString("Load Slot 1"));
		}

		else
		{
			MM_LoadSlotText1->SetText(FText::FromString("Slot 1 not available"));
		}
	}
}

void UMM_LoadGameWidget::RemoveBindedButtonDelegates()
{
	if (BackButton) BackButton->OnPressed.RemoveDynamic(this, &UMM_LoadGameWidget::GoBackToMainMenu);
	if (MM_LoadSlotButton1) MM_LoadSlotButton1->OnPressed.RemoveDynamic(this, &UMM_LoadGameWidget::LoadInSlot1);
}
