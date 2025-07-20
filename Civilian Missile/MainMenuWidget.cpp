// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Set up button OnClicked logic using AddDynamic delegate method
    playButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayLevel);
    quitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
}

void UMainMenuWidget::PlayLevel()
{
    // Open the game's level
    UGameplayStatics::OpenLevel(this, "GameLevel");
}

void UMainMenuWidget::QuitGame()
{
    // Quit the game
    UKismetSystemLibrary::QuitGame(nullptr, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
