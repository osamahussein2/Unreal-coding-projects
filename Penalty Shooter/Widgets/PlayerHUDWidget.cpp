// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/PlayerHUDWidget.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerHUDWidget::NativeConstruct()
{
}

void UPlayerHUDWidget::SetTimeText(int& minutes_, float& seconds_)
{
	if (TimeText)
	{
		if (seconds_ < 10.0f)
		{
			TimeText->SetText(FText::FromString(TEXT("Time: ") + FString::FromInt(minutes_) + TEXT(":0") + 
				FString::FromInt(static_cast<int>(seconds_))));
		}

		else
		{
			TimeText->SetText(FText::FromString(TEXT("Time: ") + FString::FromInt(minutes_) + TEXT(":") +
				FString::FromInt(static_cast<int>(seconds_))));
		}
	}
}

void UPlayerHUDWidget::SetScoreText(int& score_)
{
	if (ScoreText) ScoreText->SetText(FText::FromString(TEXT("Score: ") + FString::FromInt(score_)));
}

void UPlayerHUDWidget::SetPlayerCurrentLevelText(int& level_)
{
	if (CurrentLevelText) CurrentLevelText->SetText(FText::FromString(TEXT("Level ") + FString::FromInt(level_)));
}

void UPlayerHUDWidget::SetProgressBarEXP(float& currentEXP_, float& maxEXP_)
{
	if (LevelProgressBar) LevelProgressBar->SetPercent(currentEXP_ / maxEXP_);
}

void UPlayerHUDWidget::SetProgressToNextLevelText(int& currentEXP_, int& maxEXP_)
{
	if (ProgressToNextLevelText)
	{
		ProgressToNextLevelText->SetText(FText::FromString(FString::FromInt(currentEXP_) + TEXT("/") +
			FString::FromInt(maxEXP_)));
	}
}

void UPlayerHUDWidget::DestroyEXPRelatedUI()
{
	// Remove and release all EXP related UI
	if (CurrentLevelText)
	{
		CurrentLevelText->RemoveFromParent();
		CurrentLevelText->ConditionalBeginDestroy();
		CurrentLevelText = nullptr;
	}

	if (LevelProgressBar)
	{
		LevelProgressBar->RemoveFromParent();
		LevelProgressBar->ConditionalBeginDestroy();
		LevelProgressBar = nullptr;
	}

	if (ProgressToNextLevelText)
	{
		ProgressToNextLevelText->RemoveFromParent();
		ProgressToNextLevelText->ConditionalBeginDestroy();
		ProgressToNextLevelText = nullptr;
	}
}
