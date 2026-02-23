// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/CreditsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "GameMode/BrackeysGM.h"
#include "Animation/WidgetAnimation.h"

UCreditsWidget::UCreditsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreditsMenuCanvas && CreditsMenuCanvas->GetVisibility() != ESlateVisibility::Visible)
	{
		CreditsMenuCanvas->SetVisibility(ESlateVisibility::Visible);
	}

	if (BlackImage && BlackImage->GetVisibility() != ESlateVisibility::Visible)
	{
		BlackImage->SetVisibility(ESlateVisibility::Visible);
	}

	if (CreditsOverlay && CreditsOverlay->GetVisibility() != ESlateVisibility::Visible)
	{
		CreditsOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton && BackButton->GetVisibility() != ESlateVisibility::Visible)
	{
		BackButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &UCreditsWidget::GoBackToMainMenu);
		BackButton->OnPressed.AddUniqueDynamic(this, &UCreditsWidget::GoBackToMainMenu);
	}

	if (ScrollCredits)
	{
		PlayAnimation(ScrollCredits);
	}

	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PlayCreditsMenuMusic();
	}
}

void UCreditsWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	// Play the scrolling credits animation again if it's not playing already
	if (ScrollCredits && !IsAnimationPlaying(ScrollCredits))
	{
		PlayAnimation(ScrollCredits);
	}
}

void UCreditsWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (CreditsMenuCanvas)
	{
		CreditsMenuCanvas->RemoveFromParent();
		CreditsMenuCanvas = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (CreditsOverlay)
	{
		CreditsOverlay->RemoveFromParent();
		CreditsOverlay = nullptr;
	}

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
	}

	if (ScrollCredits)
	{
		ScrollCredits->RemoveBinding(FWidgetAnimationBinding());
		ScrollCredits = nullptr;
	}
}

void UCreditsWidget::GoBackToMainMenu()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetCreditsMenuClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());

		gameMode->PlayMenuCloseSound();
	}

	RemoveBindedButtonDelegates();
}

void UCreditsWidget::RemoveBindedButtonDelegates()
{
	if (BackButton)
	{
		BackButton->OnClicked.RemoveDynamic(this, &UCreditsWidget::GoBackToMainMenu);
		BackButton->OnPressed.RemoveDynamic(this, &UCreditsWidget::GoBackToMainMenu);
	}
}

void UCreditsWidget::PlayCreditsScrollAnimation()
{
	UE_LOG(LogTemp, Warning, TEXT("Credits scrolling..."));
	if (ScrollCredits) PlayAnimation(ScrollCredits);
}
