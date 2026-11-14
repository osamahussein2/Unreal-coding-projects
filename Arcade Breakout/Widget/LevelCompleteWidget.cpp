// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/LevelCompleteWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/WidgetManager.h"
#include "GameMode/ShroomJamGM.h"
#include "Widget/MainMenuWidget.h"
#include "Animation/WidgetAnimation.h"

ULevelCompleteWidget::ULevelCompleteWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{}

void ULevelCompleteWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShowButtons) PlayAnimation(ShowButtons);

	if (!UGameplayStatics::IsGamePaused(this)) UGameplayStatics::SetGamePaused(this, true);

	if (RestartButton)
	{
		RestartButton->OnPressed.AddUniqueDynamic(this, &ULevelCompleteWidget::RestartGame);
	}

	if (QuitButton)
	{
		QuitButton->OnPressed.AddUniqueDynamic(this, &ULevelCompleteWidget::QuitToMainMenu);
	}
}

void ULevelCompleteWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	if (ShowButtons && IsAnimationPlaying(ShowButtons))
	{
		buttonShowTimer += DeltaTime;
	}
}

void ULevelCompleteWidget::NativeDestruct()
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

	if (TransparentImage)
	{
		TransparentImage->RemoveFromParent();
		TransparentImage = nullptr;
	}

	if (LevelCompleteCanvas)
	{
		LevelCompleteCanvas->RemoveFromParent();
		LevelCompleteCanvas = nullptr;
	}
}

void ULevelCompleteWidget::RestartGame()
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

				EndDelegate.BindDynamic(this, &ULevelCompleteWidget::OnRestartAnimationFinished);
				BindToAnimationFinished(RestartPressedAnimation, EndDelegate);
			}
		}

		else
		{
			if (RestartPressedAnimation)
			{
				PlayAnimation(RestartPressedAnimation, 0.0f);

				EndDelegate.BindDynamic(this, &ULevelCompleteWidget::OnRestartAnimationFinished);
				BindToAnimationFinished(RestartPressedAnimation, EndDelegate);
			}
		}
	}
}

void ULevelCompleteWidget::QuitToMainMenu()
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

				EndDelegate.BindDynamic(this, &ULevelCompleteWidget::OnQuitAnimationFinished);
				BindToAnimationFinished(QuitPressedAnimation, EndDelegate);
			}
		}

		else
		{
			if (QuitPressedAnimation)
			{
				PlayAnimation(QuitPressedAnimation, 0.0f);

				EndDelegate.BindDynamic(this, &ULevelCompleteWidget::OnQuitAnimationFinished);
				BindToAnimationFinished(QuitPressedAnimation, EndDelegate);
			}
		}
	}
}

void ULevelCompleteWidget::OnRestartAnimationFinished()
{
	UGameplayStatics::SetGamePaused(this, false);

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetLevelCompleteClass());

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

void ULevelCompleteWidget::OnQuitAnimationFinished()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetLevelCompleteClass());
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

void ULevelCompleteWidget::RemoveBindedDelegates()
{
	if (RestartButton)
	{
		if (RestartButton->OnPressed.IsBound())
		{
			RestartButton->OnPressed.RemoveDynamic(this, &ULevelCompleteWidget::RestartGame);
		}
	}

	if (QuitButton)
	{
		if (QuitButton->OnPressed.IsBound())
		{
			QuitButton->OnPressed.RemoveDynamic(this, &ULevelCompleteWidget::QuitToMainMenu);
		}
	}
}