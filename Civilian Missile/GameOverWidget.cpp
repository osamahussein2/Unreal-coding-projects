// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverWidget.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnClicked.AddDynamic(this, &UGameOverWidget::RestartGame);
	QuitToMainMenuButton->OnClicked.AddDynamic(this, &UGameOverWidget::QuitToMainMenu);
}

void UGameOverWidget::RestartGame()
{
	// Open the game's level
	UGameplayStatics::OpenLevel(this, "GameLevel");
}

void UGameOverWidget::QuitToMainMenu()
{
	// Open the main menu level
	UGameplayStatics::OpenLevel(this, "MainMenuLevel");
}
