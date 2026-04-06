// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/LevelCompletionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/DungeonCrawlerGM.h"
#include "Player/PlayerCharacter.h"

ULevelCompletionWidget::ULevelCompletionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ULevelCompletionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (LevelCompletedCanvas && !LevelCompletedCanvas->IsVisible()) LevelCompletedCanvas->SetVisibility(ESlateVisibility::Visible);
	if (LevelCompletedTitleText && !LevelCompletedTitleText->IsVisible()) LevelCompletedTitleText->SetVisibility(ESlateVisibility::Visible);
	if (RestartButton && !RestartButton) RestartButton->SetVisibility(ESlateVisibility::Visible);
	if (QuitButton && !QuitButton) QuitButton->SetVisibility(ESlateVisibility::Visible);

	if (RestartButton)
	{
		RestartButton->OnClicked.AddUniqueDynamic(this, &ULevelCompletionWidget::RestartGame);
		RestartButton->OnPressed.AddUniqueDynamic(this, &ULevelCompletionWidget::RestartGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddUniqueDynamic(this, &ULevelCompletionWidget::QuitToMainMenu);
		QuitButton->OnPressed.AddUniqueDynamic(this, &ULevelCompletionWidget::QuitToMainMenu);
	}
}

void ULevelCompletionWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (RestartButton)
	{
		RestartButton->RemoveFromParent();
		RestartButton = nullptr;
	}

	if (QuitButton)
	{
		QuitButton->RemoveFromParent();
		QuitButton = nullptr;
	}

	if (LevelCompletedTitleText)
	{
		LevelCompletedTitleText->RemoveFromParent();
		LevelCompletedTitleText = nullptr;
	}

	if (LevelCompletedCanvas)
	{
		LevelCompletedCanvas->RemoveFromParent();
		LevelCompletedCanvas = nullptr;
	}
}

void ULevelCompletionWidget::RestartGame()
{
	RemoveBindedButtonDelegates();

	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->DisableMouseCursor();

		gameMode->PopWidget(gameMode->GetLevelCompletionClass());
		gameMode->PushWidget(gameMode->GetPlayerHUDClass());

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->ResetPlayerValues();
			player->UpdatePlayerHUD();
		}

		gameMode->LoadPlayingLevel();

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

void ULevelCompletionWidget::QuitToMainMenu()
{
	RemoveBindedButtonDelegates();

	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->ResetPlayerValues();
		}

		gameMode->PopWidget(gameMode->GetLevelCompletionClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());
	}
}

void ULevelCompletionWidget::RemoveBindedButtonDelegates()
{
	if (RestartButton)
	{
		RestartButton->OnClicked.RemoveDynamic(this, &ULevelCompletionWidget::RestartGame);
		RestartButton->OnPressed.RemoveDynamic(this, &ULevelCompletionWidget::RestartGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.RemoveDynamic(this, &ULevelCompletionWidget::QuitToMainMenu);
		QuitButton->OnPressed.RemoveDynamic(this, &ULevelCompletionWidget::QuitToMainMenu);
	}
}
