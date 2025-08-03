// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UCanvasPanel;
class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class GMTKJAM2025_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void SetPlayerHealthProgressBar();

	UFUNCTION()
	void SetEnemyHealthProgressBar(float currentHealth_, float maxHealth_);

	UFUNCTION()
	void SetPhaseNumberText();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PlayerHUDCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* PlayerHealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* EnemyHealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PhaseNumberText;

	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> bossFillImages;

	void UpdateBossFillHealth(float index_);
};
