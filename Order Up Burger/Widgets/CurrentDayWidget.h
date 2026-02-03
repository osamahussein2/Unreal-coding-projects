// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentDayWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UCurrentDayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCurrentDayWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	virtual void NativeDestruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* CurrentDayCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentDayText;

	float alpha = 0.0f;

	float currentDayTimer = 0.0f;

	bool alphaStateChanged = false;

	UPROPERTY(EditAnywhere, Category = "Text Fade Time")
	float maxCurrentDayTimer = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Text Fade Time")
	float alphaFadeSpeed = 0.5f;
};
