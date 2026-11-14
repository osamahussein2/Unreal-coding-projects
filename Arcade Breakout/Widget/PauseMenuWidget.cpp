// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PauseMenuWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/WidgetManager.h"
#include "GameMode/ShroomJamGM.h"
#include "Widget/MainMenuWidget.h"
#include "Widget/PlayerHUDWidget.h"
#include "Animation/WidgetAnimation.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShowButtons) PlayAnimation(ShowButtons);

	if (ResumeButton)
	{
		ResumeButton->OnPressed.AddUniqueDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (QuitButton)
	{
		QuitButton->OnPressed.AddUniqueDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
	}
}

void UPauseMenuWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	if (ShowButtons && IsAnimationPlaying(ShowButtons))
	{
		buttonShowTimer += DeltaTime;
	}
}

void UPauseMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (PauseMenuCanvas)
	{
		PauseMenuCanvas->RemoveFromParent();
		PauseMenuCanvas = nullptr;
	}

	if (TransparentImage)
	{
		TransparentImage->RemoveFromParent();
		TransparentImage = nullptr;
	}

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

	if (PauseMenuCanvas)
	{
		PauseMenuCanvas->RemoveFromParent();
		PauseMenuCanvas = nullptr;
	}
}

void UPauseMenuWidget::ResumeGame()
{
	RemoveBindedDelegates();

	if (ShowButtons)
	{
		if (IsAnimationPlaying(ShowButtons))
		{
			if (ResumePressedAnimation)
			{
				PlayAnimation(ResumePressedAnimation, ShowButtons->GetEndTime() - buttonShowTimer);
				StopAnimation(ShowButtons);

				EndDelegate.BindDynamic(this, &UPauseMenuWidget::OnResumeAnimationFinished);
				BindToAnimationFinished(ResumePressedAnimation, EndDelegate);
			}
		}

		else
		{
			if (ResumePressedAnimation)
			{
				PlayAnimation(ResumePressedAnimation, 0.0f);

				EndDelegate.BindDynamic(this, &UPauseMenuWidget::OnResumeAnimationFinished);
				BindToAnimationFinished(ResumePressedAnimation, EndDelegate);
			}
		}
	}
}

void UPauseMenuWidget::QuitToMainMenu()
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

				EndDelegate.BindDynamic(this, &UPauseMenuWidget::OnQuitAnimationFinished);
				BindToAnimationFinished(QuitPressedAnimation, EndDelegate);
			}
		}

		else
		{
			if (QuitPressedAnimation)
			{
				PlayAnimation(QuitPressedAnimation, 0.0f);

				EndDelegate.BindDynamic(this, &UPauseMenuWidget::OnQuitAnimationFinished);
				BindToAnimationFinished(QuitPressedAnimation, EndDelegate);
			}
		}
	}
}

void UPauseMenuWidget::OnResumeAnimationFinished()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetPauseMenuClass());
			widgetManager->DisableMouseCursor();

			UGameplayStatics::SetGamePaused(this, false);

			widgetManager->PushWidget(widgetManager->GetPlayerHUDClass());
		}
	}
}

void UPauseMenuWidget::OnQuitAnimationFinished()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetPauseMenuClass());
			widgetManager->PushWidget(widgetManager->GetMainMenuClass());

			widgetManager->EnableMouseCursor();

			UGameplayStatics::SetGamePaused(this, false);
		}

		if (AShroomJamGM* shroomJamGM = Cast<AShroomJamGM>(UGameplayStatics::GetGameMode(this)))
		{
			shroomJamGM->UnloadPlayingLevel();
		}
	}
}

void UPauseMenuWidget::RemoveBindedDelegates()
{
	if (ResumeButton)
	{
		if (ResumeButton->OnPressed.IsBound())
		{
			ResumeButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
		}
	}

	if (QuitButton)
	{
		if (QuitButton->OnPressed.IsBound())
		{
			QuitButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
		}
	}
}
