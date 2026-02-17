// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/GoedWareCommunityGM.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (PauseMenuCanvas && PauseMenuCanvas->GetVisibility() != ESlateVisibility::Visible)
	{
		PauseMenuCanvas->SetVisibility(ESlateVisibility::Visible);
	}

	if (PauseMenuTitleText && PauseMenuTitleText->GetVisibility() != ESlateVisibility::Visible)
	{
		PauseMenuTitleText->SetVisibility(ESlateVisibility::Visible);
	}

	if (SemiTransparentImage && SemiTransparentImage->GetVisibility() != ESlateVisibility::Visible)
	{
		SemiTransparentImage->SetVisibility(ESlateVisibility::Visible);
	}

	if (ResumeButton && ResumeButton->GetVisibility() != ESlateVisibility::Visible)
	{
		ResumeButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (QuitButton && QuitButton->GetVisibility() != ESlateVisibility::Visible)
	{
		QuitButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (SaveButton && SaveButton->GetVisibility() != ESlateVisibility::Visible)
	{
		SaveButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (LoadButton && LoadButton->GetVisibility() != ESlateVisibility::Visible)
	{
		LoadButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
		ResumeButton->OnPressed.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (SaveButton)
	{
		SaveButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::GoToSaveGame);
		SaveButton->OnPressed.AddDynamic(this, &UPauseMenuWidget::GoToSaveGame);
	}

	if (LoadButton)
	{
		LoadButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::GoToLoadGame);
		LoadButton->OnPressed.AddDynamic(this, &UPauseMenuWidget::GoToLoadGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::GoToQuitConfirmationPM);
		QuitButton->OnPressed.AddDynamic(this, &UPauseMenuWidget::GoToQuitConfirmationPM);
	}
}

/*void UPauseMenuWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}*/

void UPauseMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

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

	if (PauseMenuTitleText)
	{
		PauseMenuTitleText->RemoveFromParent();
		PauseMenuTitleText = nullptr;
	}

	if (SemiTransparentImage)
	{
		SemiTransparentImage->RemoveFromParent();
		SemiTransparentImage = nullptr;
	}

	if (PauseMenuCanvas)
	{
		PauseMenuCanvas->RemoveFromParent();
		PauseMenuCanvas = nullptr;
	}
}

void UPauseMenuWidget::ResumeGame()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		gameMode->PopWidget(gameMode->GetPauseMenuClass());

		switch (gameMode->GetPickNerveState())
		{
		case EPickNerveState::EPickingThree:
			gameMode->PushWidget(gameMode->GetPlayerHUDClass());
			gameMode->PushWidget(gameMode->GetPickThreeNervesClass());
			break;

		case EPickNerveState::EPickingFour:
			gameMode->PushWidget(gameMode->GetPlayerHUDClass());
			gameMode->PushWidget(gameMode->GetPickFourNervesClass());
			break;

		case EPickNerveState::EPickingFive:
			gameMode->PushWidget(gameMode->GetPlayerHUDClass());
			gameMode->PushWidget(gameMode->GetPickFiveNervesClass());
			break;

		default:
			break;
		}
	}

	RemoveBindedButtonDelegates();
}

void UPauseMenuWidget::GoToLoadGame()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetPauseMenuClass());
		gameMode->PushWidget(gameMode->GetPM_LoadGameClass());
	}

	RemoveBindedButtonDelegates();
}

void UPauseMenuWidget::GoToSaveGame()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetPauseMenuClass());
		gameMode->PushWidget(gameMode->GetSaveGameClass());
	}

	RemoveBindedButtonDelegates();
}

void UPauseMenuWidget::GoToQuitConfirmationPM()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetPauseMenuClass());
		gameMode->PushWidget(gameMode->GetPM_QuitConfirmationClass());
	}

	RemoveBindedButtonDelegates();
}

void UPauseMenuWidget::RemoveBindedButtonDelegates()
{
	if (ResumeButton)
	{
		ResumeButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
		ResumeButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (SaveButton)
	{
		SaveButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::GoToSaveGame);
		SaveButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::GoToSaveGame);
	}

	if (LoadButton)
	{
		LoadButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::GoToLoadGame);
		LoadButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::GoToLoadGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::GoToQuitConfirmationPM);
		QuitButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::GoToQuitConfirmationPM);
	}
}