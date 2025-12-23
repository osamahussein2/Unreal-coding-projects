// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "OptionsMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCOREJAM37_API UOptionsMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UOptionsMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// Clicked button functions
	UFUNCTION()
	void GoBackToMainMenu();

	UFUNCTION()
	void LowerDifficulty();

	UFUNCTION()
	void IncreaseDifficulty();

	// Hovered button functions
	UFUNCTION()
	void HoverOnBackButton();

	UFUNCTION()
	void HoverOnLeftDifficultyArrowButton();

	UFUNCTION()
	void HoverOnRightDifficultyArrowButton();

	// Music volume slider function
	UFUNCTION()
	void UpdateMusicVolumeSlider(float musicVolumeValue_);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UCanvasPanel* OptionsMenuCanvas;

	// Back button
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* backButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftBackImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightBackImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* BackText;

	// Difficulty area
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UHorizontalBox* DifficultyHorizontalBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* LeftDifficultyArrowButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DifficultyTitleText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentDifficultyText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RightDifficultyArrowButton;

	// Music volume slide
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* MusicVolumeSlider;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MusicVolumePercentageText;
};
