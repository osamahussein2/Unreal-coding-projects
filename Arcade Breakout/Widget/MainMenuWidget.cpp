// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MainMenuWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/WidgetManager.h"
#include "GameMode/ShroomJamGM.h"
#include "Animation/WidgetAnimation.h"
#include "Widget/CreditsWidget.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShowButtons) PlayAnimation(ShowButtons);

	if (PlayButton)
	{
		PlayButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::PlayGame);
	}

	if (CreditsButton)
	{
		CreditsButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::GoToCredits);
	}

	if (QuitButton)
	{
		QuitButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	if (ShowButtons && IsAnimationPlaying(ShowButtons))
	{
		buttonShowTimer += DeltaTime;
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

	if (MainMenuCanvas)
	{
		MainMenuCanvas->RemoveFromParent();
		MainMenuCanvas = nullptr;
	}
}

void UMainMenuWidget::PlayGame()
{
	RemoveBindedDelegates();

	if (ShowButtons)
	{
		if (IsAnimationPlaying(ShowButtons))
		{
			if (PlayPressedAnimation)
			{
				PlayAnimation(PlayPressedAnimation, ShowButtons->GetEndTime() - buttonShowTimer);
				StopAnimation(ShowButtons);

				EndDelegate.BindDynamic(this, &UMainMenuWidget::OnPlayAnimationFinished);
				BindToAnimationFinished(PlayPressedAnimation, EndDelegate);
			}
		}

		else
		{
			if (PlayPressedAnimation)
			{
				PlayAnimation(PlayPressedAnimation, 0.0f);

				EndDelegate.BindDynamic(this, &UMainMenuWidget::OnPlayAnimationFinished);
				BindToAnimationFinished(PlayPressedAnimation, EndDelegate);
			}
		}
	}
}

void UMainMenuWidget::GoToCredits()
{
	RemoveBindedDelegates();

	if (ShowButtons)
	{
		if (IsAnimationPlaying(ShowButtons))
		{
			if (CreditsPressedAnimation)
			{
				PlayAnimation(CreditsPressedAnimation, ShowButtons->GetEndTime() - buttonShowTimer);
				StopAnimation(ShowButtons);

				EndDelegate.BindDynamic(this, &UMainMenuWidget::OnCreditsAnimationFinished);
				BindToAnimationFinished(CreditsPressedAnimation, EndDelegate);
			}
		}

		else
		{
			if (CreditsPressedAnimation)
			{
				PlayAnimation(CreditsPressedAnimation, 0.0f);

				EndDelegate.BindDynamic(this, &UMainMenuWidget::OnCreditsAnimationFinished);
				BindToAnimationFinished(CreditsPressedAnimation, EndDelegate);
			}
		}
	}
}

void UMainMenuWidget::QuitGame()
{
	RemoveBindedDelegates();

	if (ShowButtons)
	{
		if (IsAnimationPlaying(ShowButtons))
		{
			if (QuitPressedAnimation)
			{
				PlayAnimation(QuitPressedAnimation, ShowButtons->GetEndTime() - buttonShowTimer);
				StopAnimation(ShowButtons);

				EndDelegate.BindDynamic(this, &UMainMenuWidget::OnQuitAnimationFinished);
				BindToAnimationFinished(QuitPressedAnimation, EndDelegate);
			}
		}

		else
		{
			if (QuitPressedAnimation)
			{
				PlayAnimation(QuitPressedAnimation, 0.0f);

				EndDelegate.BindDynamic(this, &UMainMenuWidget::OnQuitAnimationFinished);
				BindToAnimationFinished(QuitPressedAnimation, EndDelegate);
			}
		}
	}
}

void UMainMenuWidget::OnPlayAnimationFinished()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetMainMenuClass());

			if (AShroomJamGM* shroomJamGM = Cast<AShroomJamGM>(UGameplayStatics::GetGameMode(this)))
			{
				widgetManager->DisableMouseCursor();
				shroomJamGM->LoadPlayingLevel();
			}
		}
	}
}

void UMainMenuWidget::OnCreditsAnimationFinished()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetMainMenuClass());
			widgetManager->PushWidget(widgetManager->GetCreditsMenuClass());
		}
	}
}

void UMainMenuWidget::OnQuitAnimationFinished()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetMainMenuClass());
		}

		// Quit the game
		UKismetSystemLibrary::QuitGame(nullptr, playerController, EQuitPreference::Quit, true);
	}
}

void UMainMenuWidget::RemoveBindedDelegates()
{
	if (PlayButton)
	{
		if (PlayButton->OnPressed.IsBound())
		{
			PlayButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::PlayGame);
		}
	}

	if (CreditsButton)
	{
		if (CreditsButton->OnPressed.IsBound())
		{
			CreditsButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::GoToCredits);
		}
	}

	if (QuitButton)
	{
		if (QuitButton->OnPressed.IsBound())
		{
			QuitButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
		}
	}
}
