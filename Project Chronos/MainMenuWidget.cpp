// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"
#include "PlayerCharacter.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Set up button OnClicked logic using AddDynamic delegate method
    PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayLevel);
    CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
    ControlsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToControlsMenu);
    ConceptArtButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToConceptArtMenu);
    QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);

    CreditsBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToBackToMainMenu);

    ControlsResetButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ResetControls);
    ControlsBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToBackToMainMenu);

    ConceptArtLeftButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LookAtPreviousConceptArt);
    ConceptArtRightButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LookAtNextConceptArt);
    ConceptArtBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToBackToMainMenu);

    // Set up modify controls OnClicked logic
    ChangeMoveForwardButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ChangeForwardKey);
    ChangeMoveBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ChangeBackKey);
    ChangeMoveLeftButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ChangeLeftKey);
    ChangeMoveRightButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ChangeRightKey);

    ChangePauseButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ChangePauseKeyLogic);
    ChangeAttackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ChangeAttackKeyLogic);

    // Set visibility of canvas panels
    MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);
    CreditsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    ControlsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    ConceptArtMenuCanvas->SetVisibility(ESlateVisibility::Hidden);

    currentImageIndex = 0;

    imageNumber = currentImageIndex + 1;
    maxImages = conceptArtImages.Num();

    if (ConceptArtImage) ConceptArtImage->SetBrushFromTexture(conceptArtImages[currentImageIndex]);

    if (currentImageText) currentImageText->SetText(FText::FromString(FString::FromInt(imageNumber) + "/" +
        FString::FromInt(maxImages)));
}

void UMainMenuWidget::PlayLevel()
{
    // Open the game's level
    UGameplayStatics::OpenLevel(this, "GameLevel");

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::GoToCreditsMenu()
{
    MainMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    ControlsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    ConceptArtMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    CreditsMenuCanvas->SetVisibility(ESlateVisibility::Visible);

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::GoToControlsMenu()
{
    MainMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    CreditsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    ConceptArtMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    ControlsMenuCanvas->SetVisibility(ESlateVisibility::Visible);

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::GoToConceptArtMenu()
{
    MainMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    CreditsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    ControlsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    ConceptArtMenuCanvas->SetVisibility(ESlateVisibility::Visible);

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::QuitGame()
{
    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));

    // Quit the game
    UKismetSystemLibrary::QuitGame(nullptr, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UMainMenuWidget::GoToBackToMainMenu()
{
    if (!APlayerCharacter::modifyMoveLeftKey && !APlayerCharacter::modifyMoveRightKey && 
        !APlayerCharacter::modifyMoveForwardKey && !APlayerCharacter::modifyMoveBackKey &&
        !APlayerCharacter::modifyPauseKey && !APlayerCharacter::modifyAttackKey)
    {
        MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);
        CreditsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
        ControlsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
        ConceptArtMenuCanvas->SetVisibility(ESlateVisibility::Hidden);

        UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
    }
}

void UMainMenuWidget::ResetControls()
{
    if (!APlayerCharacter::modifyMoveLeftKey && !APlayerCharacter::modifyMoveRightKey &&
        !APlayerCharacter::modifyMoveForwardKey && !APlayerCharacter::modifyMoveBackKey &&
        !APlayerCharacter::modifyPauseKey && !APlayerCharacter::modifyAttackKey)
    {
        // Reset controls back to default
        if (APlayerCharacter::currentMoveBackKey != EKeys::S) APlayerCharacter::currentMoveBackKey = EKeys::S;
        if (APlayerCharacter::currentMoveForwardKey != EKeys::W) APlayerCharacter::currentMoveForwardKey = EKeys::W;
        if (APlayerCharacter::currentMoveLeftKey != EKeys::A) APlayerCharacter::currentMoveLeftKey = EKeys::A;
        if (APlayerCharacter::currentMoveRightKey != EKeys::D) APlayerCharacter::currentMoveRightKey = EKeys::D;

        if (APlayerCharacter::currentPauseKey != EKeys::Escape) APlayerCharacter::currentPauseKey = EKeys::Escape;
        if (APlayerCharacter::currentAttackKey != EKeys::SpaceBar) APlayerCharacter::currentAttackKey = EKeys::SpaceBar;

        UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
    }
}

void UMainMenuWidget::LookAtPreviousConceptArt()
{
    if (currentImageIndex > 0)
    {
        // Decrement concept art image
        currentImageIndex -= 1;
        imageNumber -= 1;
        ConceptArtImage->SetBrushFromTexture(conceptArtImages[currentImageIndex]);

        if (currentImageText) currentImageText->SetText(FText::FromString(FString::FromInt(imageNumber) + "/" +
            FString::FromInt(maxImages)));
    }

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::LookAtNextConceptArt()
{
    if (currentImageIndex < conceptArtImages.Num() - 1)
    {
        // Decrement concept art image
        currentImageIndex += 1;
        imageNumber += 1;

        ConceptArtImage->SetBrushFromTexture(conceptArtImages[currentImageIndex]);

        if (currentImageText) currentImageText->SetText(FText::FromString(FString::FromInt(imageNumber) + "/" +
            FString::FromInt(maxImages)));
    }

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::ChangeForwardKey()
{
    if (!APlayerCharacter::modifyMoveForwardKey && !APlayerCharacter::modifyMoveBackKey &&
        !APlayerCharacter::modifyMoveLeftKey && !APlayerCharacter::modifyMoveRightKey &&
        !APlayerCharacter::modifyPauseKey && !APlayerCharacter::modifyAttackKey)
    {
        APlayerCharacter::modifyMoveForwardKey = true;
    }

    else if (APlayerCharacter::modifyMoveForwardKey) APlayerCharacter::modifyMoveForwardKey = false;

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::ChangeBackKey()
{
    if (!APlayerCharacter::modifyMoveBackKey && !APlayerCharacter::modifyMoveForwardKey &&
        !APlayerCharacter::modifyMoveLeftKey && !APlayerCharacter::modifyMoveRightKey &&
        !APlayerCharacter::modifyPauseKey && !APlayerCharacter::modifyAttackKey)
    {
        APlayerCharacter::modifyMoveBackKey = true;
    }

    else if (APlayerCharacter::modifyMoveBackKey) APlayerCharacter::modifyMoveBackKey = false;

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::ChangeLeftKey()
{
    if (!APlayerCharacter::modifyMoveLeftKey && !APlayerCharacter::modifyMoveBackKey && 
        !APlayerCharacter::modifyMoveForwardKey && !APlayerCharacter::modifyMoveRightKey &&
        !APlayerCharacter::modifyPauseKey && !APlayerCharacter::modifyAttackKey)
    {
        APlayerCharacter::modifyMoveLeftKey = true;
    }

    else if (APlayerCharacter::modifyMoveLeftKey) APlayerCharacter::modifyMoveLeftKey = false;

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::ChangeRightKey()
{
    if (!APlayerCharacter::modifyMoveRightKey && !APlayerCharacter::modifyMoveBackKey && 
        !APlayerCharacter::modifyMoveForwardKey && !APlayerCharacter::modifyMoveLeftKey &&
        !APlayerCharacter::modifyPauseKey && !APlayerCharacter::modifyAttackKey)
    {
        APlayerCharacter::modifyMoveRightKey = true;
    }

    else if (APlayerCharacter::modifyMoveRightKey) APlayerCharacter::modifyMoveRightKey = false;

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::ChangePauseKeyLogic()
{
    if (!APlayerCharacter::modifyPauseKey && !APlayerCharacter::modifyMoveRightKey && 
        !APlayerCharacter::modifyMoveBackKey && !APlayerCharacter::modifyMoveForwardKey && 
        !APlayerCharacter::modifyMoveLeftKey && !APlayerCharacter::modifyAttackKey)
    {
        APlayerCharacter::modifyPauseKey = true;
    }

    else if (APlayerCharacter::modifyPauseKey) APlayerCharacter::modifyPauseKey = false;

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

void UMainMenuWidget::ChangeAttackKeyLogic()
{
    if (!APlayerCharacter::modifyAttackKey && !APlayerCharacter::modifyPauseKey &&
        !APlayerCharacter::modifyMoveRightKey && !APlayerCharacter::modifyMoveBackKey && 
        !APlayerCharacter::modifyMoveForwardKey && !APlayerCharacter::modifyMoveLeftKey)
    {
        APlayerCharacter::modifyAttackKey = true;
    }

    else if (APlayerCharacter::modifyAttackKey) APlayerCharacter::modifyAttackKey = false;

    UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Button")));
}

// Update controls text here
void UMainMenuWidget::SetMoveForwardControlsText()
{
    // Change forward text
    if (APlayerCharacter::currentMoveForwardKey == EKeys::Up && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Up"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::W && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("W"));
    
    if (APlayerCharacter::currentMoveForwardKey == EKeys::S && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("S"));
    
    if (APlayerCharacter::currentMoveForwardKey == EKeys::A && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("A"));
    
    if (APlayerCharacter::currentMoveForwardKey == EKeys::D && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("D"));
    
    if (APlayerCharacter::currentMoveForwardKey == EKeys::Q && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Q"));
    
    if (APlayerCharacter::currentMoveForwardKey == EKeys::E && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("E"));
    
    if (APlayerCharacter::currentMoveForwardKey == EKeys::R && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("R"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::T && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("T"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Y && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("Y"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::U && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("U"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::I && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("I"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::O && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("O"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::P && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("P"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("F"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::G && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("G"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::H && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("H"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::J && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("J"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::K && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("K"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::L && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("L"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Z && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("Z"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::X && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("X"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::C && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("C"));

    if(APlayerCharacter::currentMoveForwardKey == EKeys::V && ChangeMoveForwardButton && forwardKeyText)
	    forwardKeyText->SetText(FText::FromString("V"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::B && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("B"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::N && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("N"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::M && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("M"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::CapsLock && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Caps Lock"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::LeftShift && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Left Shift"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::LeftControl && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Left Control"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::LeftAlt && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Left Alt"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::SpaceBar && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Space Bar"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Backslash && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Backslash"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::RightControl && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Right Control"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::RightShift && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Right Shift"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Enter && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Enter"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::BackSpace && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Backspace"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Equals && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("="));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Hyphen && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("-"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Zero && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("0"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::One && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("1"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Two && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("2"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Three && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("3"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Four && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("4"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Five && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("5"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Six && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("6"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Seven && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("7"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Eight && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("8"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Nine && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("9"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Tilde && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("~"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Home && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Home"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::PageUp && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("PgUp"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::PageDown && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("PgDn"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::End && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("End"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::LeftBracket && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("["));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::RightBracket && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("]"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Slash && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("/"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Semicolon && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString(";"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Colon && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString(":"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Quote && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("'"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Comma && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString(","));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Period && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("."));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::LeftParantheses && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("("));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::RightParantheses && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString(")"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Escape && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Escape"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Underscore && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("_"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F1 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F1"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F2 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F2"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F3 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F3"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F4 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F4"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F5 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F5"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F6 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F6"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F7 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F7"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F8 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F8"));
    
    if (APlayerCharacter::currentMoveForwardKey == EKeys::F9 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F9"));
    
    if (APlayerCharacter::currentMoveForwardKey == EKeys::F10 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F10"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F11 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F11"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::F12 && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("F12"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Left && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Left"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Right && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Right"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Down && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Down"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Tab && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Tab"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::Delete && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Delete"));

    if (APlayerCharacter::currentMoveForwardKey == EKeys::RightAlt && ChangeMoveForwardButton && forwardKeyText)
        forwardKeyText->SetText(FText::FromString("Right Alt"));

    // Change button background color
    if (!APlayerCharacter::modifyMoveForwardKey)
    {
        if (ChangeMoveForwardButton->GetBackgroundColor() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
            ChangeMoveForwardButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    else if (APlayerCharacter::modifyMoveForwardKey)
    {
        if (ChangeMoveForwardButton->GetBackgroundColor() != FLinearColor(1.0f, 0.0f, 0.0f, 1.0f))
            ChangeMoveForwardButton->SetBackgroundColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
    }
}

void UMainMenuWidget::SetMoveBackControlsText()
{
    if (APlayerCharacter::currentMoveBackKey == EKeys::Down && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Down"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Up && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Up"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::W && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("W"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::S && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("S"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::A && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("A"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::D && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("D"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Q && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Q"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::E && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("E"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::R && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("R"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::T && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("T"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Y && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Y"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::U && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("U"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::I && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("I"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::O && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("O"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::P && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("P"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::G && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("G"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::H && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("H"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::J && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("J"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::K && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("K"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::L && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("L"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Z && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Z"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::X && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("X"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::C && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("C"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::V && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("V"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::B && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("B"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::N && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("N"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::M && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("M"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::CapsLock && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Caps Lock"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::LeftShift && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Left Shift"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::LeftControl && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Left Control"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::LeftAlt && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Left Alt"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::SpaceBar && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Space Bar"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Backslash && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Backslash"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::RightControl && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Right Control"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::RightShift && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Right Shift"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Enter && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Enter"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::BackSpace && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Backspace"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Equals && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("="));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Hyphen && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("-"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Zero && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("0"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::One && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("1"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Two && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("2"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Three && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("3"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Four && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("4"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Five && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("5"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Six && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("6"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Seven && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("7"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Eight && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("8"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Nine && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("9"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Tilde && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("~"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Home && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Home"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::PageUp && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("PgUp"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::PageDown && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("PgDn"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::End && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("End"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::LeftBracket && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("["));

    if (APlayerCharacter::currentMoveBackKey == EKeys::RightBracket && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("]"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Slash && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("/"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Semicolon && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString(";"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Colon && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString(":"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Quote && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("'"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Comma && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString(","));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Period && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("."));

    if (APlayerCharacter::currentMoveBackKey == EKeys::LeftParantheses && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("("));

    if (APlayerCharacter::currentMoveBackKey == EKeys::RightParantheses && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString(")"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Escape && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Escape"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Underscore && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("_"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F1 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F1"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F2 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F2"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F3 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F3"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F4 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F4"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F5 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F5"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F6 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F6"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F7 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F7"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F8 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F8"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F9 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F9"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F10 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F10"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F11 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F11"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::F12 && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("F12"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Left && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Left"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Right && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Right"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Tab && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Tab"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::Delete && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Delete"));

    if (APlayerCharacter::currentMoveBackKey == EKeys::RightAlt && ChangeMoveBackButton && backKeyText)
        backKeyText->SetText(FText::FromString("Right Alt"));

    // Change button background color
    if (!APlayerCharacter::modifyMoveBackKey)
    {
        if (ChangeMoveBackButton->GetBackgroundColor() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
            ChangeMoveBackButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    else if (APlayerCharacter::modifyMoveBackKey)
    {
        if (ChangeMoveBackButton->GetBackgroundColor() != FLinearColor(1.0f, 0.0f, 0.0f, 1.0f))
            ChangeMoveBackButton->SetBackgroundColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
    }
}

void UMainMenuWidget::SetMoveLeftControlsText()
{
    if (APlayerCharacter::currentMoveLeftKey == EKeys::Left && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Left"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Down && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Down"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Up && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Up"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::W && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("W"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::S && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("S"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::A && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("A"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::D && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("D"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Q && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Q"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::E && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("E"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::R && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("R"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::T && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("T"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Y && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Y"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::U && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("U"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::I && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("I"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::O && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("O"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::P && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("P"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::G && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("G"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::H && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("H"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::J && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("J"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::K && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("K"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::L && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("L"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Z && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Z"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::X && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("X"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::C && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("C"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::V && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("V"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::B && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("B"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::N && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("N"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::M && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("M"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::CapsLock && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Caps Lock"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::LeftShift && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Left Shift"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::LeftControl && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Left Control"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::LeftAlt && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Left Alt"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::SpaceBar && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Space Bar"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Backslash && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Backslash"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::RightControl && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Right Control"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::RightShift && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Right Shift"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Enter && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Enter"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::BackSpace && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Backspace"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Equals && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("="));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Hyphen && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("-"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Zero && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("0"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::One && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("1"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Two && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("2"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Three && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("3"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Four && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("4"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Five && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("5"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Six && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("6"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Seven && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("7"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Eight && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("8"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Nine && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("9"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Tilde && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("~"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Home && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Home"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::PageUp && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("PgUp"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::PageDown && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("PgDn"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::End && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("End"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::LeftBracket && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("["));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::RightBracket && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("]"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Slash && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("/"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Semicolon && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString(";"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Colon && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString(":"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Quote && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("'"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Comma && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString(","));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Period && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("."));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::LeftParantheses && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("("));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::RightParantheses && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString(")"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Escape && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Escape"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Underscore && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("_"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F1 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F1"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F2 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F2"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F3 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F3"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F4 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F4"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F5 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F5"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F6 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F6"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F7 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F7"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F8 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F8"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F9 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F9"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F10 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F10"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F11 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F11"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::F12 && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("F12"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Right && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Right"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Tab && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Tab"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::Delete && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Delete"));

    if (APlayerCharacter::currentMoveLeftKey == EKeys::RightAlt && ChangeMoveLeftButton && leftKeyText)
        leftKeyText->SetText(FText::FromString("Right Alt"));

    // Change button background color
    if (!APlayerCharacter::modifyMoveLeftKey)
    {
        if (ChangeMoveLeftButton->GetBackgroundColor() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
            ChangeMoveLeftButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    else if (APlayerCharacter::modifyMoveLeftKey)
    {
        if (ChangeMoveLeftButton->GetBackgroundColor() != FLinearColor(1.0f, 0.0f, 0.0f, 1.0f))
            ChangeMoveLeftButton->SetBackgroundColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
    }
}

void UMainMenuWidget::SetMoveRightControlsText()
{
    if (APlayerCharacter::currentMoveRightKey == EKeys::Right && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Right"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Left && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Left"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Down && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Down"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Up && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Up"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::W && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("W"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::S && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("S"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::A && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("A"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::D && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("D"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Q && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Q"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::E && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("E"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::R && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("R"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::T && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("T"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Y && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Y"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::U && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("U"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::I && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("I"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::O && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("O"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::P && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("P"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::G && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("G"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::H && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("H"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::J && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("J"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::K && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("K"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::L && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("L"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Z && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Z"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::X && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("X"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::C && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("C"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::V && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("V"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::B && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("B"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::N && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("N"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::M && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("M"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::CapsLock && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Caps Lock"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::LeftShift && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Left Shift"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::LeftControl && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Left Control"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::LeftAlt && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Left Alt"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::SpaceBar && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Space Bar"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Backslash && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Backslash"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::RightControl && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Right Control"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::RightShift && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Right Shift"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Enter && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Enter"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::BackSpace && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Backspace"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Equals && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("="));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Hyphen && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("-"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Zero && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("0"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::One && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("1"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Two && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("2"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Three && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("3"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Four && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("4"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Five && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("5"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Six && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("6"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Seven && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("7"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Eight && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("8"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Nine && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("9"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Tilde && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("~"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Home && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Home"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::PageUp && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("PgUp"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::PageDown && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("PgDn"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::End && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("End"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::LeftBracket && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("["));

    if (APlayerCharacter::currentMoveRightKey == EKeys::RightBracket && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("]"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Slash && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("/"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Semicolon && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString(";"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Colon && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString(":"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Quote && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("'"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Comma && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString(","));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Period && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("."));

    if (APlayerCharacter::currentMoveRightKey == EKeys::LeftParantheses && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("("));

    if (APlayerCharacter::currentMoveRightKey == EKeys::RightParantheses && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString(")"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Escape && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Escape"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Underscore && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("_"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F1 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F1"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F2 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F2"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F3 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F3"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F4 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F4"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F5 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F5"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F6 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F6"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F7 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F7"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F8 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F8"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F9 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F9"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F10 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F10"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F11 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F11"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::F12 && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("F12"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Tab && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Tab"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::Delete && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Delete"));

    if (APlayerCharacter::currentMoveRightKey == EKeys::RightAlt && ChangeMoveRightButton && rightKeyText)
        rightKeyText->SetText(FText::FromString("Right Alt"));

    // Change button background color
    if (!APlayerCharacter::modifyMoveRightKey)
    {
        if (ChangeMoveRightButton->GetBackgroundColor() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
            ChangeMoveRightButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    else if (APlayerCharacter::modifyMoveRightKey)
    {
        if (ChangeMoveRightButton->GetBackgroundColor() != FLinearColor(1.0f, 0.0f, 0.0f, 1.0f))
            ChangeMoveRightButton->SetBackgroundColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
    }
}

void UMainMenuWidget::SetPauseControlsText()
{
    if (APlayerCharacter::currentPauseKey == EKeys::SpaceBar && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("SPACE"));

    if (APlayerCharacter::currentPauseKey == EKeys::Left && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Left"));

    if (APlayerCharacter::currentPauseKey == EKeys::Down && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Down"));

    if (APlayerCharacter::currentPauseKey == EKeys::Up && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Up"));

    if (APlayerCharacter::currentPauseKey == EKeys::W && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("W"));

    if (APlayerCharacter::currentPauseKey == EKeys::S && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("S"));

    if (APlayerCharacter::currentPauseKey == EKeys::A && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("A"));

    if (APlayerCharacter::currentPauseKey == EKeys::D && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("D"));

    if (APlayerCharacter::currentPauseKey == EKeys::Q && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Q"));

    if (APlayerCharacter::currentPauseKey == EKeys::E && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("E"));

    if (APlayerCharacter::currentPauseKey == EKeys::R && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("R"));

    if (APlayerCharacter::currentPauseKey == EKeys::T && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("T"));

    if (APlayerCharacter::currentPauseKey == EKeys::Y && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Y"));

    if (APlayerCharacter::currentPauseKey == EKeys::U && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("U"));

    if (APlayerCharacter::currentPauseKey == EKeys::I && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("I"));

    if (APlayerCharacter::currentPauseKey == EKeys::O && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("O"));

    if (APlayerCharacter::currentPauseKey == EKeys::P && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("P"));

    if (APlayerCharacter::currentPauseKey == EKeys::F && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F"));

    if (APlayerCharacter::currentPauseKey == EKeys::G && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("G"));

    if (APlayerCharacter::currentPauseKey == EKeys::H && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("H"));

    if (APlayerCharacter::currentPauseKey == EKeys::J && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("J"));

    if (APlayerCharacter::currentPauseKey == EKeys::K && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("K"));

    if (APlayerCharacter::currentPauseKey == EKeys::L && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("L"));

    if (APlayerCharacter::currentPauseKey == EKeys::Z && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Z"));

    if (APlayerCharacter::currentPauseKey == EKeys::X && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("X"));

    if (APlayerCharacter::currentPauseKey == EKeys::C && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("C"));

    if (APlayerCharacter::currentPauseKey == EKeys::V && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("V"));

    if (APlayerCharacter::currentPauseKey == EKeys::B && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("B"));

    if (APlayerCharacter::currentPauseKey == EKeys::N && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("N"));

    if (APlayerCharacter::currentPauseKey == EKeys::M && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("M"));

    if (APlayerCharacter::currentPauseKey == EKeys::CapsLock && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Caps Lock"));

    if (APlayerCharacter::currentPauseKey == EKeys::LeftShift && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Left Shift"));

    if (APlayerCharacter::currentPauseKey == EKeys::LeftControl && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Left Control"));

    if (APlayerCharacter::currentPauseKey == EKeys::LeftAlt && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Left Alt"));

    if (APlayerCharacter::currentPauseKey == EKeys::Backslash && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Backslash"));

    if (APlayerCharacter::currentPauseKey == EKeys::RightControl && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Right Control"));

    if (APlayerCharacter::currentPauseKey == EKeys::RightShift && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Right Shift"));

    if (APlayerCharacter::currentPauseKey == EKeys::Enter && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Enter"));

    if (APlayerCharacter::currentPauseKey == EKeys::BackSpace && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Backspace"));

    if (APlayerCharacter::currentPauseKey == EKeys::Equals && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("="));

    if (APlayerCharacter::currentPauseKey == EKeys::Hyphen && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("-"));

    if (APlayerCharacter::currentPauseKey == EKeys::Zero && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("0"));

    if (APlayerCharacter::currentPauseKey == EKeys::One && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("1"));

    if (APlayerCharacter::currentPauseKey == EKeys::Two && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("2"));

    if (APlayerCharacter::currentPauseKey == EKeys::Three && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("3"));

    if (APlayerCharacter::currentPauseKey == EKeys::Four && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("4"));

    if (APlayerCharacter::currentPauseKey == EKeys::Five && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("5"));

    if (APlayerCharacter::currentPauseKey == EKeys::Six && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("6"));

    if (APlayerCharacter::currentPauseKey == EKeys::Seven && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("7"));

    if (APlayerCharacter::currentPauseKey == EKeys::Eight && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("8"));

    if (APlayerCharacter::currentPauseKey == EKeys::Nine && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("9"));

    if (APlayerCharacter::currentPauseKey == EKeys::Tilde && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("~"));

    if (APlayerCharacter::currentPauseKey == EKeys::Home && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Home"));

    if (APlayerCharacter::currentPauseKey == EKeys::PageUp && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("PgUp"));

    if (APlayerCharacter::currentPauseKey == EKeys::PageDown && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("PgDn"));

    if (APlayerCharacter::currentPauseKey == EKeys::End && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("End"));

    if (APlayerCharacter::currentPauseKey == EKeys::LeftBracket && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("["));

    if (APlayerCharacter::currentPauseKey == EKeys::RightBracket && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("]"));

    if (APlayerCharacter::currentPauseKey == EKeys::Slash && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("/"));

    if (APlayerCharacter::currentPauseKey == EKeys::Semicolon && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString(";"));

    if (APlayerCharacter::currentPauseKey == EKeys::Colon && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString(":"));

    if (APlayerCharacter::currentPauseKey == EKeys::Quote && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("'"));

    if (APlayerCharacter::currentPauseKey == EKeys::Comma && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString(","));

    if (APlayerCharacter::currentPauseKey == EKeys::Period && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("."));

    if (APlayerCharacter::currentPauseKey == EKeys::LeftParantheses && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("("));

    if (APlayerCharacter::currentPauseKey == EKeys::RightParantheses && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString(")"));

    if (APlayerCharacter::currentPauseKey == EKeys::Escape && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Escape"));

    if (APlayerCharacter::currentPauseKey == EKeys::Underscore && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("_"));

    if (APlayerCharacter::currentPauseKey == EKeys::F1 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F1"));

    if (APlayerCharacter::currentPauseKey == EKeys::F2 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F2"));

    if (APlayerCharacter::currentPauseKey == EKeys::F3 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F3"));

    if (APlayerCharacter::currentPauseKey == EKeys::F4 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F4"));

    if (APlayerCharacter::currentPauseKey == EKeys::F5 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F5"));

    if (APlayerCharacter::currentPauseKey == EKeys::F6 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F6"));

    if (APlayerCharacter::currentPauseKey == EKeys::F7 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F7"));

    if (APlayerCharacter::currentPauseKey == EKeys::F8 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F8"));

    if (APlayerCharacter::currentPauseKey == EKeys::F9 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F9"));

    if (APlayerCharacter::currentPauseKey == EKeys::F10 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F10"));

    if (APlayerCharacter::currentPauseKey == EKeys::F11 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F11"));

    if (APlayerCharacter::currentPauseKey == EKeys::F12 && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("F12"));

    if (APlayerCharacter::currentPauseKey == EKeys::Right && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Right"));

    if (APlayerCharacter::currentPauseKey == EKeys::Tab && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Tab"));

    if (APlayerCharacter::currentPauseKey == EKeys::Delete && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Delete"));

    if (APlayerCharacter::currentPauseKey == EKeys::RightAlt && ChangePauseButton && pauseKeyText)
        pauseKeyText->SetText(FText::FromString("Right Alt"));

    // Change button background color
    if (!APlayerCharacter::modifyPauseKey)
    {
        if (ChangePauseButton->GetBackgroundColor() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
            ChangePauseButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    else if (APlayerCharacter::modifyPauseKey)
    {
        if (ChangePauseButton->GetBackgroundColor() != FLinearColor(1.0f, 0.0f, 0.0f, 1.0f))
            ChangePauseButton->SetBackgroundColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
    }
}

void UMainMenuWidget::SetAttackControlsText()
{
    if (APlayerCharacter::currentAttackKey == EKeys::SpaceBar && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("SPACE"));

    if (APlayerCharacter::currentAttackKey == EKeys::Left && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Left"));

    if (APlayerCharacter::currentAttackKey == EKeys::Down && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Down"));

    if (APlayerCharacter::currentAttackKey == EKeys::Up && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Up"));

    if (APlayerCharacter::currentAttackKey == EKeys::W && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("W"));

    if (APlayerCharacter::currentAttackKey == EKeys::S && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("S"));

    if (APlayerCharacter::currentAttackKey == EKeys::A && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("A"));

    if (APlayerCharacter::currentAttackKey == EKeys::D && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("D"));

    if (APlayerCharacter::currentAttackKey == EKeys::Q && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Q"));

    if (APlayerCharacter::currentAttackKey == EKeys::E && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("E"));

    if (APlayerCharacter::currentAttackKey == EKeys::R && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("R"));

    if (APlayerCharacter::currentAttackKey == EKeys::T && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("T"));

    if (APlayerCharacter::currentAttackKey == EKeys::Y && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Y"));

    if (APlayerCharacter::currentAttackKey == EKeys::U && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("U"));

    if (APlayerCharacter::currentAttackKey == EKeys::I && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("I"));

    if (APlayerCharacter::currentAttackKey == EKeys::O && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("O"));

    if (APlayerCharacter::currentAttackKey == EKeys::P && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("P"));

    if (APlayerCharacter::currentAttackKey == EKeys::F && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F"));

    if (APlayerCharacter::currentAttackKey == EKeys::G && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("G"));

    if (APlayerCharacter::currentAttackKey == EKeys::H && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("H"));

    if (APlayerCharacter::currentAttackKey == EKeys::J && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("J"));

    if (APlayerCharacter::currentAttackKey == EKeys::K && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("K"));

    if (APlayerCharacter::currentAttackKey == EKeys::L && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("L"));

    if (APlayerCharacter::currentAttackKey == EKeys::Z && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Z"));

    if (APlayerCharacter::currentAttackKey == EKeys::X && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("X"));

    if (APlayerCharacter::currentAttackKey == EKeys::C && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("C"));

    if (APlayerCharacter::currentAttackKey == EKeys::V && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("V"));

    if (APlayerCharacter::currentAttackKey == EKeys::B && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("B"));

    if (APlayerCharacter::currentAttackKey == EKeys::N && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("N"));

    if (APlayerCharacter::currentAttackKey == EKeys::M && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("M"));

    if (APlayerCharacter::currentAttackKey == EKeys::CapsLock && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Caps Lock"));

    if (APlayerCharacter::currentAttackKey == EKeys::LeftShift && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Left Shift"));

    if (APlayerCharacter::currentAttackKey == EKeys::LeftControl && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Left Control"));

    if (APlayerCharacter::currentAttackKey == EKeys::LeftAlt && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Left Alt"));

    if (APlayerCharacter::currentAttackKey == EKeys::Backslash && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Backslash"));

    if (APlayerCharacter::currentAttackKey == EKeys::RightControl && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Right Control"));

    if (APlayerCharacter::currentAttackKey == EKeys::RightShift && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Right Shift"));

    if (APlayerCharacter::currentAttackKey == EKeys::Enter && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Enter"));

    if (APlayerCharacter::currentAttackKey == EKeys::BackSpace && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Backspace"));

    if (APlayerCharacter::currentAttackKey == EKeys::Equals && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("="));

    if (APlayerCharacter::currentAttackKey == EKeys::Hyphen && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("-"));

    if (APlayerCharacter::currentAttackKey == EKeys::Zero && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("0"));

    if (APlayerCharacter::currentAttackKey == EKeys::One && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("1"));

    if (APlayerCharacter::currentAttackKey == EKeys::Two && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("2"));

    if (APlayerCharacter::currentAttackKey == EKeys::Three && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("3"));

    if (APlayerCharacter::currentAttackKey == EKeys::Four && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("4"));

    if (APlayerCharacter::currentAttackKey == EKeys::Five && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("5"));

    if (APlayerCharacter::currentAttackKey == EKeys::Six && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("6"));

    if (APlayerCharacter::currentAttackKey == EKeys::Seven && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("7"));

    if (APlayerCharacter::currentAttackKey == EKeys::Eight && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("8"));

    if (APlayerCharacter::currentAttackKey == EKeys::Nine && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("9"));

    if (APlayerCharacter::currentAttackKey == EKeys::Tilde && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("~"));

    if (APlayerCharacter::currentAttackKey == EKeys::Home && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Home"));

    if (APlayerCharacter::currentAttackKey == EKeys::PageUp && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("PgUp"));

    if (APlayerCharacter::currentAttackKey == EKeys::PageDown && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("PgDn"));

    if (APlayerCharacter::currentAttackKey == EKeys::End && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("End"));

    if (APlayerCharacter::currentAttackKey == EKeys::LeftBracket && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("["));

    if (APlayerCharacter::currentAttackKey == EKeys::RightBracket && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("]"));

    if (APlayerCharacter::currentAttackKey == EKeys::Slash && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("/"));

    if (APlayerCharacter::currentAttackKey == EKeys::Semicolon && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString(";"));

    if (APlayerCharacter::currentAttackKey == EKeys::Colon && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString(":"));

    if (APlayerCharacter::currentAttackKey == EKeys::Quote && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("'"));

    if (APlayerCharacter::currentAttackKey == EKeys::Comma && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString(","));

    if (APlayerCharacter::currentAttackKey == EKeys::Period && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("."));

    if (APlayerCharacter::currentAttackKey == EKeys::LeftParantheses && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("("));

    if (APlayerCharacter::currentAttackKey == EKeys::RightParantheses && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString(")"));

    if (APlayerCharacter::currentAttackKey == EKeys::Escape && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Escape"));

    if (APlayerCharacter::currentAttackKey == EKeys::Underscore && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("_"));

    if (APlayerCharacter::currentAttackKey == EKeys::F1 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F1"));

    if (APlayerCharacter::currentAttackKey == EKeys::F2 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F2"));

    if (APlayerCharacter::currentAttackKey == EKeys::F3 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F3"));

    if (APlayerCharacter::currentAttackKey == EKeys::F4 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F4"));

    if (APlayerCharacter::currentAttackKey == EKeys::F5 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F5"));

    if (APlayerCharacter::currentAttackKey == EKeys::F6 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F6"));

    if (APlayerCharacter::currentAttackKey == EKeys::F7 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F7"));

    if (APlayerCharacter::currentAttackKey == EKeys::F8 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F8"));

    if (APlayerCharacter::currentAttackKey == EKeys::F9 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F9"));

    if (APlayerCharacter::currentAttackKey == EKeys::F10 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F10"));

    if (APlayerCharacter::currentAttackKey == EKeys::F11 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F11"));

    if (APlayerCharacter::currentAttackKey == EKeys::F12 && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("F12"));

    if (APlayerCharacter::currentAttackKey == EKeys::Right && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Right"));

    if (APlayerCharacter::currentAttackKey == EKeys::Tab && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Tab"));

    if (APlayerCharacter::currentAttackKey == EKeys::Delete && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Delete"));

    if (APlayerCharacter::currentAttackKey == EKeys::RightAlt && ChangeAttackButton && attackKeyText)
        attackKeyText->SetText(FText::FromString("Right Alt"));

    // Change button background color
    if (!APlayerCharacter::modifyAttackKey)
    {
        if (ChangeAttackButton->GetBackgroundColor() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
            ChangeAttackButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
    }

    else if (APlayerCharacter::modifyAttackKey)
    {
        if (ChangeAttackButton->GetBackgroundColor() != FLinearColor(1.0f, 0.0f, 0.0f, 1.0f))
            ChangeAttackButton->SetBackgroundColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
    }
}