// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/GameOverWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/WidgetManager.h"
#include "GameMode/ShroomJamGM.h"
#include "Widget/MainMenuWidget.h"
#include "Widget/PlayerHUDWidget.h"
#include "Animation/WidgetAnimation.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShowButtons) PlayAnimation(ShowButtons);

	UGameplayStatics::SetGamePaused(this, true);

	if (RestartButton)
	{
		RestartButton->OnPressed.AddUniqueDynamic(this, &UGameOverWidget::RestartGame);
	}

	if (QuitButton)
	{
		QuitButton->OnPressed.AddUniqueDynamic(this, &UGameOverWidget::QuitToMainMenu);
	}
}

void UGameOverWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	if (ShowButtons && IsAnimationPlaying(ShowButtons))
	{
		buttonShowTimer += DeltaTime;
	}
}

void UGameOverWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (GameOverCanvas)
	{
		GameOverCanvas->RemoveFromParent();
		GameOverCanvas = nullptr;
	}

	if (TransparentImage)
	{
		TransparentImage->RemoveFromParent();
		TransparentImage = nullptr;
	}

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

	if (GameOverCanvas)
	{
		GameOverCanvas->RemoveFromParent();
		GameOverCanvas = nullptr;
	}
}

void UGameOverWidget::RestartGame()
{
	RemoveBindedDelegates();

	if (ShowButtons)
	{
		if (IsAnimationPlaying(ShowButtons))
		{
			if (RestartPressedAnimation)
			{
				PlayAnimation(RestartPressedAnimation, ShowButtons->GetEndTime() - buttonShowTimer);
				StopAnimation(ShowButtons);

				EndDelegate.BindDynamic(this, &UGameOverWidget::OnRestartAnimationFinished);
				BindToAnimationFinished(RestartPressedAnimation, EndDelegate);
			}
		}

		else
		{
			if (RestartPressedAnimation)
			{
				PlayAnimation(RestartPressedAnimation, 0.0f);

				EndDelegate.BindDynamic(this, &UGameOverWidget::OnRestartAnimationFinished);
				BindToAnimationFinished(RestartPressedAnimation, EndDelegate);
			}
		}
	}
}

void UGameOverWidget::QuitToMainMenu()
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

				EndDelegate.BindDynamic(this, &UGameOverWidget::OnQuitAnimationFinished);
				BindToAnimationFinished(QuitPressedAnimation, EndDelegate);
			}
		}

		else
		{
			if (QuitPressedAnimation)
			{
				PlayAnimation(QuitPressedAnimation, 0.0f);

				EndDelegate.BindDynamic(this, &UGameOverWidget::OnQuitAnimationFinished);
				BindToAnimationFinished(QuitPressedAnimation, EndDelegate);
			}
		}
	}
}

void UGameOverWidget::OnRestartAnimationFinished()
{
	UGameplayStatics::SetGamePaused(this, false);

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetGameOverClass());

			widgetManager->DisableMouseCursor();
		}
	}

	if (AShroomJamGM* shroomJamGM = Cast<AShroomJamGM>(UGameplayStatics::GetGameMode(this)))
	{
		shroomJamGM->UnloadPlayingLevel();

		if (GetWorld())
		{
			FTimerHandle timer{};
			GetWorld()->GetTimerManager().SetTimer(timer, [shroomJamGM]()
				{
					shroomJamGM->LoadPlayingLevel();
				}, 1.0f, false);
		}
	}
}

void UGameOverWidget::OnQuitAnimationFinished()
{
	UGameplayStatics::SetGamePaused(this, false);

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetGameOverClass());
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

void UGameOverWidget::RemoveBindedDelegates()
{
	if (RestartButton)
	{
		if (RestartButton->OnPressed.IsBound())
		{
			RestartButton->OnPressed.RemoveDynamic(this, &UGameOverWidget::RestartGame);
		}
	}

	if (QuitButton)
	{
		if (QuitButton->OnPressed.IsBound())
		{
			QuitButton->OnPressed.RemoveDynamic(this, &UGameOverWidget::QuitToMainMenu);
		}
	}
}
