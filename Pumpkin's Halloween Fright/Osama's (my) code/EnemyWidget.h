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
class GDTV_HALLOWEEN2025_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//virtual void NativeConstruct() override;

	UFUNCTION()
	void SetEnemyHealthProgressBar(float currentHealth_, float maxHealth_);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* EnemyCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* EnemyHealthBar;
};
