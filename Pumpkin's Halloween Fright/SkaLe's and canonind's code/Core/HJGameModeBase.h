// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/BaseUIWidget.h"
#include "HJGameModeBase.generated.h"

class AUIManager;
class ALightAnchorsSystem;

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API AHJGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void OnPlayerDied();
	void TogglePause();
	void OpenDialogueSystem();
	void ShowCredits();

	virtual void RestartGame();
	virtual void StartGame();
	virtual void EndGame();
	virtual void GoToMainMenu();

	virtual bool LoadNextLevel();
	virtual void ResetLevels(bool bBlock);

	virtual void PlayEndSequence();

	UFUNCTION(BlueprintCallable)
	void PlayDialogueLevel2();

	void ActivateAnchorsForSolvedPuzzle(int32 PuzzleId);
	void ResetLightAnchorsSections();
	void ActivateInitialAnchors();


protected:
	void SetPlayerHUD(UBaseUIWidget* HUDWidget);

	UFUNCTION()
	void ShowGameOverScreen();

	virtual void OnUIManagerReady(AUIManager* UIManager);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Widget Classes")
	TSubclassOf<UBaseUIWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Widget Classes")
	TSubclassOf<UBaseUIWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Widget Classes")
	TSubclassOf<UBaseUIWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Widget Classes")
	TSubclassOf<UBaseUIWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Widget Classes")
	TSubclassOf<UBaseUIWidget> CreditsWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Widget Classes")
	TSubclassOf<UBaseUIWidget> DialogueWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Widget Classes")
	TObjectPtr<UDataTable> DialogueDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Level Loading")
	TArray<FName> LevelsList;

	UPROPERTY(VisibleAnywhere, Category = "Level Loading")
	int32 NextLevelIndex;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	float GameOverScreenDelay = 2.5f;

	UPROPERTY()
	TObjectPtr<ALightAnchorsSystem> LightAnchorSystemCached = nullptr;


	UPROPERTY()
	TObjectPtr<AUIManager> CachedUIManager;

	bool bIsPaused = false;
};
