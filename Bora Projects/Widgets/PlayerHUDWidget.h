// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHROOMJAM2025_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	void SetHealthBar(float currentHealth, float maxHealth);
	void UpdateHealthBar(float updatedHealth, float maxHealth);

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PlayerHUDCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* minimapImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* CurrentHealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* DynamicHealthBar;
};
