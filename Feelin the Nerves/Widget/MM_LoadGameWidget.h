// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MM_LoadGameWidget.generated.h"

class UCanvasPanel;
class UButton;
class UOverlay;
class UTextBlock;

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UMM_LoadGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMM_LoadGameWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void GoBackToMainMenu();

	UFUNCTION()
	void LoadInSlot1();

	UFUNCTION()
	void ValidateSaveSlot1();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MM_LoadGameCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* MM_LoadGameOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* MM_LoadSlotButton1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MM_LoadSlotText1;

	// Back button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton;
};
