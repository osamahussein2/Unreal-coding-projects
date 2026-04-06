// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HowToUnlockDoorWidget.generated.h"

class UOverlay;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API UHowToUnlockDoorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHowToUnlockDoorWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetInstructionText(FKey& FKey_);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* HowToUnlockDoorOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* HowToUnlockDoorText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InstructionText = nullptr;
};
