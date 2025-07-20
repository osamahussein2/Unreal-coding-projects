// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class KENNEYJAM2025_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetHealthProgressBar();

	UFUNCTION()
	void SetPowerProgressBar();

	UFUNCTION()
	void SetEnemyHealthProgressBar();

	UFUNCTION()
	void UpdateTimerText();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PlayerHUDCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* timerImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* timerText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* healthText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* enemyHealthText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* powerText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* healthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* powerBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* enemyHealthBar;
};
