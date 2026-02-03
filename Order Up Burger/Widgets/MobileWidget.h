// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MobileWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UMobileWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMobileWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void SetInteractionText(FString text_);
	void SetDropFoodText(FString text_);

	void ShowInteractionButton();
	void ShowDropFoodButton();

	void HideInteractionButton();
	void HideDropFoodButton();

private:
	UFUNCTION()
	void Interact();

	UFUNCTION()
	void DropFood();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MobileCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* InteractButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InteractText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* DropFoodButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DropFoodText = nullptr;
};
