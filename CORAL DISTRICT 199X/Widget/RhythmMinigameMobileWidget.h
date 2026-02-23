// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RhythmMinigameMobileWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;
class UOverlay;
class UFallingImageWidget;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API URhythmMinigameMobileWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	URhythmMinigameMobileWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

private:
	void SpawnFallingImagesForA();
	void SpawnFallingImagesForS();
	void SpawnFallingImagesForD();
	void SpawnFallingImagesForF();

	void UpdateFallingImagesForA();
	void UpdateFallingImagesForS();
	void UpdateFallingImagesForD();
	void UpdateFallingImagesForF();

	// Functions for button press
	UFUNCTION()
	void PressOnFallingImageA();

	UFUNCTION()
	void PressOnFallingImageS();

	UFUNCTION()
	void PressOnFallingImageD();

	UFUNCTION()
	void PressOnFallingImageF();

	UFUNCTION()
	void PauseGame();

	// Function to determine enemy score
	UFUNCTION()
	void DetermineEnemyScore();

	UFUNCTION()
	void StartDancing();

	// Sounds functions
	void PlayMissNoteSound();
	void PlayHitNoteSound();

	UFUNCTION()
	void PlayPlayerMissedNoteDance();

	UFUNCTION()
	void EndMinigame();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* RhythmMinigameCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* RhythmMinigameOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* AButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* DButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* FButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PauseGameButton = nullptr;

	// Player and enemy score texts
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* EnemyText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerScoreText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* EnemyScoreText = nullptr;

	// Spawn falling images for different inputs
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UFallingImageWidget> fallingImageClass;

	UPROPERTY(VisibleAnywhere)
	TArray<UFallingImageWidget*> imagesForA;

	UPROPERTY(VisibleAnywhere)
	TArray<UFallingImageWidget*> imagesForS;

	UPROPERTY(VisibleAnywhere)
	TArray<UFallingImageWidget*> imagesForD;

	UPROPERTY(VisibleAnywhere)
	TArray<UFallingImageWidget*> imagesForF;

	float spawnTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn Times")
	float maxSpawnTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn Locations")
	FMargin imageAPadding = FMargin(50.0f, -100.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Spawn Locations")
	FMargin imageSPadding = FMargin(50.0f, -100.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Spawn Locations")
	FMargin imageDPadding = FMargin(50.0f, -100.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Spawn Locations")
	FMargin imageFPadding = FMargin(50.0f, -100.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Spawn Locations")
	float maxYThreshold = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn Locations")
	float maxDistanceToTexts = 800.0f;

	FTimerHandle enemyIncreaseScoreHandler;

	UPROPERTY(EditAnywhere, Category = "Wait Times")
	float timeToUpdateEnemyScore = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Wait Times")
	float timeToStartDancing = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Score Parameters")
	int maxScoreToIncrement = 10;

	UPROPERTY(EditAnywhere, Category = "Score Parameters")
	float distanceOffset = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Score Parameters")
	float distanceOffsetForPerfectScore = 10.0f;

	int fallingImagesSpawned = 0;

	UPROPERTY(EditAnywhere, Category = "Minigame Parameters")
	int maxFallingImagesSpawned = 30;

	UPROPERTY(EditAnywhere, Category = "Note Sounds")
	TArray<USoundBase*> noteSounds; // 0 for miss note sound, 1 for hit note sound

	float minigameFinishedTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Wait Times")
	float timeToEndMinigame = 3.0f;

	float currentMusicVolume = 1.0f;
	float currentSFXVolume = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Ready Sounds")
	TArray<USoundBase*> readySounds;

	UPROPERTY(EditAnywhere, Category = "Start Sounds")
	TArray<USoundBase*> startSounds;
};
