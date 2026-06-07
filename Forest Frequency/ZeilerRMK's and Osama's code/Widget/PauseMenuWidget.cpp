// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PauseMenuWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager/WidgetManager.h"
#include "Widget/MainMenuWidget.h"
#include "Animation/WidgetAnimation.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (ResumeButton)
	{
		ResumeButton->OnHovered.AddUniqueDynamic(this, &UPauseMenuWidget::HoverOnResume);
		ResumeButton->OnPressed.AddUniqueDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (QuitToMainMenuButton)
	{
		QuitToMainMenuButton->OnHovered.AddUniqueDynamic(this, &UPauseMenuWidget::HoverOnMainMenu);
		QuitToMainMenuButton->OnPressed.AddUniqueDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
	}
}

void UPauseMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	switch (pressedButtonAnimation)
	{
	case PressedButtonAnimation::ResumeAnimation:

		if (PressResumeAnimation)
		{
			animationTimer += InDeltaTime;

			if (animationTimer >= PressResumeAnimation->GetEndTime())
			{
				TransitionToResume();
			}
		}

		break;

	case PressedButtonAnimation::QuitToMainMenu:

		if (PressMainMenuAnimation)
		{
			animationTimer += InDeltaTime;

			if (animationTimer >= PressMainMenuAnimation->GetEndTime())
			{
				TransitionToMainMenu();
			}
		}

		break;
	}
}

void UPauseMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	RemoveBindedDelegates();

	pressedButtonAnimation = PressedButtonAnimation::Null;

	UGameplayStatics::SetGamePaused(GetWorld(), false);

	if (QuitToMainMenuButton)
	{
		QuitToMainMenuButton->RemoveFromParent();
		QuitToMainMenuButton = nullptr;
	}

	if (ResumeButton)
	{
		ResumeButton->RemoveFromParent();
		ResumeButton = nullptr;
	}

	if (HookImage)
	{
		HookImage->RemoveFromParent();
		HookImage = nullptr;
	}

	if (OverlayedImage)
	{
		OverlayedImage->RemoveFromParent();
		OverlayedImage = nullptr;
	}

	if (PauseMenuCanvas)
	{
		PauseMenuCanvas->RemoveFromParent();
		PauseMenuCanvas = nullptr;
	}
}

void UPauseMenuWidget::PlayResumeAnimation()
{
	HoverOnResume();
	RemoveBindedDelegates();

	if (!PressResumeAnimation) return;

	PlayAnimation(PressResumeAnimation);

	pressedButtonAnimation = PressedButtonAnimation::ResumeAnimation;
}

void UPauseMenuWidget::HoverOnResume()
{
	if (UCanvasPanelSlot* hookImageSlot = Cast<UCanvasPanelSlot>(HookImage->Slot))
	{
		if (!ResumeButton) return;

		if (UCanvasPanelSlot* creditsButtonSlot = Cast<UCanvasPanelSlot>(ResumeButton->Slot))
		{
			hookImageSlot->SetPosition(FVector2D(hookImageSlot->GetPosition().X, creditsButtonSlot->GetPosition().Y));
		}
	}
}

void UPauseMenuWidget::HoverOnMainMenu()
{
	if (UCanvasPanelSlot* hookImageSlot = Cast<UCanvasPanelSlot>(HookImage->Slot))
	{
		if (!QuitToMainMenuButton) return;

		if (UCanvasPanelSlot* mainMenuButtonSlot = Cast<UCanvasPanelSlot>(QuitToMainMenuButton->Slot))
		{
			hookImageSlot->SetPosition(FVector2D(hookImageSlot->GetPosition().X, mainMenuButtonSlot->GetPosition().Y));
		}
	}
}

void UPauseMenuWidget::ResumeGame()
{
	PlayResumeAnimation();
}

void UPauseMenuWidget::QuitToMainMenu()
{
	RemoveBindedDelegates();

	if (!PressMainMenuAnimation) return;

	PlayAnimation(PressMainMenuAnimation);

	pressedButtonAnimation = PressedButtonAnimation::QuitToMainMenu;
}

void UPauseMenuWidget::RemoveBindedDelegates()
{
	if (ResumeButton && ResumeButton->OnHovered.IsBound())
	{
		ResumeButton->OnHovered.RemoveDynamic(this, &UPauseMenuWidget::HoverOnResume);
	}

	if (QuitToMainMenuButton && QuitToMainMenuButton->OnHovered.IsBound())
	{
		QuitToMainMenuButton->OnHovered.RemoveDynamic(this, &UPauseMenuWidget::HoverOnMainMenu);
	}

	if (ResumeButton && ResumeButton->OnPressed.IsBound())
	{
		ResumeButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (QuitToMainMenuButton && QuitToMainMenuButton->OnPressed.IsBound())
	{
		QuitToMainMenuButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
	}
}

void UPauseMenuWidget::TransitionToResume()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
	if (!widgetManager) return;

	widgetManager->DisableMouseCursor();
	widgetManager->PopWidget(widgetManager->GetPauseMenuClass());
}

void UPauseMenuWidget::TransitionToMainMenu()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
	if (!widgetManager) return;

	widgetManager->PopWidget(widgetManager->GetPauseMenuClass());
	widgetManager->PushWidget(widgetManager->GetMainMenuClass());

	if (!menuLevel.IsNull()) UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), menuLevel);
}