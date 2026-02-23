// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/FinalScoreWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameMode/BrackeysGM.h"
#include "Player/PlayerCharacter.h"

UFinalScoreWidget::UFinalScoreWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UFinalScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (FinalScoreCanvas && FinalScoreCanvas->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		FinalScoreCanvas->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (FinalScoreOverlay && FinalScoreOverlay->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		FinalScoreOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (BlackImage && BlackImage->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		BlackImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (PlayerText && PlayerText->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		PlayerText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (EnemyText && EnemyText->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		EnemyText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (PlayerScoreText && PlayerScoreText->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		PlayerScoreText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (EnemyScoreText && EnemyScoreText->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		EnemyScoreText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (PlayerScoreText)
	{
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			PlayerScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetPlayerScore())));
		}
	}

	if (EnemyScoreText)
	{
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->PlayMallAmbienceMusic();
			currentSFXVolume = gameMode->GetSFXVolume();
			EnemyScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetEnemyScore())));
		}
	}

	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// If the player wins the minigame by achieving a higher score than the enemy, trigger player win dance
		if (gameMode->GetPlayerScore() > gameMode->GetEnemyScore())
		{
			const int& randomWinSoundIndex = FMath::RandRange(0, winSounds.Num() - 1);
			UGameplayStatics::PlaySound2D(GetWorld(), winSounds[randomWinSoundIndex], currentSFXVolume);

			FTimerHandle rewardHandle;
			GetWorld()->GetTimerManager().SetTimer(rewardHandle, this, &UFinalScoreWidget::TriggerPlayerWinDance, 
				timeToTriggerPlayerWinDance, false);
		}

		// Else if the player lost the minigame, make the enemy breakdance on them
		else if (gameMode->GetPlayerScore() < gameMode->GetEnemyScore())
		{
			const int& randomLoseSoundIndex = FMath::RandRange(0, loseSounds.Num() - 1);
			UGameplayStatics::PlaySound2D(GetWorld(), loseSounds[randomLoseSoundIndex], currentSFXVolume);

			FTimerHandle breakDanceHandle;
			GetWorld()->GetTimerManager().SetTimer(breakDanceHandle, this, &UFinalScoreWidget::TriggerEnemyBreakdance, 
				timeToShowReward, false);
		}

		// Otherwise, the minigame ended in a tie just restart the minigame
		else
		{
			FTimerHandle restartMinigameHandle;
			GetWorld()->GetTimerManager().SetTimer(restartMinigameHandle, this, &UFinalScoreWidget::RestartMinigame, 
				timeToShowReward, false);
		}
	}
}

void UFinalScoreWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (FinalScoreCanvas)
	{
		FinalScoreCanvas->RemoveFromParent();
		FinalScoreCanvas = nullptr;
	}

	if (FinalScoreOverlay)
	{
		FinalScoreOverlay->RemoveFromParent();
		FinalScoreOverlay = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (PlayerText)
	{
		PlayerText->RemoveFromParent();
		PlayerText = nullptr;
	}

	if (EnemyText)
	{
		EnemyText->RemoveFromParent();
		EnemyText = nullptr;
	}

	if (PlayerScoreText)
	{
		PlayerScoreText->RemoveFromParent();
		PlayerScoreText = nullptr;
	}

	if (EnemyScoreText)
	{
		EnemyScoreText->RemoveFromParent();
		EnemyScoreText = nullptr;
	}
}

void UFinalScoreWidget::ShowReward()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (!gameMode->GetIsForMobile()) gameMode->EnableMouseCursor();

		gameMode->EarnReward();

		gameMode->PopWidget(gameMode->GetFinalScoreClass());
		gameMode->PushWidget(gameMode->GetRewardClass());
	}
}

void UFinalScoreWidget::RestartMinigame()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			gameMode->ResetScores();

			gameMode->PopWidget(gameMode->GetFinalScoreClass());
			gameMode->PushWidget(gameMode->GetRhythmMinigameClass());

			player->SetPlayerActionToPlayingRhythmMinigame();
		}
	}
}

void UFinalScoreWidget::TriggerPlayerWinDance()
{
	// Make the player do the win dance
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			gameMode->PopWidget(gameMode->GetFinalScoreClass());

			player->PlayWinDanceAnimation();

			FTimerHandle restartTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(restartTimerHandle, this, &UFinalScoreWidget::ShowReward,
				timeToShowReward, false);
		}
	}
}

void UFinalScoreWidget::TriggerEnemyBreakdance()
{
	// TODO: Enemy breakdance
	// For now, just reset minigame
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			gameMode->PopWidget(gameMode->GetFinalScoreClass());

			gameMode->MakeEnemyBreakDance();

			FTimerHandle restartTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(restartTimerHandle, this, &UFinalScoreWidget::RestartRhythmMinigame,
				timeToRestartMinigame, false);
		}
	}
}

void UFinalScoreWidget::RestartRhythmMinigame()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			gameMode->ResetScores();
			
			gameMode->PushWidget(gameMode->GetRhythmMinigameClass());

			player->SetPlayerActionToPlayingRhythmMinigame();
		}
	}
}
