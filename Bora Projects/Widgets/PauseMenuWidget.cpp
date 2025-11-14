// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/PauseMenuWidget.h"
#include "Characters/Player/PlayerCharacter.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    selectedButton = 0;

    nextState = NextPauseMenuState::ENull;
}

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UGameplayStatics::SetGamePaused(this, true);

    PauseGameMusic(true);

    // Set up button delegates here
    ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
    QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitToMain);

    ResumeButton->OnPressed.AddDynamic(this, &UPauseMenuWidget::ResumeGame);
    QuitButton->OnPressed.AddDynamic(this, &UPauseMenuWidget::QuitToMain);

    ResumeButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::HoverOnPlayButton);
    QuitButton->OnHovered.AddDynamic(this, &UPauseMenuWidget::HoverOnQuitButton);

    if (ResumeButton)
    {
        normalColor = ResumeButton->GetStyle().Normal.TintColor;
        hoveredColor = ResumeButton->GetStyle().Hovered.TintColor;
        pressedColor = ResumeButton->GetStyle().Pressed.TintColor;
    }

    if (ShowButtons)
    {
        PlayAnimation(ShowButtons);
    }
}

void UPauseMenuWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
    Super::NativeTick(Geometry, DeltaTime);

    switch (nextState)
    {
    case NextPauseMenuState::ENull:
        UpdatePauseMenuButtonState();

        break;

    case NextPauseMenuState::EResume:

        if (HideButtons && !IsAnimationPlaying(HideButtons))
        {
            UGameplayStatics::SetGamePaused(this, false);

            PauseGameMusic(false);

            this->RemoveFromParent();
        }

        break;

    case NextPauseMenuState::EQuit:

        if (HideButtons && !IsAnimationPlaying(HideButtons))
        {
            UGameplayStatics::SetGamePaused(this, false);

            // Open the main menu
            UGameplayStatics::OpenLevel(this, "/Game/Levels/MainMenu");
        }

        break;

    default:
        break;
    }
}

void UPauseMenuWidget::UpdatePauseMenuButtonState()
{
    switch (selectedButton)
    {
    case 0: // Play button

        if (ResumeButton)
        {
            FButtonStyle ButtonStyle = ResumeButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != hoveredColor && !ResumeButton->IsHovered())
            {
                ButtonStyle.Normal.TintColor = hoveredColor;
            }

            if (ButtonStyle.Normal.TintColor != hoveredColor && !ResumeButton->IsPressed())
            {
                ButtonStyle.Normal.TintColor = hoveredColor;
            }

            ResumeButton->SetStyle(ButtonStyle);
        }

        if (QuitButton)
        {
            FButtonStyle ButtonStyle2 = QuitButton->GetStyle();

            if (ButtonStyle2.Normal.TintColor != normalColor && !QuitButton->IsHovered())
            {
                ButtonStyle2.Normal.TintColor = normalColor;
            }

            QuitButton->SetStyle(ButtonStyle2);
        }

        if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Down))
        {
            ++selectedButton;
        }

        if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Enter))
        {
            ResumeGame();
        }

        break;

    case 1: // Quit button

        if (ResumeButton)
        {
            FButtonStyle ButtonStyle = ResumeButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != normalColor && !ResumeButton->IsHovered())
            {
                ButtonStyle.Normal.TintColor = normalColor;
            }

            ResumeButton->SetStyle(ButtonStyle);
        }

        if (QuitButton)
        {
            FButtonStyle ButtonStyle2 = QuitButton->GetStyle();

            if (ButtonStyle2.Normal.TintColor != hoveredColor && !QuitButton->IsHovered())
            {
                ButtonStyle2.Normal.TintColor = hoveredColor;
            }

            if (ButtonStyle2.Normal.TintColor != hoveredColor && !QuitButton->IsPressed())
            {
                ButtonStyle2.Normal.TintColor = hoveredColor;
            }

            QuitButton->SetStyle(ButtonStyle2);
        }

        if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Up))
        {
            --selectedButton;
        }

        if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Enter))
        {
            QuitToMain();
        }

        break;

    default:
        selectedButton = 0;
        break;
    }
}

void UPauseMenuWidget::HoverOnPlayButton()
{
    selectedButton = 0;
}

void UPauseMenuWidget::HoverOnQuitButton()
{
    selectedButton = 1;
}

void UPauseMenuWidget::ResumeGame()
{
    if (HideButtons)
    {
        if (ResumeButton)
        {
            FButtonStyle ButtonStyle = ResumeButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != pressedColor)
            {
                ButtonStyle.Normal.TintColor = pressedColor;
            }

            ResumeButton->SetStyle(ButtonStyle);
        }

        PlayAnimation(HideButtons);

        nextState = NextPauseMenuState::EResume;

        ResumeButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
        QuitButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::QuitToMain);

        ResumeButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
        QuitButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::QuitToMain);

        ResumeButton->OnHovered.RemoveDynamic(this, &UPauseMenuWidget::HoverOnPlayButton);
        QuitButton->OnHovered.RemoveDynamic(this, &UPauseMenuWidget::HoverOnQuitButton);
    }
}

void UPauseMenuWidget::QuitToMain()
{
    if (HideButtons)
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

        nextState = NextPauseMenuState::EQuit;

        ResumeButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
        QuitButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::QuitToMain);

        ResumeButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
        QuitButton->OnPressed.RemoveDynamic(this, &UPauseMenuWidget::QuitToMain);

        ResumeButton->OnHovered.RemoveDynamic(this, &UPauseMenuWidget::HoverOnPlayButton);
        QuitButton->OnHovered.RemoveDynamic(this, &UPauseMenuWidget::HoverOnQuitButton);
    }
}

void UPauseMenuWidget::PauseGameMusic(bool paused_)
{
    if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()))
    {
        playerCharacter->ShouldPauseMusic(paused_);
        playerCharacter = nullptr;
    }
}