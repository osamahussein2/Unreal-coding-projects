// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHROOMJAM2025_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void SetEnemyDetectionBar(float& value_);

	UWidgetAnimation* GetBarAnimation() const;

	bool GetIsBarAnimationPlaying() const;
	void SetIsBarAnimationPlaying(bool value_);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* EnemyCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* EnemyDetectionBar;

	UPROPERTY(meta = (BindWidgetAnim, allowPrivateAccess = true), Transient)
	UWidgetAnimation* BarAnimation;

	bool isBarAnimationPlaying = false;
};
