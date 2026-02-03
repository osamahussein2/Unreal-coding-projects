// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Animation/WidgetAnimation.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (MainMenuCanvas && !MainMenuCanvas->IsVisible()) MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);
	if (MainMenuTitleText && !MainMenuTitleText->IsVisible()) MainMenuTitleText->SetVisibility(ESlateVisibility::Visible);
	if (BlackImage && !BlackImage->IsVisible()) BlackImage->SetVisibility(ESlateVisibility::Visible);
	if (PlayButton && !PlayButton) PlayButton->SetVisibility(ESlateVisibility::Visible);
	if (QuitButton && !QuitButton) QuitButton->SetVisibility(ESlateVisibility::Visible);

	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayGame);
		PlayButton->OnPressed.AddDynamic(this, &UMainMenuWidget::PlayGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
		QuitButton->OnPressed.AddDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	switch (menuTransitionState)
	{
	case MainMenuTransitionState::EPlay:
		Super::NativeTick(Geometry, DeltaTime);

		if (!IsAnimationPlaying(PlayBurgerFlipAnimation))
		{
			if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				gameMode->InitializeCurrentDayWidget(gameMode->GetCurrentDay() <= 0 ? true : false);
				gameMode->DestroyMainMenuWidget();
			}
		}

		break;

	case MainMenuTransitionState::EQuit:
		Super::NativeTick(Geometry, DeltaTime);

		if (!IsAnimationPlaying(QuitBurgerFlipAnimation))
		{
			// Quit the game
			UKismetSystemLibrary::QuitGame(nullptr, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
		}

		break;

	default:
		break;
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

	menuTransitionState = MainMenuTransitionState::ENull;
}

void UMainMenuWidget::PlayGame()
{
	if (PlayBurgerFlipAnimation) PlayAnimation(PlayBurgerFlipAnimation);

	menuTransitionState = MainMenuTransitionState::EPlay;

	RemoveBindedButtonDelegates();
}

void UMainMenuWidget::QuitGame()
{
	if (QuitBurgerFlipAnimation) PlayAnimation(QuitBurgerFlipAnimation);

	menuTransitionState = MainMenuTransitionState::EQuit;

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
