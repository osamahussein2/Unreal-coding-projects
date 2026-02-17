// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PM_QuitConfirmationWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/GoedWareCommunityGM.h"

UPM_QuitConfirmationWidget::UPM_QuitConfirmationWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPM_QuitConfirmationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PM_QuitConfirmationCanvas && !PM_QuitConfirmationCanvas->IsVisible()) PM_QuitConfirmationCanvas->SetVisibility(ESlateVisibility::Visible);
	if (PM_QuitConfirmationText && !PM_QuitConfirmationText->IsVisible()) PM_QuitConfirmationText->SetVisibility(ESlateVisibility::Visible);
	if (SemiTransparentImage && !SemiTransparentImage->IsVisible()) SemiTransparentImage->SetVisibility(ESlateVisibility::Visible);
	if (YesButton && !YesButton) YesButton->SetVisibility(ESlateVisibility::Visible);
	if (NoButton && !NoButton) NoButton->SetVisibility(ESlateVisibility::Visible);

	if (YesButton)
	{
		YesButton->OnClicked.AddDynamic(this, &UPM_QuitConfirmationWidget::GoBackToMainMenu);
		YesButton->OnPressed.AddDynamic(this, &UPM_QuitConfirmationWidget::GoBackToMainMenu);
	}

	if (NoButton)
	{
		NoButton->OnClicked.AddDynamic(this, &UPM_QuitConfirmationWidget::GoBackToPauseMenu);
		NoButton->OnPressed.AddDynamic(this, &UPM_QuitConfirmationWidget::GoBackToPauseMenu);
	}
}

/*void UPM_QuitConfirmationWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}*/

void UPM_QuitConfirmationWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (YesButton)
	{
		YesButton->RemoveFromParent();
		YesButton = nullptr;
	}

	if (NoButton)
	{
		NoButton->RemoveFromParent();
		NoButton = nullptr;
	}

	if (PM_QuitConfirmationText)
	{
		PM_QuitConfirmationText->RemoveFromParent();
		PM_QuitConfirmationText = nullptr;
	}

	if (SemiTransparentImage)
	{
		SemiTransparentImage->RemoveFromParent();
		SemiTransparentImage = nullptr;
	}

	if (PM_QuitConfirmationCanvas)
	{
		PM_QuitConfirmationCanvas->RemoveFromParent();
		PM_QuitConfirmationCanvas = nullptr;
	}
}

void UPM_QuitConfirmationWidget::GoBackToPauseMenu()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetPM_QuitConfirmationClass());
		gameMode->PushWidget(gameMode->GetPauseMenuClass());
	}

	RemoveBindedButtonDelegates();
}

void UPM_QuitConfirmationWidget::GoBackToMainMenu()
{
	RemoveBindedButtonDelegates();

	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		gameMode->DestroyBodyActor();

		gameMode->PopWidget(gameMode->GetPM_QuitConfirmationClass());
		gameMode->PopWidget(gameMode->GetNerveSequenceClass());
		gameMode->PopWidget(gameMode->GetSequenceStatsClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());

		gameMode->ResetGame();
	}

	RemoveBindedButtonDelegates();
}

void UPM_QuitConfirmationWidget::RemoveBindedButtonDelegates()
{
	if (YesButton)
	{
		YesButton->OnClicked.RemoveDynamic(this, &UPM_QuitConfirmationWidget::GoBackToMainMenu);
		YesButton->OnPressed.RemoveDynamic(this, &UPM_QuitConfirmationWidget::GoBackToMainMenu);
	}

	if (NoButton)
	{
		NoButton->OnClicked.RemoveDynamic(this, &UPM_QuitConfirmationWidget::GoBackToPauseMenu);
		NoButton->OnPressed.RemoveDynamic(this, &UPM_QuitConfirmationWidget::GoBackToPauseMenu);
	}
}