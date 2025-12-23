// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/PlayerInputDataAsset.h"
#include "GameFramework/PlayerController.h"
#include "CineCameraActor.h"
#include "Widgets/MainMenuWidget.h"
#include "Widgets/LeaderboardWidget.h"
#include "Widgets/PauseMenuWidget.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Widgets/GameOverWidget.h"
#include "Widgets/UpgradeMenuWidget.h"
#include "Widgets/OptionsMenuWidget.h"
#include "Widgets/CreditsMenuWidget.h"
#include "PlayerCharacter.generated.h"

USTRUCT()
struct FPlayerLeaderboardStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FText playerName;

	UPROPERTY(VisibleAnywhere)
	int playerScore;

	// Overload the < operator to sort order
	bool operator<(const FPlayerLeaderboardStats& Other) const
	{
		// Sort in descending order (highest score first)
		return playerScore > Other.playerScore;
	}
};

/**
 * 
 */
UCLASS()
class SCOREJAM37_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PauseGame();
	void ShootBall();

public:
	// Player leaderboard functions
	int GetPlayerScores(int& index_) const;
	FText GetPlayerNames(int& index_) const;

	bool GetNamesReceived(int& index_) const;

	void SetPlayerName(FText playerName_, int& index_);
	void SetPlayerScore(int& playerScore_);

	void IncrementPlayerScore();

	int GetPlayerLeaderboardStatsSize() const;

	void AddPlayerInformation();
	void DeleteCurrentPlayerInformation();

	void DeleteAllLeaderboardInformation();

public:
	void SetPlayerViewTarget();

	// Mouse cursor functions
	void EnableMouseCursor();
	void DisableMouseCursor();

	// Resetting game function
	void ResetGame();

	// Soccer ball related functions
	void FindSoccerBall();
	void InvalidateSoccerBall();

	// Player upgrade functions
	int GetUpgradePoint() const { return upgradePoint; }

	void DecreaseUpgradePoint(int value_);

	int GetBallSpeedUpgradeIndex() const { return ballSpeedUpgradeIndex; }
	int GetGoalkeeperSpeedUpgradeIndex() const { return goalkeeperSpeedUpgradeIndex; }
	int GetEXPMultiplierUpgradeIndex() const { return EXPMultiplierUpgradeIndex; }

	void UpgradeBallSpeed();
	void UpgradeGoalkeeperSpeed();
	void UpgradeEXPMultiplier();

	// Difficulty related functions
	int GetCurrentDifficultyIndex() const { return currentDifficultyIndex; }

	void IncrementCurrentDifficultyIndex();
	void DecrementCurrentDifficultyIndex();

	void UpdateVariablesBasedOnCurrentDifficulty();

	// Function for updating shot indicator values
	FRotator GetAnimateRightRotation() const { return animateRightRotation; }
	FRotator GetAnimateLeftRotation() const { return animateLeftRotation; }
	float GetTimeToFlipRotation() const { return timeToFlipRotation; }

	// Functions for getting EXP values
	int GetCurrentEXP() const { return currentEXP; }
	int GetMaxEXP() const { return currentEXP; }
	int GetCurrentLevel() const { return currentEXP; }

	void ChangeEXPValuesDependingOnCurrentLevel();
	void ResetCountingEXPStats();

public:
	void ShowMainMenu(bool worryAboutMouseCursor);
	void ShowLeaderboardMenu();
	void ShowPauseMenu();

	void ShowPlayerHUD();
	void HidePlayerHUD();

	void ShowGameOver();
	void ShowUpgradeMenu();
	void ShowOptionsMenu();
	void ShowCreditsMenu();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> mainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<ULeaderboardWidget> leaderboardClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPauseMenuWidget> pauseMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UGameOverWidget> gameOverClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUpgradeMenuWidget> upgradeMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UOptionsMenuWidget> optionsMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UCreditsMenuWidget> creditsMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPlayerHUDWidget> playerHUDClass;

	UPlayerHUDWidget* playerHUD;

private:
	// Input objects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* inputMapping;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UPlayerInputDataAsset* PlayerDataAsset;
	
	// Get the cine camera actor found in level
	ACineCameraActor* cineCameraActor;

	UPROPERTY(EditDefaultsOnly)
	float initialWalkSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly)
	float playerSpeed = 0.5f;

	// Timer
	int minutes = 1;
	float seconds = 1.0f;

	// Player leaderboard
	UPROPERTY(VisibleAnywhere)
	TArray<FPlayerLeaderboardStats> playerInformations;

	UPROPERTY(EditAnywhere)
	int maximumScoreIndex = 9;

	int currentGameIndex = 0;

	// Soccer ball variable
	class ASoccerBall* soccerBall = nullptr;

	// Player upgrades
	int upgradePoint = 0;

	int ballSpeedUpgradeIndex = 1;
	int goalkeeperSpeedUpgradeIndex = 1;
	int EXPMultiplierUpgradeIndex = 1;

	// Difficulty
	int currentDifficultyIndex = 1; // 0 for Easy, 1 for Normal and 2 for Hard

	UPROPERTY(VisibleAnywhere, Category = "Shot Indicator Parameters")
	FRotator animateRightRotation;

	UPROPERTY(VisibleAnywhere, Category = "Shot Indicator Parameters")
	FRotator animateLeftRotation;

	UPROPERTY(VisibleAnywhere, Category = "Shot Indicator Parameters")
	float timeToFlipRotation = 1.0f;

	// Experience points
	int currentEXP = 0;
	int EXPMultiplier = 1;
	int EXPToGiveBeforeLevelingUp = 0;
	int maxEXP = 50;

	int currentLevel = 1;

	int lastEXP = 0;
	int lastLevel = 0;

	int lastUpgradePoint = 0;
};
