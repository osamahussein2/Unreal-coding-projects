// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Animation/WidgetAnimation.h"
#include "Player/PlayerCharacter.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PauseMenuCanvas && !PauseMenuCanvas->IsVisible()) PauseMenuCanvas->SetVisibility(ESlateVisibility::Visible);
	if (PauseTitleText && !PauseTitleText->IsVisible()) PauseTitleText->SetVisibility(ESlateVisibility::Visible);
	if (BlackImage && !BlackImage->IsVisible()) BlackImage->SetVisibility(ESlateVisibility::Visible);
	if (ResumeButton && !ResumeButton) ResumeButton->SetVisibility(ESlateVisibility::Visible);
	if (QuitButton && !QuitButton) QuitButton->SetVisibility(ESlateVisibility::Visible);

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
		ResumeButton->OnPressed.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitGame);
		QuitButton->OnPressed.AddDynamic(this, &UPauseMenuWidget::QuitGame);
	}
}

void UPauseMenuWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	switch (menuTransitionState)
	{
	case PauseMenuTransitionState::EResume:
		Super::NativeTick(Geometry, DeltaTime);

		if (!IsAnimationPlaying(ResumeBurgerFlipAnimation))
		{
			if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				gameMode->DestroyPauseMenuWidget();
			}
		}

		break;

	case PauseMenuTransitionState::EQuit:
		Super::NativeTick(Geometry, DeltaTime);

		if (!IsAnimationPlaying(QuitBurgerFlipAnimation))
		{
			if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
				{
					player->DestroyAnyComponentReferences();
				}

				gameMode->InitializeMainMenuWidget();
				gameMode->DestroyPauseMenuWidget(true);
			}
		}

		break;

	default:
		break;
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

	if (PauseTitleText)
	{
		PauseTitleText->RemoveFromParent();
		PauseTitleText = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (PauseMenuCanvas)
	{
		PauseMenuCanvas->RemoveFromParent();
		PauseMenuCanvas = nullptr;
	}

	menuTransitionState = PauseMenuTransitionState::ENull;
}

void UPauseMenuWidget::ResumeGame()
{
	if (ResumeBurgerFlipAnimation) PlayAnimation(ResumeBurgerFlipAnimation);

	menuTransitionState = PauseMenuTransitionState::EResume;

	RemoveBindedButtonDelegates();
}

void UPauseMenuWidget::QuitGame()
{
	if (QuitBurgerFlipAnimation) PlayAnimation(QuitBurgerFlipAnimation);

	menuTransitionState = PauseMenuTransitionState::EQuit;

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
		QuitButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::QuitGame);
		QuitButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::QuitGame);
	}
}
