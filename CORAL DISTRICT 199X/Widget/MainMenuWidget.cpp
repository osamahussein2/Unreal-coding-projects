// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/BrackeysGM.h"
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

	if (BlackImage && BlackImage->GetVisibility() != ESlateVisibility::Visible)
	{
		BlackImage->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayButton && PlayButton->GetVisibility() != ESlateVisibility::Visible)
	{
		PlayButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (OptionsButton && OptionsButton->GetVisibility() != ESlateVisibility::Visible)
	{
		OptionsButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (CreditsButton && CreditsButton->GetVisibility() != ESlateVisibility::Visible)
	{
		CreditsButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (QuitButton && QuitButton->GetVisibility() != ESlateVisibility::Visible)
	{
		QuitButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayGame);
		PlayButton->OnPressed.AddDynamic(this, &UMainMenuWidget::PlayGame);
	}

	if (OptionsButton)
	{
		OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToOptionsMenu);
		OptionsButton->OnPressed.AddDynamic(this, &UMainMenuWidget::GoToOptionsMenu);
	}

	if (CreditsButton)
	{
		CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
		CreditsButton->OnPressed.AddDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
		QuitButton->OnPressed.AddDynamic(this, &UMainMenuWidget::QuitGame);
	}

	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PlayMenuOpenSound();
		gameMode->PlayMainMenuMusic();
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

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (MainMenuCanvas)
	{
		MainMenuCanvas->RemoveFromParent();
		MainMenuCanvas = nullptr;
	}
}

void UMainMenuWidget::PlayGame()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), false);
		
		if (!gameMode->GetIsForMobile()) gameMode->DisableMouseCursor();

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->SetGameVolumes();
		}

		gameMode->PopWidget(gameMode->GetMainMenuClass());
		gameMode->PushWidget(gameMode->GetMobileClass());

		gameMode->ShowGameTouchInterface();

		gameMode->PlayMenuCloseSound();
		gameMode->PlayMallAmbienceMusic();
	}

	RemoveBindedButtonDelegates();
}

void UMainMenuWidget::GoToOptionsMenu()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetMainMenuClass());
		gameMode->PushWidget(gameMode->GetMainOptionsMenuClass());
	}

	RemoveBindedButtonDelegates();
}

void UMainMenuWidget::GoToCreditsMenu()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetMainMenuClass());
		gameMode->PushWidget(gameMode->GetCreditsMenuClass());
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

	if (OptionsButton)
	{
		OptionsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToOptionsMenu);
		OptionsButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::GoToOptionsMenu);
	}

	if (CreditsButton)
	{
		CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
		CreditsButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
		QuitButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}