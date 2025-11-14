// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MainMenuWidget.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    selectedButton = 0;

    nextState = NextMainMenuState::ENull;
}

/*void UMainMenuWidget::NativeOnInitialized()
{
}*/

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (PlayButton)
    {
        normalColor = PlayButton->GetStyle().Normal.TintColor;
        hoveredColor = PlayButton->GetStyle().Hovered.TintColor;
        pressedColor = PlayButton->GetStyle().Pressed.TintColor;
    }

    if (ShowButtons)
    {
        PlayAnimation(ShowButtons);
    }

    if (CreditsMenuCanvas)
    {
        CreditsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
    }

    if (MainMenuCanvas)
    {
        MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);
    }

    // Set up button delegates here
    PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayLevel);
    CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToCredits);
    CreditsBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoBackToMainMenu);
    QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);

    PlayButton->OnPressed.AddDynamic(this, &UMainMenuWidget::PlayLevel);
    CreditsButton->OnPressed.AddDynamic(this, &UMainMenuWidget::GoToCredits);
    CreditsBackButton->OnPressed.AddDynamic(this, &UMainMenuWidget::GoBackToMainMenu);
    QuitButton->OnPressed.AddDynamic(this, &UMainMenuWidget::QuitGame);

    PlayButton->OnHovered.AddDynamic(this, &UMainMenuWidget::HoverOnPlayButton);
    CreditsButton->OnHovered.AddDynamic(this, &UMainMenuWidget::HoverOnCreditsButton);
    QuitButton->OnHovered.AddDynamic(this, &UMainMenuWidget::HoverOnQuitButton);
}

void UMainMenuWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
    Super::NativeTick(Geometry, DeltaTime);

    switch (nextState)
    {
    case NextMainMenuState::ENull:

        UpdateMainMenuButtonState();

        break;

    case NextMainMenuState::EPlay:

        if (HideButtons && !IsAnimationPlaying(HideButtons))
        {
            // Open the game's level
            UGameplayStatics::OpenLevel(this, "/Game/Levels/GameLevel");
        }

        break;

    case NextMainMenuState::ECredits:

        // Show the credits menu after the main menu buttons are hidden
        if (HideButtons && !IsAnimationPlaying(HideButtons) && !CreditsMenuCanvas->IsVisible())
        {
            selectedButton = 0; // Set it to 0 for hovering on credits back button

            MainMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
            CreditsMenuCanvas->SetVisibility(ESlateVisibility::Visible);

            // Play the show buttons animation
            PlayAnimation(ShowButtons);
        }

        else if (ShowButtons && !IsAnimationPlaying(ShowButtons))
        {
            // Loop the scrolling credits animation
            if (ScrollCredits && !IsAnimationPlaying(ScrollCredits))
            {
                PlayAnimation(ScrollCredits);
            }

            // Check if the player has pressed the back button
            UpdateCreditsMenuButtonState();
        }

        break;

    case NextMainMenuState::EGoBackToMainMenu:

        if (HideButtons && !IsAnimationPlaying(HideButtons))
        {
            // Determine the selected button number depending on what's visible before hiding the canvas
            if (CreditsMenuCanvas->IsVisible()) selectedButton = 1;

            // Hide credits menu and show main menu
            CreditsMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
            MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);

            StopAnimation(ScrollCredits); // Stop scrolling credits
            PlayAnimation(ShowButtons); // Play show buttons animation

            // Go back to the null main menu state
            nextState = NextMainMenuState::ENull;
        }

        break;

    case NextMainMenuState::EQuit:

        if (HideButtons && !IsAnimationPlaying(HideButtons))
        {
            // Quit the game
            UKismetSystemLibrary::QuitGame(nullptr, GetWorld()->GetFirstPlayerController(),
                EQuitPreference::Quit, true);
        }

        break;

    default:
        break;
    }
}

void UMainMenuWidget::UpdateMainMenuButtonState()
{
    switch (selectedButton)
    {
    case 0: // Play button

        if (PlayButton)
        {
            FButtonStyle ButtonStyle = PlayButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != hoveredColor && !PlayButton->IsHovered())
            {
                ButtonStyle.Normal.TintColor = hoveredColor;
            }

            if (ButtonStyle.Normal.TintColor != hoveredColor && !PlayButton->IsPressed())
            {
                ButtonStyle.Normal.TintColor = hoveredColor;
            }

            PlayButton->SetStyle(ButtonStyle);
        }

        if (CreditsButton)
        {
            FButtonStyle ButtonStyle2 = CreditsButton->GetStyle();

            if (ButtonStyle2.Normal.TintColor != normalColor && !CreditsButton->IsHovered())
            {
                ButtonStyle2.Normal.TintColor = normalColor;
            }

            CreditsButton->SetStyle(ButtonStyle2);
        }

        if (QuitButton)
        {
            FButtonStyle ButtonStyle3 = QuitButton->GetStyle();

            if (ButtonStyle3.Normal.TintColor != normalColor && !QuitButton->IsHovered())
            {
                ButtonStyle3.Normal.TintColor = normalColor;
            }

            QuitButton->SetStyle(ButtonStyle3);
        }

        if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Down))
        {
            ++selectedButton;
        }

        if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
        {
            PlayLevel();
        }

        break;

    case 1: // Credits button

        if (PlayButton)
        {
            FButtonStyle ButtonStyle = PlayButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != normalColor && !PlayButton->IsHovered())
            {
                ButtonStyle.Normal.TintColor = normalColor;
            }

            PlayButton->SetStyle(ButtonStyle);
        }

        if (CreditsButton)
        {
            FButtonStyle ButtonStyle2 = CreditsButton->GetStyle();

            if (ButtonStyle2.Normal.TintColor != hoveredColor && !CreditsButton->IsHovered())
            {
                ButtonStyle2.Normal.TintColor = hoveredColor;
            }

            if (ButtonStyle2.Normal.TintColor != hoveredColor && !CreditsButton->IsPressed())
            {
                ButtonStyle2.Normal.TintColor = hoveredColor;
            }

            CreditsButton->SetStyle(ButtonStyle2);
        }

        if (QuitButton)
        {
            FButtonStyle ButtonStyle3 = QuitButton->GetStyle();

            if (ButtonStyle3.Normal.TintColor != normalColor && !QuitButton->IsHovered())
            {
                ButtonStyle3.Normal.TintColor = normalColor;
            }

            QuitButton->SetStyle(ButtonStyle3);
        }

        if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Up))
        {
            --selectedButton;
        }

        if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Down))
        {
            ++selectedButton;
        }

        if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
        {
            GoToCredits();
        }

        break;

    case 2: // Quit button

        if (PlayButton)
        {
            FButtonStyle ButtonStyle = PlayButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != normalColor && !PlayButton->IsHovered())
            {
                ButtonStyle.Normal.TintColor = normalColor;
            }

            PlayButton->SetStyle(ButtonStyle);
        }

        if (CreditsButton)
        {
            FButtonStyle ButtonStyle2 = CreditsButton->GetStyle();

            if (ButtonStyle2.Normal.TintColor != normalColor && !CreditsButton->IsHovered())
            {
                ButtonStyle2.Normal.TintColor = normalColor;
            }

            CreditsButton->SetStyle(ButtonStyle2);
        }

        if (QuitButton)
        {
            FButtonStyle ButtonStyle3 = QuitButton->GetStyle();

            if (ButtonStyle3.Normal.TintColor != hoveredColor && !QuitButton->IsHovered())
            {
                ButtonStyle3.Normal.TintColor = hoveredColor;
            }

            if (ButtonStyle3.Normal.TintColor != hoveredColor && !QuitButton->IsPressed())
            {
                ButtonStyle3.Normal.TintColor = hoveredColor;
            }

            QuitButton->SetStyle(ButtonStyle3);
        }

        if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Up))
        {
            --selectedButton;
        }

        if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
        {
            QuitGame();
        }

        break;

    default:
        selectedButton = 0;
        break;
    }
}

void UMainMenuWidget::UpdateCreditsMenuButtonState()
{
    switch (selectedButton)
    {
    case 0: // Credits Back button

        if (CreditsBackButton)
        {
            FButtonStyle ButtonStyle = CreditsBackButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != hoveredColor && !CreditsBackButton->IsHovered())
            {
                ButtonStyle.Normal.TintColor = hoveredColor;
            }

            if (ButtonStyle.Normal.TintColor != hoveredColor && !CreditsBackButton->IsPressed())
            {
                ButtonStyle.Normal.TintColor = hoveredColor;
            }

            CreditsBackButton->SetStyle(ButtonStyle);
        }

        if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Enter))
        {
            GoBackToMainMenu();
        }

        break;

    default:
        selectedButton = 0;
        break;
    }
}

void UMainMenuWidget::HoverOnPlayButton()
{
    selectedButton = 0;
}

void UMainMenuWidget::HoverOnCreditsButton()
{
    selectedButton = 1;
}

void UMainMenuWidget::HoverOnQuitButton()
{
    selectedButton = 2;
}

void UMainMenuWidget::PlayLevel()
{
    if (HideButtons && !IsAnimationPlaying(HideButtons))
    {
        if (PlayButton)
        {
            FButtonStyle ButtonStyle = PlayButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != pressedColor)
            {
                ButtonStyle.Normal.TintColor = pressedColor;
            }

            PlayButton->SetStyle(ButtonStyle);
        }

        PlayAnimation(HideButtons);

        nextState = NextMainMenuState::EPlay;
    }
}

void UMainMenuWidget::GoToCredits()
{
    if (HideButtons && !IsAnimationPlaying(HideButtons))
    {
        if (CreditsButton)
        {
            FButtonStyle ButtonStyle = CreditsButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != pressedColor)
            {
                ButtonStyle.Normal.TintColor = pressedColor;
            }

            CreditsButton->SetStyle(ButtonStyle);
        }

        PlayAnimation(HideButtons);

        nextState = NextMainMenuState::ECredits;
    }
}

void UMainMenuWidget::QuitGame()
{
    if (HideButtons && !IsAnimationPlaying(HideButtons))
    {
        if (QuitButton)
        {
            FButtonStyle ButtonStyle = QuitButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != pressedColor)
            {
                ButtonStyle.Normal.TintColor = pressedColor;
            }

            QuitButton->SetStyle(ButtonStyle);
        }

        PlayAnimation(HideButtons);

        nextState = NextMainMenuState::EQuit;
    }
}

// Credits Menu back button functionality
void UMainMenuWidget::GoBackToMainMenu()
{
    if (HideButtons && !IsAnimationPlaying(HideButtons))
    {
        if (CreditsBackButton)
        {
            FButtonStyle ButtonStyle = CreditsBackButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != pressedColor)
            {
                ButtonStyle.Normal.TintColor = pressedColor;
            }

            CreditsBackButton->SetStyle(ButtonStyle);
        }

        PlayAnimation(HideButtons);

        nextState = NextMainMenuState::EGoBackToMainMenu;
    }
}