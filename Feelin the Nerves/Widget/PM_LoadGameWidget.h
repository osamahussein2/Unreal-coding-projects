// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PM_LoadGameWidget.generated.h"

class UCanvasPanel;
class UButton;
class UOverlay;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UPM_LoadGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPM_LoadGameWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void GoBackToPauseMenu();

	UFUNCTION()
	void LoadInSlot1();

	UFUNCTION()
	void ValidateSaveSlot1();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PM_LoadGameCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SemiTransparentImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* PM_LoadGameOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PM_LoadSlotButton1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PM_LoadSlotText1;

	// Back button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton;
};
