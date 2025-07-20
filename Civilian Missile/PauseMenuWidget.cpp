// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuWidget.h"
#include "PlayerCharacter.h"
#include "PauseMenuActor.h"

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
    QuitToMainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitToMainMenu);
}

void UPauseMenuWidget::ResumeGame()
{
    // Set game paused bool to false and hide the pause menu widget
    if (APauseMenuActor::pauseStateChanged != false) APauseMenuActor::pauseStateChanged = false;

    APlayerCharacter::SetGamePaused(false);
    RemoveFromParent();
}

void UPauseMenuWidget::QuitToMainMenu()
{
    // Open the main menu level
    UGameplayStatics::OpenLevel(this, "MainMenuLevel");
}