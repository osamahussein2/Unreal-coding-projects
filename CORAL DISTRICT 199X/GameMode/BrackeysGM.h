// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Widget include header files
#include "Widget/MainMenuWidget.h"
#include "Widget/PauseMenuWidget.h"
#include "Widget/RhythmMinigameWidget.h"
#include "Widget/FinalScoreWidget.h"
#include "Widget/RewardWidget.h"
#include "Widget/CreditsWidget.h"
#include "Widget/MainOptionsMenuWidget.h"
#include "Widget/MobileWidget.h"
#include "Widget/RhythmMinigameMobileWidget.h"

#include "Object/RewardActor.h"
#include "Character/DancingEnemyFish.h"

#include "BrackeysGM.generated.h"

class ACineCameraActor;
class UTouchInterface;
class UAudioComponent;
class ADancingFishNPC;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API ABrackeysGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABrackeysGM();

	// Mouse cursor functions
	void EnableMouseCursor();
	void DisableMouseCursor();

	void PushWidget(TSubclassOf<UUserWidget> widgetClass_);
	void PopWidget(TSubclassOf<UUserWidget> widgetClass_);

	// Widget subclasses getter functions (preventing anyone from modifying the widget subclasses)
	TSubclassOf<UMainMenuWidget> GetMainMenuClass() const;
	TSubclassOf<UPauseMenuWidget> GetPauseMenuClass() const;
	TSubclassOf<URhythmMinigameWidget> GetRhythmMinigameClass() const;
	TSubclassOf<UFinalScoreWidget> GetFinalScoreClass() const;
	TSubclassOf<URewardWidget> GetRewardClass() const;
	TSubclassOf<UCreditsWidget> GetCreditsMenuClass() const;
	TSubclassOf<UMainOptionsMenuWidget> GetMainOptionsMenuClass() const;
	TSubclassOf<UMobileWidget> GetMobileClass() const;
	TSubclassOf<URhythmMinigameMobileWidget> GetRhythmMinigameMobileClass() const;

	// Scoring functions
	int GetPlayerScore() const { return playerScore; }
	int GetEnemyScore() const { return enemyScore; }

	void IncrementPlayerScore(const int& score_);
	void IncrementEnemyScore(const int& score_);

	void ResetGame();
	void ResetScores();

	// Reward functions
	void EarnReward();
	void DestroyReward();

	int GetRewardNumber() const { return rewardNumber; }

	// Dancing enemy fish
	void StartRhythmMinigame();
	void EndRhythmMinigame();

	ADancingEnemyFish* GetDancingEnemyFish() const { return dancingEnemyFish; }

	void MakeEnemyDance();
	void StopEnemyFromDancing();
	void MakeEnemyBreakDance();

	// Menu sound functions
	void PlayMenuOpenSound();
	void PlayMenuCloseSound();

	// Mobile functions
	void ShowGameTouchInterface();
	void InvalidateTouchInterface();

	bool GetIsForMobile() const { return isForMobile; }

	// Music/SFX functions
	float GetMusicVolume() const { return musicVolume; }
	float GetSFXVolume() const { return sfxVolume; }

	void SetMusicVolume(float& value_);
	void SetSFXVolume(float& value_) { sfxVolume = value_; }

	// Play music functions
	void PlayMainMenuMusic();
	void PlayCreditsMenuMusic();
	void PlayMallAmbienceMusic();

	void PlayBattleMusic();

	void ResumeMusic();
	void PauseMusic();

protected:
	virtual void BeginPlay() override;

private:
	void SetDrawDistanceForEveryMesh();
	void SetDanceBattleDrawDistanceForEveryMesh();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<APawn> playerPawnClass;

	// Widget array holding UUserWidget pointers
	UPROPERTY(VisibleAnywhere, Category = "Widgets")
	TArray<UUserWidget*> cachedWidgets;

	// Widget subclasses
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> mainMenuClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPauseMenuWidget> pauseMenuClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<URhythmMinigameWidget> rhythmMinigameClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UFinalScoreWidget> finalScoreClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<URewardWidget> rewardClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UCreditsWidget> creditsMenuClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMainOptionsMenuWidget> mainOptionsMenuClass;

	UPROPERTY(EditAnywhere, Category = "Mobile")
	TSubclassOf<UMobileWidget> mobileClass;

	UPROPERTY(EditAnywhere, Category = "Mobile")
	TSubclassOf<URhythmMinigameMobileWidget> rhythmMinigameMobileClass;

	// Player and enemy scores
	UPROPERTY(VisibleAnywhere, Category = "Game Score")
	int playerScore = 0;

	UPROPERTY(VisibleAnywhere, Category = "Game Score")
	int enemyScore = 0;

	// Player location and rotation
	FVector startingPosition = FVector::ZeroVector;
	FRotator startingRotation = FRotator::ZeroRotator;

	// Reward
	UPROPERTY(EditAnywhere, Category = "Reward")
	TArray<TSubclassOf<ARewardActor>> rewardActorClass;

	int rewardNumber = 0;
	int lastRewardNumber = 0;

	// Dancing scene
	UPROPERTY(VisibleAnywhere, Category = "Dance Minigame")
	ACineCameraActor* danceCamera;

	UPROPERTY(EditAnywhere, Category = "Dance Minigame")
	FVector cameraLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Dance Minigame")
	FRotator cameraRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Dance Minigame")
	TSubclassOf<ADancingEnemyFish> dancingEnemyFishClass;

	UPROPERTY(VisibleAnywhere, Category = "Dance Minigame")
	ADancingEnemyFish* dancingEnemyFish;

	UPROPERTY(EditAnywhere, Category = "Dance Minigame")
	TSubclassOf<ADancingFishNPC> dancingNPCFishClass;

	UPROPERTY(VisibleAnywhere, Category = "Dance Minigame")
	ADancingFishNPC* dancingNPCFish;

	UPROPERTY(EditAnywhere, Category = "Dance Minigame")
	FVector dancingEnemyLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Dance Minigame")
	FRotator dancingEnemyRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Dance Minigame")
	FVector playerDanceLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Dance Minigame")
	FRotator playerDanceRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Dance Minigame")
	FVector dancingNPCLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Dance Minigame")
	FRotator dancingNPCRotation = FRotator::ZeroRotator;

	// Sounds
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* menuOpenSound;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* menuCloseSound;

	// Mobile interface
	UPROPERTY(EditAnywhere, Category = "Mobile")
	UTouchInterface* touchInterface;

	UPROPERTY(EditAnywhere, Category = "Mobile")
	bool isForMobile = false;

	// Music/SFX variables
	UPROPERTY(VisibleAnywhere, Category = "Music/SFX Settings")
	float musicVolume = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Music/SFX Settings")
	float sfxVolume = 1.0f;

	// Music
	UPROPERTY(EditAnywhere, Category = "Music")
	USoundBase* mainMenuMusic;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundBase* creditsMenuMusic;

	UPROPERTY(EditAnywhere, Category = "Music")
	USoundBase* mallAmbienceMusic;

	UPROPERTY(EditAnywhere, Category = "Music")
	TArray<USoundBase*> battleMusics;

	// Audio component
	UPROPERTY(EditAnywhere, Category = "Components")
	UAudioComponent* gameMusic;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh Settings")
	float maxDrawDistance = 2000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh Settings")
	float maxDrawDistanceDuringDanceBattle = 5000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ambience Settings")
	float mallAmbienceVolumeMultiplier = 5.0f;
};
