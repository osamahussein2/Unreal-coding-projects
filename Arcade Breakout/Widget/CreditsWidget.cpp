// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/CreditsWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/WidgetManager.h"
#include "GameMode/ShroomJamGM.h"
#include "Animation/WidgetAnimation.h"
#include "Widget/MainMenuWidget.h"

UCreditsWidget::UCreditsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShowButtons) PlayAnimation(ShowButtons);

	if (BackButton)
	{
		BackButton->OnPressed.AddUniqueDynamic(this, &UCreditsWidget::BackToMainMenu);
	}
}

void UCreditsWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	if (ShowButtons && IsAnimationPlaying(ShowButtons))
	{
		buttonShowTimer += DeltaTime;
	}
}

void UCreditsWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
	}

	if (CreditsMenuCanvas)
	{
		CreditsMenuCanvas->RemoveFromParent();
		CreditsMenuCanvas = nullptr;
	}
}

void UCreditsWidget::BackToMainMenu()
{
	RemoveBindedDelegates();

	if (ShowButtons)
	{
		if (IsAnimationPlaying(ShowButtons))
		{
			if (BackPressedAnimation)
			{
				PlayAnimation(BackPressedAnimation, ShowButtons->GetEndTime() - buttonShowTimer);
				StopAnimation(ShowButtons);

				EndDelegate.BindDynamic(this, &UCreditsWidget::OnBackAnimationFinished);
				BindToAnimationFinished(BackPressedAnimation, EndDelegate);
			}
		}

		else
		{
			if (BackPressedAnimation)
			{
				PlayAnimation(BackPressedAnimation, 0.0f);

				EndDelegate.BindDynamic(this, &UCreditsWidget::OnBackAnimationFinished);
				BindToAnimationFinished(BackPressedAnimation, EndDelegate);
			}
		}
	}
}

void UCreditsWidget::OnBackAnimationFinished()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetCreditsMenuClass());
			widgetManager->PushWidget(widgetManager->GetMainMenuClass());
		}
	}
}

void UCreditsWidget::RemoveBindedDelegates()
{
	if (BackButton)
	{
		if (BackButton->OnPressed.IsBound())
		{
			BackButton->OnPressed.RemoveDynamic(this, &UCreditsWidget::BackToMainMenu);
		}
	}
}
