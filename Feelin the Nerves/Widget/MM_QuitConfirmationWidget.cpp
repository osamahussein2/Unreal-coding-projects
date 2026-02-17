// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MM_QuitConfirmationWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/GoedWareCommunityGM.h"

UMM_QuitConfirmationWidget::UMM_QuitConfirmationWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMM_QuitConfirmationWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (MM_QuitConfirmationCanvas && !MM_QuitConfirmationCanvas->IsVisible()) MM_QuitConfirmationCanvas->SetVisibility(ESlateVisibility::Visible);
	if (MM_QuitConfirmationText && !MM_QuitConfirmationText->IsVisible()) MM_QuitConfirmationText->SetVisibility(ESlateVisibility::Visible);
	if (YesButton && !YesButton) YesButton->SetVisibility(ESlateVisibility::Visible);
	if (NoButton && !NoButton) NoButton->SetVisibility(ESlateVisibility::Visible);

	if (YesButton)
	{
		YesButton->OnClicked.AddDynamic(this, &UMM_QuitConfirmationWidget::QuitGame);
		YesButton->OnPressed.AddDynamic(this, &UMM_QuitConfirmationWidget::QuitGame);
	}

	if (NoButton)
	{
		NoButton->OnClicked.AddDynamic(this, &UMM_QuitConfirmationWidget::GoBackToMainMenu);
		NoButton->OnPressed.AddDynamic(this, &UMM_QuitConfirmationWidget::GoBackToMainMenu);
	}
}

/*void UMM_QuitConfirmationWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}*/

void UMM_QuitConfirmationWidget::NativeDestruct()
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

	if (MM_QuitConfirmationText)
	{
		MM_QuitConfirmationText->RemoveFromParent();
		MM_QuitConfirmationText = nullptr;
	}

	if (MM_QuitConfirmationCanvas)
	{
		MM_QuitConfirmationCanvas->RemoveFromParent();
		MM_QuitConfirmationCanvas = nullptr;
	}
}

void UMM_QuitConfirmationWidget::GoBackToMainMenu()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetMM_QuitConfirmationClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());
	}

	RemoveBindedButtonDelegates();
}

void UMM_QuitConfirmationWidget::QuitGame()
{
	RemoveBindedButtonDelegates();

	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// Get rid of main menu quit confirmation widget first before quitting
		gameMode->PopWidget(gameMode->GetMM_QuitConfirmationClass());
	}

	// Quit the game
	UKismetSystemLibrary::QuitGame(nullptr, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UMM_QuitConfirmationWidget::RemoveBindedButtonDelegates()
{
	if (YesButton)
	{
		YesButton->OnClicked.RemoveDynamic(this, &UMM_QuitConfirmationWidget::QuitGame);
		YesButton->OnPressed.RemoveDynamic(this, &UMM_QuitConfirmationWidget::QuitGame);
	}

	if (NoButton)
	{
		NoButton->OnClicked.RemoveDynamic(this, &UMM_QuitConfirmationWidget::GoBackToMainMenu);
		NoButton->OnPressed.RemoveDynamic(this, &UMM_QuitConfirmationWidget::GoBackToMainMenu);
	}
}