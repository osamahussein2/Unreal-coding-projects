// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HJGameModeBase.h"
#include "UI/UIManager.h"
#include "UI/GameOverWidget.h"
#include "UI/CreditsWidget.h"
#include "UI/DialogueSystemWidget.h"
#include "Characters/HJPlayerCharacter.h"
#include "LightAnchor.h"
#include "LightAnchorsSystem.h"

#include "Kismet/GameplayStatics.h"

void AHJGameModeBase::OnPlayerDied()
{
	FTimerHandle MemberTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		MemberTimerHandle,
		this,
		&AHJGameModeBase::ShowGameOverScreen,
		GameOverScreenDelay,
		false
	);
}

void AHJGameModeBase::RestartGame()
{
	// BROKEN - DON"T USE
	if (LevelsList.Num() <= 0) return;


	FString LevelShortName = LevelsList[0].ToString();
	FString FullLevelPath = FString::Printf(TEXT("/Game/Maps/%s"), *LevelShortName);
	FName LevelToOpen = FName(*FullLevelPath);

	if (!LevelToOpen.IsNone())
	{
		UGameplayStatics::OpenLevel(
			this,
			LevelToOpen,
			true,
			TEXT("")
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not construct a valid path for level: %s"), *LevelShortName);
	}
}

void AHJGameModeBase::TogglePause()
{
	if (!CachedUIManager->GetTopWidget())
	{
		return;
	}
	// Fixes pause toggle in options from main menu where game is not paused
	if (CachedUIManager->ShouldHideTopWidgetOnPauseToggleInput())
	{
		CachedUIManager->HideTopWidget();
		return;
	}

	if (bIsPaused)
	{
		CachedUIManager->HideTopWidget();
	}
	else
	{

		if (CachedUIManager->DoesTopWidgetAllowPause())
		{
			CachedUIManager->PushWidget(PauseMenuWidgetClass);
		}
		else
		{
			return;
		}
	}
	bIsPaused = !bIsPaused;
}

void AHJGameModeBase::OpenDialogueSystem()
{
	if (CachedUIManager && DialogueWidgetClass)
	{
		UDialogueSystemWidget* DialogueWidget = Cast<UDialogueSystemWidget>(CachedUIManager->PushWidget(DialogueWidgetClass));

		if (DialogueWidget)
		{
			DialogueWidget->DialogueTable = DialogueDataTable;
			DialogueWidget->StartDialogueByID("Intro");
		}
	}
}

void AHJGameModeBase::ShowCredits()
{
	if (CachedUIManager && CreditsWidgetClass)
	{
		UCreditsWidget* CreditsWidget = Cast<UCreditsWidget>(CachedUIManager->PushWidget(CreditsWidgetClass));
		CreditsWidget->FlagEndGameCredits();
	}
}

void AHJGameModeBase::StartGame()
{
	// Override in child
}

void AHJGameModeBase::EndGame()
{
	// OVerride in child
}

void AHJGameModeBase::GoToMainMenu()
{

}

bool AHJGameModeBase::LoadNextLevel()
{
	// Override in child
	return false;
}

void AHJGameModeBase::ResetLevels(bool bBlock)
{

}



void AHJGameModeBase::PlayEndSequence()
{

}

void AHJGameModeBase::PlayDialogueLevel2()
{
	if (CachedUIManager && DialogueWidgetClass)
	{
		UDialogueSystemWidget* DialogueWidget = Cast<UDialogueSystemWidget>(CachedUIManager->PushWidget(DialogueWidgetClass));

		if (DialogueWidget)
		{
			DialogueWidget->DialogueTable = DialogueDataTable;
			DialogueWidget->StartDialogueByID("Level2");
		}
	}
}

void AHJGameModeBase::ActivateAnchorsForSolvedPuzzle(int32 PuzzleId)
{
	if (LightAnchorSystemCached) LightAnchorSystemCached->ActivateAnchorsForSolvedPuzzle(PuzzleId);
}


void AHJGameModeBase::ResetLightAnchorsSections()
{
	if (LightAnchorSystemCached) LightAnchorSystemCached->ResetLightAnchorsSections();
}


void AHJGameModeBase::ActivateInitialAnchors()
{
	LightAnchorSystemCached->ActivateInitialAnchors();
}

void AHJGameModeBase::OnUIManagerReady(AUIManager* UIManager)
{
	CachedUIManager = UIManager;
}

void AHJGameModeBase::SetPlayerHUD(UBaseUIWidget* HUDWidget)
{
	UWorld* World = GetWorld();
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	if (PC)
	{
		APawn* PlayerPawn = PC->GetPawn();
		if (!PlayerPawn)
		{
			UE_LOG(LogTemp, Warning, TEXT("AHJGameModeBase::SetPlayerHUD: Failed to retrieve player character"));
			return;
		}
		AHJPlayerCharacter* PlayerCharacter = Cast<AHJPlayerCharacter>(PlayerPawn);
		if (!PlayerCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("AHJGameModeBase::SetPlayerHUD: Failed to retrieve player character"));
			return;
		}
		PlayerCharacter->SetPlayerHUD(HUDWidget);
	}
}

void AHJGameModeBase::ShowGameOverScreen()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	AUIManager* Manager = Cast<AUIManager>(PC->GetHUD());
	if (!Manager) return;

	if (!GameOverWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Game Over Widget is not set"))
	}

	Manager->PushWidget(GameOverWidgetClass);

}
