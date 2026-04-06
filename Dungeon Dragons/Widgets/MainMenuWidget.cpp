// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/DungeonCrawlerGM.h"
#include "Player/PlayerCharacter.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (MainMenuCanvas && MainMenuCanvas->GetVisibility() != ESlateVisibility::Visible)
	{
		MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);
	}

	if (MainMenuTitleText && MainMenuTitleText->GetVisibility() != ESlateVisibility::Visible)
	{
		MainMenuTitleText->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayButton && PlayButton->GetVisibility() != ESlateVisibility::Visible)
	{
		PlayButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (QuitButton && QuitButton->GetVisibility() != ESlateVisibility::Visible)
	{
		QuitButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayButton)
	{
		PlayButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::PlayGame);
		PlayButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::PlayGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::QuitGame);
		QuitButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (PlayButton)
	{
		PlayButton->RemoveFromParent();
		PlayButton = nullptr;
	}

	if (QuitButton)
	{
		QuitButton->RemoveFromParent();
		QuitButton = nullptr;
	}

	if (MainMenuTitleText)
	{
		MainMenuTitleText->RemoveFromParent();
		MainMenuTitleText = nullptr;
	}

	if (MainMenuCanvas)
	{
		MainMenuCanvas->RemoveFromParent();
		MainMenuCanvas = nullptr;
	}
}

void UMainMenuWidget::PlayGame()
{
	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->DisableMouseCursor();

		gameMode->PopWidget(gameMode->GetMainMenuClass());
		gameMode->PushWidget(gameMode->GetPlayerHUDClass());

		gameMode->LoadPlayingLevel();

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->UpdatePlayerHUD();
		}

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}

	RemoveBindedButtonDelegates();
}

void UMainMenuWidget::QuitGame()
{
	// Quit the game
	UKismetSystemLibrary::QuitGame(nullptr, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);

	RemoveBindedButtonDelegates();
}

void UMainMenuWidget::RemoveBindedButtonDelegates()
{
	if (PlayButton)
	{
		PlayButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::PlayGame);
		PlayButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::PlayGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
		QuitButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}