// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MobileWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Player/PlayerCharacter.h"
#include "GameMode/BrackeysGM.h"
#include "Kismet/GameplayStatics.h"

UMobileWidget::UMobileWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMobileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MobileCanvas && MobileCanvas->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		MobileCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (InteractWithFishButton && InteractWithFishButton->GetVisibility() != ESlateVisibility::Collapsed)
	{
		InteractWithFishButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (PauseGameButton && PauseGameButton->GetVisibility() != ESlateVisibility::Visible)
	{
		PauseGameButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (InteractWithFishButton) InteractWithFishButton->OnPressed.AddUniqueDynamic(this, &UMobileWidget::InteractWithFish);
	if (PauseGameButton) PauseGameButton->OnPressed.AddUniqueDynamic(this, &UMobileWidget::PauseGame);

	FTimerHandle interactWithFishHandle;
	GetWorld()->GetTimerManager().SetTimer(interactWithFishHandle, this,
		&UMobileWidget::UpdateInteractWithFishButtonVisibility, GetWorld()->GetDeltaSeconds(), true);
}

void UMobileWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (PauseGameButton)
	{
		if (PauseGameButton->OnPressed.IsBound()) PauseGameButton->OnPressed.RemoveDynamic(this, &UMobileWidget::PauseGame);

		PauseGameButton->RemoveFromParent();
		PauseGameButton = nullptr;
	}

	if (InteractWithFishButton)
	{
		if (InteractWithFishButton->OnPressed.IsBound())
		{
			InteractWithFishButton->OnPressed.RemoveDynamic(this, &UMobileWidget::InteractWithFish);
		}

		InteractWithFishButton->RemoveFromParent();
		InteractWithFishButton = nullptr;
	}

	if (MobileCanvas)
	{
		MobileCanvas->RemoveFromParent();
		MobileCanvas = nullptr;
	}
}

void UMobileWidget::UpdateInteractWithFishButtonVisibility()
{
	if (InteractWithFishButton)
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			if (player->GetInteractableFish() && InteractWithFishButton->GetVisibility() != ESlateVisibility::Visible)
			{
				InteractWithFishButton->SetVisibility(ESlateVisibility::Visible);
			}

			else if (!player->GetInteractableFish() && InteractWithFishButton->GetVisibility() != ESlateVisibility::Collapsed)
			{
				InteractWithFishButton->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UMobileWidget::InteractWithFish()
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			player->InteractWithFish();
			gameMode->PopWidget(gameMode->GetMobileClass());
			gameMode->PushWidget(gameMode->GetRhythmMinigameMobileClass());
			gameMode->InvalidateTouchInterface();
		}
	}
}

void UMobileWidget::PauseGame()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->PopWidget(gameMode->GetMobileClass());
			gameMode->PushWidget(gameMode->GetPauseMenuClass());

			gameMode->InvalidateTouchInterface();
		}
	}
}
