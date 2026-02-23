// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PauseGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/EhGameJamGM.h"

UPauseGameWidget::UPauseGameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPauseGameWidget::NativeConstruct()
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
		ResumeButton->OnClicked.AddUniqueDynamic(this, &UPauseGameWidget::ResumeGame);
		ResumeButton->OnPressed.AddUniqueDynamic(this, &UPauseGameWidget::ResumeGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddUniqueDynamic(this, &UPauseGameWidget::QuitToMainMenu);
		QuitButton->OnPressed.AddUniqueDynamic(this, &UPauseGameWidget::QuitToMainMenu);
	}
}

void UPauseGameWidget::NativeDestruct()
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

void UPauseGameWidget::ResumeGame()
{
	if (AEhGameJamGM* gameMode = Cast<AEhGameJamGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		//if (!gameMode->GetIsForMobile())
		gameMode->DisableMouseCursor();

		gameMode->PopWidget(gameMode->GetPauseMenuClass());
		gameMode->ShowPlayerHUD();
		//gameMode->PushWidget(gameMode->GetMobileClass());

		//gameMode->ShowGameTouchInterface();
	}

	RemoveBindedButtonDelegates();
}

void UPauseGameWidget::QuitToMainMenu()
{
	if (AEhGameJamGM* gameMode = Cast<AEhGameJamGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		gameMode->PopWidget(gameMode->GetPauseMenuClass());
		gameMode->UnloadPlayingLevel();
		gameMode->PushWidget(gameMode->GetMainMenuClass());
	}

	RemoveBindedButtonDelegates();
}

void UPauseGameWidget::RemoveBindedButtonDelegates()
{
	if (ResumeButton)
	{
		ResumeButton->OnClicked.RemoveDynamic(this, &UPauseGameWidget::ResumeGame);
		ResumeButton->OnPressed.RemoveDynamic(this, &UPauseGameWidget::ResumeGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.RemoveDynamic(this, &UPauseGameWidget::QuitToMainMenu);
		QuitButton->OnPressed.RemoveDynamic(this, &UPauseGameWidget::QuitToMainMenu);
	}
}