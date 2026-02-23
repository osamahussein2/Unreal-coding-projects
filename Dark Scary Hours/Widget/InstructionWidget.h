// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InstructionWidget.generated.h"

class UOverlay;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class EHGAMEJAM_API UInstructionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInstructionWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* InstructionOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* KeyText;
};
