// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MobileWidget.generated.h"

class UCanvasPanel;
class UButton;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API UMobileWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMobileWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:

	// Button delegates
	UFUNCTION()
	void InteractWithFish();

	UFUNCTION()
	void PauseGame();

	// Update delegates
	UFUNCTION()
	void UpdateInteractWithFishButtonVisibility();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MobileCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* InteractWithFishButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PauseGameButton = nullptr;
};
