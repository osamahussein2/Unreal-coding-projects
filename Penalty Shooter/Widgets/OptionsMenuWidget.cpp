// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/OptionsMenuWidget.h"
#include "Player/PlayerCharacter.h"
#include "GameMode/ScoreJam37_GM.h"

UOptionsMenuWidget::UOptionsMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UOptionsMenuWidget::NativeConstruct()
{
	if (backButton)
	{
		backButton->OnHovered.AddUniqueDynamic(this, &UOptionsMenuWidget::HoverOnBackButton);
		backButton->OnClicked.AddUniqueDynamic(this, &UOptionsMenuWidget::GoBackToMainMenu);
	}

	if (LeftDifficultyArrowButton)
	{
		LeftDifficultyArrowButton->OnHovered.AddUniqueDynamic(this, &UOptionsMenuWidget::HoverOnLeftDifficultyArrowButton);
		LeftDifficultyArrowButton->OnClicked.AddUniqueDynamic(this, &UOptionsMenuWidget::LowerDifficulty);
	}

	if (RightDifficultyArrowButton)
	{
		RightDifficultyArrowButton->OnHovered.AddUniqueDynamic(this, &UOptionsMenuWidget::HoverOnRightDifficultyArrowButton);
		RightDifficultyArrowButton->OnClicked.AddUniqueDynamic(this, &UOptionsMenuWidget::IncreaseDifficulty);
	}

	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		switch (playerCharacter->GetCurrentDifficultyIndex())
		{
		case 0:
			if (CurrentDifficultyText) CurrentDifficultyText->SetText(FText::FromString("Easy"));
			if (LeftDifficultyArrowButton) LeftDifficultyArrowButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
			break;

		case 1:
			if (CurrentDifficultyText) CurrentDifficultyText->SetText(FText::FromString("Normal"));
			break;

		case 2:
			if (CurrentDifficultyText) CurrentDifficultyText->SetText(FText::FromString("Hard"));
			if (RightDifficultyArrowButton) RightDifficultyArrowButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
			break;
		}

		playerCharacter->UpdateVariablesBasedOnCurrentDifficulty();
		playerCharacter = nullptr;
	}

	// Set back images to be invisible
	if (LeftBackImage && LeftBackImage->IsVisible()) LeftBackImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightBackImage && RightBackImage->IsVisible()) RightBackImage->SetVisibility(ESlateVisibility::Hidden);

	// Set back text color and opacity to white
	if (BackText) BackText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	if (AScoreJam37_GM* scoreJamGM = Cast<AScoreJam37_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// Set the music volume slider value to the current music volume from the AScoreJam37_GM class
		if (MusicVolumeSlider) MusicVolumeSlider->SetValue(scoreJamGM->GetMusicVolume());

		// Return the music volume percentage
		if (MusicVolumePercentageText)
		{
			MusicVolumePercentageText->SetText(FText::FromString(FString::FromInt(static_cast<int>(
				scoreJamGM->GetMusicVolume() * 100.0f)) + TEXT("%")));
		}

		scoreJamGM = nullptr;
	}

	// Bind music volume slider OnValueChanged delegate to the update music volume function
	if (MusicVolumeSlider)
	{
		MusicVolumeSlider->OnValueChanged.AddUniqueDynamic(this, &UOptionsMenuWidget::UpdateMusicVolumeSlider);
	}
}

void UOptionsMenuWidget::GoBackToMainMenu()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		RemoveFromParent();

		playerCharacter->ShowMainMenu(false);

		playerCharacter = nullptr;
	}

	if (backButton)
	{
		backButton->OnHovered.RemoveDynamic(this, &UOptionsMenuWidget::HoverOnBackButton);
		backButton->OnClicked.RemoveDynamic(this, &UOptionsMenuWidget::GoBackToMainMenu);
	}

	if (LeftDifficultyArrowButton)
	{
		LeftDifficultyArrowButton->OnHovered.RemoveDynamic(this, &UOptionsMenuWidget::HoverOnLeftDifficultyArrowButton);
		LeftDifficultyArrowButton->OnClicked.RemoveDynamic(this, &UOptionsMenuWidget::LowerDifficulty);
	}

	if (RightDifficultyArrowButton)
	{
		RightDifficultyArrowButton->OnHovered.RemoveDynamic(this, &UOptionsMenuWidget::HoverOnRightDifficultyArrowButton);
		RightDifficultyArrowButton->OnClicked.RemoveDynamic(this, &UOptionsMenuWidget::IncreaseDifficulty);
	}

	// Remove the update music volume slider binding from the OnValueChanged signature once going back to the main menu
	if (MusicVolumeSlider) MusicVolumeSlider->OnValueChanged.RemoveDynamic(this, &UOptionsMenuWidget::UpdateMusicVolumeSlider);
}

void UOptionsMenuWidget::LowerDifficulty()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		switch (playerCharacter->GetCurrentDifficultyIndex())
		{
		case 0: // Easy
			break;

		case 1: // Normal
			playerCharacter->DecrementCurrentDifficultyIndex();
			if (CurrentDifficultyText) CurrentDifficultyText->SetText(FText::FromString("Easy"));

			if (LeftDifficultyArrowButton) LeftDifficultyArrowButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
			break;

		case 2: // Hard
			playerCharacter->DecrementCurrentDifficultyIndex();
			if (CurrentDifficultyText) CurrentDifficultyText->SetText(FText::FromString("Normal"));

			if (RightDifficultyArrowButton) RightDifficultyArrowButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			break;

		default:
			break;
		}

		playerCharacter->UpdateVariablesBasedOnCurrentDifficulty();
		playerCharacter = nullptr;
	}
}

void UOptionsMenuWidget::IncreaseDifficulty()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		switch (playerCharacter->GetCurrentDifficultyIndex())
		{
		case 0: // Easy
			playerCharacter->IncrementCurrentDifficultyIndex();
			if (CurrentDifficultyText) CurrentDifficultyText->SetText(FText::FromString("Normal"));

			if (LeftDifficultyArrowButton) LeftDifficultyArrowButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			break;

		case 1: // Normal
			playerCharacter->IncrementCurrentDifficultyIndex();
			if (CurrentDifficultyText) CurrentDifficultyText->SetText(FText::FromString("Hard"));

			if (RightDifficultyArrowButton) RightDifficultyArrowButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.3f));
			break;

		case 2: // Hard
			break;

		default:
			break;
		}

		playerCharacter->UpdateVariablesBasedOnCurrentDifficulty();
		playerCharacter = nullptr;
	}
}

void UOptionsMenuWidget::HoverOnBackButton()
{
	// Set back images to be visible
	if (LeftBackImage && !LeftBackImage->IsVisible()) LeftBackImage->SetVisibility(ESlateVisibility::Visible);
	if (RightBackImage && !RightBackImage->IsVisible()) RightBackImage->SetVisibility(ESlateVisibility::Visible);

	// Set back text color and opacity to yellow
	if (BackText && BackText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		BackText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}
}

void UOptionsMenuWidget::HoverOnLeftDifficultyArrowButton()
{
	// Set back images to be invisible
	if (LeftBackImage && LeftBackImage->IsVisible()) LeftBackImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightBackImage && RightBackImage->IsVisible()) RightBackImage->SetVisibility(ESlateVisibility::Hidden);

	// Set back text color and opacity to white
	if (BackText && BackText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		BackText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UOptionsMenuWidget::HoverOnRightDifficultyArrowButton()
{
	// Set back images to be invisible
	if (LeftBackImage && LeftBackImage->IsVisible()) LeftBackImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightBackImage && RightBackImage->IsVisible()) RightBackImage->SetVisibility(ESlateVisibility::Hidden);

	// Set back text color and opacity to white
	if (BackText && BackText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		BackText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UOptionsMenuWidget::UpdateMusicVolumeSlider(float musicVolumeValue_)
{
	if (AScoreJam37_GM* scoreJamGM = Cast<AScoreJam37_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// Change the soccer music volume based modified music volume value
		scoreJamGM->UpdateSoccerMusicVolume(musicVolumeValue_);

		if (MusicVolumePercentageText)
		{
			// Return the updated music volume percentage
			MusicVolumePercentageText->SetText(FText::FromString(FString::FromInt(static_cast<int>(musicVolumeValue_ * 100.0f)) 
				+ TEXT("%")));
		}

		scoreJamGM = nullptr;
	}
}