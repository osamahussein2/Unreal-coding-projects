// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PlayerHUDWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "GameMode/GoedWareCommunityGM.h"
#include "Kismet/GameplayStatics.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerHUDCanvas && PlayerHUDCanvas->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		PlayerHUDCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (PauseGameButton && !PauseGameButton->IsVisible()) PauseGameButton->SetVisibility(ESlateVisibility::Visible);

	if (PauseGameButton)
	{
		PauseGameButton->OnClicked.AddDynamic(this, &UPlayerHUDWidget::PauseGame);
		PauseGameButton->OnPressed.AddDynamic(this, &UPlayerHUDWidget::PauseGame);
	}
}

void UPlayerHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (PauseGameButton)
	{
		PauseGameButton->RemoveFromParent();
		PauseGameButton = nullptr;
	}

	if (PlayerHUDCanvas)
	{
		PlayerHUDCanvas->RemoveFromParent();
		PlayerHUDCanvas = nullptr;
	}
}

void UPlayerHUDWidget::PauseGame()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetPlayerHUDClass());
		gameMode->PopWidget(gameMode->GetNerveSequenceClass());
		gameMode->PopWidget(gameMode->GetPickThreeNervesClass());
		gameMode->PopWidget(gameMode->GetPickFourNervesClass());
		gameMode->PopWidget(gameMode->GetPickFiveNervesClass());
		gameMode->PushWidget(gameMode->GetPauseMenuClass());
	}

	RemoveBindedButtonDelegates();
}

void UPlayerHUDWidget::RemoveBindedButtonDelegates()
{
	PauseGameButton->OnClicked.RemoveDynamic(this, &UPlayerHUDWidget::PauseGame);
	PauseGameButton->OnPressed.RemoveDynamic(this, &UPlayerHUDWidget::PauseGame);
}
