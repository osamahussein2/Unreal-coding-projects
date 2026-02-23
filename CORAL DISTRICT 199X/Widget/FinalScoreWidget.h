// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FinalScoreWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UOverlay;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API UFinalScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFinalScoreWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void ShowReward();

	UFUNCTION()
	void RestartMinigame();

	UFUNCTION()
	void TriggerPlayerWinDance();

	UFUNCTION()
	void TriggerEnemyBreakdance();

	UFUNCTION()
	void RestartRhythmMinigame();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* FinalScoreCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* FinalScoreOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* EnemyText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerScoreText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* EnemyScoreText = nullptr;

	UPROPERTY(EditAnywhere, Category = "Wait Times")
	float timeToShowReward = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Wait Times")
	float timeToRestartMinigame = 3.53f;

	UPROPERTY(EditAnywhere, Category = "Wait Times")
	float timeToTriggerPlayerWinDance = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Win/Lose Sounds")
	TArray<USoundBase*> loseSounds;

	UPROPERTY(EditAnywhere, Category = "Win/Lose Sounds")
	TArray<USoundBase*> winSounds;

	float currentSFXVolume = 1.0;
};
