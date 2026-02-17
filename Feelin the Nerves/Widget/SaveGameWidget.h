// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveGameWidget.generated.h"

class UCanvasPanel;
class UButton;
class UOverlay;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API USaveGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USaveGameWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void GoBackToPauseMenu();

	UFUNCTION()
	void SaveInSlot1();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* SaveGameCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SemiTransparentImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* SaveGameOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SaveSlotButton1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SaveSlotText1;

	// Back button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton;
};
