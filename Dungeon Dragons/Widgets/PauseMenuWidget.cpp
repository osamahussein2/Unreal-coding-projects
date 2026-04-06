// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/DungeonCrawlerGM.h"
#include "Player/PlayerCharacter.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (PauseMenuCanvas && !PauseMenuCanvas->IsVisible()) PauseMenuCanvas->SetVisibility(ESlateVisibility::Visible);
	if (PauseMenuTitleText && !PauseMenuTitleText->IsVisible()) PauseMenuTitleText->SetVisibility(ESlateVisibility::Visible);
	if (SemiTransparentImage && !SemiTransparentImage->IsVisible()) SemiTransparentImage->SetVisibility(ESlateVisibility::Visible);
	if (ResumeButton && !ResumeButton) ResumeButton->SetVisibility(ESlateVisibility::Visible);
	if (QuitButton && !QuitButton) QuitButton->SetVisibility(ESlateVisibility::Visible);

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::ResumeGame);
		ResumeButton->OnPressed.AddUniqueDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
		QuitButton->OnPressed.AddUniqueDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
	}
}

void UPauseMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (ResumeButton)
	{
		ResumeButton->RemoveFromParent();
		ResumeButton = nullptr;
	}

	if (QuitButton)
	{
		QuitButton->RemoveFromParent();
		QuitButton = nullptr;
	}

	if (PauseMenuTitleText)
	{
		PauseMenuTitleText->RemoveFromParent();
		PauseMenuTitleText = nullptr;
	}

	if (SemiTransparentImage)
	{
		SemiTransparentImage->RemoveFromParent();
		SemiTransparentImage = nullptr;
	}

	if (PauseMenuCanvas)
	{
		PauseMenuCanvas->RemoveFromParent();
		PauseMenuCanvas = nullptr;
	}
}

void UPauseMenuWidget::ResumeGame()
{
	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->DisableMouseCursor();

		gameMode->PopWidget(gameMode->GetPauseMenuClass());
		gameMode->PushWidget(gameMode->GetPlayerHUDClass());

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->UpdatePlayerHUD();
		}

		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}

	RemoveBindedButtonDelegates();
}

void UPauseMenuWidget::QuitToMainMenu()
{
	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetPauseMenuClass());
		gameMode->UnloadPlayingLevel();
		gameMode->PushWidget(gameMode->GetMainMenuClass());

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->ResetPlayerValues();
		}
	}

	RemoveBindedButtonDelegates();
}

void UPauseMenuWidget::RemoveBindedButtonDelegates()
{
	if (ResumeButton)
	{
		ResumeButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
		ResumeButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
		QuitButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
	}
}