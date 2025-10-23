// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CreditsWidget.h"

#include "Core/HJGameModeBase.h"

UCreditsWidget::UCreditsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowsPause = false;
	bShowMouseCursor = true;
	bHideOnPauseToggle = false;
	bPauseGameWhenShown = true;
}

void UCreditsWidget::OnRestartClicked()
{
	bIsEndGame = false;
	AHJGameModeBase* GM = Cast<AHJGameModeBase>(GetOwiningGameMode());
	GM->RestartGame();
}

void UCreditsWidget::FlagEndGameCredits()
{
	bIsEndGame = true;
}
