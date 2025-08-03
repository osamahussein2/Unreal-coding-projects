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
    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));

    APlayerCharacter::SetGamePaused(false);
    APauseMenuActor::pauseStateChanged = false;
}

void UPauseMenuWidget::QuitToMainMenu()
{
    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));

    // Open the main menu level
    UGameplayStatics::OpenLevel(this, "MainMenu");
}