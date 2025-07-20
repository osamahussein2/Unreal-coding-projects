// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWinWidget.h"

void UGameWinWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnClicked.AddDynamic(this, &UGameWinWidget::RestartGame);
	QuitToMainMenuButton->OnClicked.AddDynamic(this, &UGameWinWidget::QuitToMainMenu);
}

void UGameWinWidget::RestartGame()
{
	// Open the game's level
	UGameplayStatics::OpenLevel(this, "GameLevel");
}

void UGameWinWidget::QuitToMainMenu()
{
	// Open the main menu level
	UGameplayStatics::OpenLevel(this, "MainMenuLevel");
}