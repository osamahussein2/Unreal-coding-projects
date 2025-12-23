// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCOREJAM37_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetTimeText(int& minutes_, float& seconds_);
	void SetScoreText(int& score_);

	void SetPlayerCurrentLevelText(int& level_);
	void SetProgressBarEXP(float& currentEXP_, float& maxEXP_);
	void SetProgressToNextLevelText(int& currentEXP_, int& maxEXP_);

	void DestroyEXPRelatedUI();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PlayerHUDCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* verticalBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TimeText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ScoreText;

	// Player level
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentLevelText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* LevelProgressBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ProgressToNextLevelText;
};
