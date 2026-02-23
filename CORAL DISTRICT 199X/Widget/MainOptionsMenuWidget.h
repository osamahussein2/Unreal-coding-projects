// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainOptionsMenuWidget.generated.h"

class UCanvasPanel;
class UButton;
class USlider;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API UMainOptionsMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainOptionsMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Clicked button functions
	UFUNCTION()
	void GoBackToMainMenu();

	UFUNCTION()
	void ApplyOptions();

	// Music volume slider function
	UFUNCTION()
	void UpdateMusicVolumeSlider(float musicVolumeValue_);

	// SFX volume slider function
	UFUNCTION()
	void UpdateSFXVolumeSlider(float musicVolumeValue_);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* OptionsMenuCanvas = nullptr;

	// Back button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton = nullptr;

	// Apply button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ApplyButton = nullptr;

	// Music volume slider
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* MusicVolumeSlider = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MusicVolumePercentageText = nullptr;

	// Music volume slider
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* SFXVolumeSlider = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SFXVolumePercentageText = nullptr;

	float lastMusicVolume = 0.0f;
	float lastSFXVolume = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Options Menu Sounds")
	USoundBase* menuCancel = nullptr;

	UPROPERTY(EditAnywhere, Category = "Options Menu Sounds")
	USoundBase* menuConfirm = nullptr;

	UPROPERTY(EditAnywhere, Category = "Options Menu Sounds")
	USoundBase* menuOptionChange = nullptr;
};
