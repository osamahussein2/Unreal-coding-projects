// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/GameOverWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/DungeonCrawlerGM.h"
#include "Player/PlayerCharacter.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (GameOverCanvas && !GameOverCanvas->IsVisible()) GameOverCanvas->SetVisibility(ESlateVisibility::Visible);
	if (GameOverTitleText && !GameOverTitleText->IsVisible()) GameOverTitleText->SetVisibility(ESlateVisibility::Visible);
	if (SemiTransparentImage && !SemiTransparentImage->IsVisible()) SemiTransparentImage->SetVisibility(ESlateVisibility::Visible);
	if (RestartButton && !RestartButton) RestartButton->SetVisibility(ESlateVisibility::Visible);
	if (QuitButton && !QuitButton) QuitButton->SetVisibility(ESlateVisibility::Visible);

	if (RestartButton)
	{
		RestartButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::StartRestartGameTimer);
		RestartButton->OnPressed.AddUniqueDynamic(this, &UGameOverWidget::StartRestartGameTimer);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::QuitToMainMenu);
		QuitButton->OnPressed.AddUniqueDynamic(this, &UGameOverWidget::QuitToMainMenu);
	}
}

void UGameOverWidget::NativeDestruct()
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

	if (GameOverTitleText)
	{
		GameOverTitleText->RemoveFromParent();
		GameOverTitleText = nullptr;
	}

	if (SemiTransparentImage)
	{
		SemiTransparentImage->RemoveFromParent();
		SemiTransparentImage = nullptr;
	}

	if (GameOverCanvas)
	{
		GameOverCanvas->RemoveFromParent();
		GameOverCanvas = nullptr;
	}

	restartPressed = false;
	restartTimer = 0.0f;
}

void UGameOverWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	if (restartPressed)
	{
		Super::NativeTick(Geometry, DeltaTime);

		restartTimer += DeltaTime;

		if (restartTimer >= timeToRestartGame) RestartGame();
	}
}

void UGameOverWidget::StartRestartGameTimer()
{
	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->UnloadPlayingLevel();

		restartPressed = true;
	}

	RemoveBindedButtonDelegates();

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

	if (GameOverTitleText)
	{
		GameOverTitleText->RemoveFromParent();
		GameOverTitleText = nullptr;
	}

	if (SemiTransparentImage)
	{
		SemiTransparentImage->RemoveFromParent();
		SemiTransparentImage = nullptr;
	}
}

void UGameOverWidget::QuitToMainMenu()
{
	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->UnloadPlayingLevel();

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->ResetPlayerValues();
		}

		gameMode->PopWidget(gameMode->GetGameOverClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());
	}

	RemoveBindedButtonDelegates();
}

void UGameOverWidget::RemoveBindedButtonDelegates()
{
	if (RestartButton)
	{
		RestartButton->OnClicked.RemoveDynamic(this, &UGameOverWidget::StartRestartGameTimer);
		RestartButton->OnPressed.RemoveDynamic(this, &UGameOverWidget::StartRestartGameTimer);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.RemoveDynamic(this, &UGameOverWidget::QuitToMainMenu);
		QuitButton->OnPressed.RemoveDynamic(this, &UGameOverWidget::QuitToMainMenu);
	}
}

void UGameOverWidget::RestartGame()
{
	if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->DisableMouseCursor();

		gameMode->PopWidget(gameMode->GetGameOverClass());
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
