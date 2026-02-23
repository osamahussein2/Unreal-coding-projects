// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputWidget.generated.h"

class UOverlay;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API UInputWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInputWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* InputOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InputText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UTextBlock* ActionText = nullptr;
};
