// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/BrackeysGM.h"
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
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
		ResumeButton->OnPressed.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
		QuitButton->OnPressed.AddDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
	}

	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PlayMenuOpenSound();
		if (gameMode->GetMusicVolume() > 0.0f) gameMode->PauseMusic();
	}
}

/*void UPauseMenuWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}*/

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
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		
		if (!gameMode->GetIsForMobile()) gameMode->DisableMouseCursor();

		gameMode->PopWidget(gameMode->GetPauseMenuClass());

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->SetGameVolumes();

			if (player->IsInFreeRoam())
			{
				gameMode->PushWidget(gameMode->GetMobileClass());

				gameMode->ShowGameTouchInterface();
			}
		}

		gameMode->PlayMenuCloseSound();
		if (gameMode->GetMusicVolume() > 0.0f) gameMode->ResumeMusic();
	}

	RemoveBindedButtonDelegates();
}

void UPauseMenuWidget::QuitToMainMenu()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetRhythmMinigameClass());
		gameMode->PopWidget(gameMode->GetRhythmMinigameMobileClass());
		gameMode->PopWidget(gameMode->GetPauseMenuClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());

		gameMode->ResetGame();
		if (gameMode->GetMusicVolume() > 0.0f) gameMode->ResumeMusic();
		gameMode->PlayMainMenuMusic();
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