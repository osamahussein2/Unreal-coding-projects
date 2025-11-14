// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/GameWinWidget.h"
#include "Characters/Player/PlayerCharacter.h"

UGameWinWidget::UGameWinWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    selectedButton = 0;

    nextState = NextGameWinState::ENull;
}

void UGameWinWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UGameplayStatics::SetGamePaused(this, true);

    PauseGameMusic(true);

    // Set up button delegates here
    RestartButton->OnClicked.AddDynamic(this, &UGameWinWidget::ResumeGame);
    QuitButton->OnClicked.AddDynamic(this, &UGameWinWidget::QuitToMain);

    RestartButton->OnPressed.AddDynamic(this, &UGameWinWidget::ResumeGame);
    QuitButton->OnPressed.AddDynamic(this, &UGameWinWidget::QuitToMain);

    RestartButton->OnHovered.AddDynamic(this, &UGameWinWidget::HoverOnPlayButton);
    QuitButton->OnHovered.AddDynamic(this, &UGameWinWidget::HoverOnQuitButton);

    if (RestartButton)
    {
        normalColor = RestartButton->GetStyle().Normal.TintColor;
        hoveredColor = RestartButton->GetStyle().Hovered.TintColor;
        pressedColor = RestartButton->GetStyle().Pressed.TintColor;
    }

    if (ShowButtons)
    {
        PlayAnimation(ShowButtons);
    }
}

void UGameWinWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
    Super::NativeTick(Geometry, DeltaTime);

    switch (nextState)
    {
    case NextGameWinState::ENull:
        UpdateGameOverButtonState();

        break;

    case NextGameWinState::EResume:

        if (HideButtons && !IsAnimationPlaying(HideButtons))
        {
            UGameplayStatics::SetGamePaused(this, false);

            this->RemoveFromParent();

            // Open the game level
            UGameplayStatics::OpenLevel(this, "/Game/Levels/GameLevel");
        }

        break;

    case NextGameWinState::EQuit:

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

void UGameWinWidget::UpdateGameOverButtonState()
{
    switch (selectedButton)
    {
    case 0: // Play button

        if (RestartButton)
        {
            FButtonStyle ButtonStyle = RestartButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != hoveredColor && !RestartButton->IsHovered())
            {
                ButtonStyle.Normal.TintColor = hoveredColor;
            }

            if (ButtonStyle.Normal.TintColor != hoveredColor && !RestartButton->IsPressed())
            {
                ButtonStyle.Normal.TintColor = hoveredColor;
            }

            RestartButton->SetStyle(ButtonStyle);
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

        if (RestartButton)
        {
            FButtonStyle ButtonStyle = RestartButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != normalColor && !RestartButton->IsHovered())
            {
                ButtonStyle.Normal.TintColor = normalColor;
            }

            RestartButton->SetStyle(ButtonStyle);
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

void UGameWinWidget::HoverOnPlayButton()
{
    selectedButton = 0;
}

void UGameWinWidget::HoverOnQuitButton()
{
    selectedButton = 1;
}

void UGameWinWidget::ResumeGame()
{
    if (HideButtons)
    {
        if (RestartButton)
        {
            FButtonStyle ButtonStyle = RestartButton->GetStyle();

            if (ButtonStyle.Normal.TintColor != pressedColor)
            {
                ButtonStyle.Normal.TintColor = pressedColor;
            }

            RestartButton->SetStyle(ButtonStyle);
        }

        PlayAnimation(HideButtons);

        nextState = NextGameWinState::EResume;

        RestartButton->OnClicked.RemoveDynamic(this, &UGameWinWidget::ResumeGame);
        QuitButton->OnClicked.RemoveDynamic(this, &UGameWinWidget::QuitToMain);

        RestartButton->OnPressed.RemoveDynamic(this, &UGameWinWidget::ResumeGame);
        QuitButton->OnPressed.RemoveDynamic(this, &UGameWinWidget::QuitToMain);

        RestartButton->OnHovered.RemoveDynamic(this, &UGameWinWidget::HoverOnPlayButton);
        QuitButton->OnHovered.RemoveDynamic(this, &UGameWinWidget::HoverOnQuitButton);
    }
}

void UGameWinWidget::QuitToMain()
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

        nextState = NextGameWinState::EQuit;

        RestartButton->OnClicked.RemoveDynamic(this, &UGameWinWidget::ResumeGame);
        QuitButton->OnClicked.RemoveDynamic(this, &UGameWinWidget::QuitToMain);

        RestartButton->OnPressed.RemoveDynamic(this, &UGameWinWidget::ResumeGame);
        QuitButton->OnPressed.RemoveDynamic(this, &UGameWinWidget::QuitToMain);

        RestartButton->OnHovered.RemoveDynamic(this, &UGameWinWidget::HoverOnPlayButton);
        QuitButton->OnHovered.RemoveDynamic(this, &UGameWinWidget::HoverOnQuitButton);
    }
}

void UGameWinWidget::PauseGameMusic(bool paused_)
{
    if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()))
    {
        playerCharacter->ShouldPauseMusic(paused_);
        playerCharacter = nullptr;
    }
}