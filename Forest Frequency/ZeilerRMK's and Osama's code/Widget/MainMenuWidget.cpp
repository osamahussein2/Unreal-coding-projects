// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MainMenuWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager/WidgetManager.h"
#include "Widget/ControlsWidget.h"
#include "Widget/CreditsWidget.h"
#include "Animation/WidgetAnimation.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayButton)
	{
		PlayButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::HoverOnPlay);
		PlayButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::PlayGame);
	}

	if (ControlsButton)
	{
		ControlsButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::HoverOnControls);
		ControlsButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::GoToControls);
	}

	if (CreditsButton)
	{
		CreditsButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::HoverOnCredits);
		CreditsButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::GoToCredits);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::HoverOnQuit);
		QuitButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (QuitButton)
	{
		QuitButton->RemoveFromParent();
		QuitButton = nullptr;
	}

	if (CreditsButton)
	{
		CreditsButton->RemoveFromParent();
		CreditsButton = nullptr;
	}

	if (ControlsButton)
	{
		ControlsButton->RemoveFromParent();
		ControlsButton = nullptr;
	}

	if (PlayButton)
	{
		PlayButton->RemoveFromParent();
		PlayButton = nullptr;
	}

	if (HookImage)
	{
		HookImage->RemoveFromParent();
		HookImage = nullptr;
	}

	if (MainMenuCanvas)
	{
		MainMenuCanvas->RemoveFromParent();
		MainMenuCanvas = nullptr;
	}
}

void UMainMenuWidget::HoverOnPlay()
{
	if (UCanvasPanelSlot* hookImageSlot = Cast<UCanvasPanelSlot>(HookImage->Slot))
	{
		if (!PlayButton) return;

		if (UCanvasPanelSlot* playButtonSlot = Cast<UCanvasPanelSlot>(PlayButton->Slot))
		{
			hookImageSlot->SetPosition(FVector2D(hookImageSlot->GetPosition().X, 
				playButtonSlot->GetPosition().Y + hookImageOffsetY));
		}
	}
}

void UMainMenuWidget::HoverOnControls()
{
	if (UCanvasPanelSlot* hookImageSlot = Cast<UCanvasPanelSlot>(HookImage->Slot))
	{
		if (!ControlsButton) return;

		if (UCanvasPanelSlot* creditsButtonSlot = Cast<UCanvasPanelSlot>(ControlsButton->Slot))
		{
			hookImageSlot->SetPosition(FVector2D(hookImageSlot->GetPosition().X,
				creditsButtonSlot->GetPosition().Y + hookImageOffsetY));
		}
	}
}

void UMainMenuWidget::HoverOnCredits()
{
	if (UCanvasPanelSlot* hookImageSlot = Cast<UCanvasPanelSlot>(HookImage->Slot))
	{
		if (!CreditsButton) return;

		if (UCanvasPanelSlot* controlsButtonSlot = Cast<UCanvasPanelSlot>(CreditsButton->Slot))
		{
			hookImageSlot->SetPosition(FVector2D(hookImageSlot->GetPosition().X,
				controlsButtonSlot->GetPosition().Y + hookImageOffsetY));
		}
	}
}

void UMainMenuWidget::HoverOnQuit()
{
	if (UCanvasPanelSlot* hookImageSlot = Cast<UCanvasPanelSlot>(HookImage->Slot))
	{
		if (!QuitButton) return;

		if (UCanvasPanelSlot* quitButtonSlot = Cast<UCanvasPanelSlot>(QuitButton->Slot))
		{
			hookImageSlot->SetPosition(FVector2D(hookImageSlot->GetPosition().X,
				quitButtonSlot->GetPosition().Y + hookImageOffsetY));
		}
	}
}

void UMainMenuWidget::PlayGame()
{
	RemoveBindedDelegates();

	if (!PressPlayAnimation) return;

	PlayAnimation(PressPlayAnimation);

	FTimerHandle animationHandle{};
	GetWorld()->GetTimerManager().SetTimer(animationHandle, this, &UMainMenuWidget::TransitionToPlayGame,
		PressPlayAnimation->GetEndTime(), false);
}

void UMainMenuWidget::GoToControls()
{
	RemoveBindedDelegates();

	if (!PressControlsAnimation) return;

	PlayAnimation(PressControlsAnimation);

	FTimerHandle animationHandle{};
	GetWorld()->GetTimerManager().SetTimer(animationHandle, this, &UMainMenuWidget::TransitionToControls,
		PressControlsAnimation->GetEndTime(), false);
}

void UMainMenuWidget::GoToCredits()
{
	RemoveBindedDelegates();

	if (!PressCreditsAnimation) return;

	PlayAnimation(PressCreditsAnimation);

	FTimerHandle animationHandle{};
	GetWorld()->GetTimerManager().SetTimer(animationHandle, this, &UMainMenuWidget::TransitionToCredits,
		PressCreditsAnimation->GetEndTime(), false);
}

void UMainMenuWidget::QuitGame()
{
	RemoveBindedDelegates();

	if (!PressQuitAnimation) return;

	PlayAnimation(PressQuitAnimation);

	FTimerHandle animationHandle{};
	GetWorld()->GetTimerManager().SetTimer(animationHandle, this, &UMainMenuWidget::TransitionToQuitGame,
		PressQuitAnimation->GetEndTime(), false);
}

void UMainMenuWidget::RemoveBindedDelegates()
{
	if (PlayButton)
	{
		PlayButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnPlay);
		PlayButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::PlayGame);
	}

	if (ControlsButton)
	{
		ControlsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnControls);
		ControlsButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::GoToControls);
	}

	if (CreditsButton)
	{
		CreditsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnCredits);
		CreditsButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::GoToCredits);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnQuit);
		QuitButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::TransitionToPlayGame()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
	if (!widgetManager) return;

	widgetManager->DisableMouseCursor();
	widgetManager->PopWidget(widgetManager->GetMainMenuClass());

	if (!gameLevel.IsNull()) UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), gameLevel);
}

void UMainMenuWidget::TransitionToControls()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
	if (!widgetManager) return;

	widgetManager->PopWidget(widgetManager->GetMainMenuClass());
	widgetManager->PushWidget(widgetManager->GetControlsClass());
}

void UMainMenuWidget::TransitionToCredits()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
	if (!widgetManager) return;

	widgetManager->PopWidget(widgetManager->GetMainMenuClass());
	widgetManager->PushWidget(widgetManager->GetCreditsClass());
}

void UMainMenuWidget::TransitionToQuitGame()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
	if (!widgetManager) return;

	widgetManager->PopWidget(widgetManager->GetMainMenuClass());

	// Quit the game
	UKismetSystemLibrary::QuitGame(nullptr, playerController, EQuitPreference::Quit, true);
}