// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NerveSequenceWidget.generated.h"

class UCanvasPanel;
class UOverlay;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UNerveSequenceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UNerveSequenceWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void ShowNerveImage();

	UFUNCTION()
	void UpdateNerveImageSequence();

	UFUNCTION()
	void ShowPickNervesWidget();

private:
	// Widget components
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* NerveSequenceCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* NerveSequenceOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* NerveImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CurrentSequenceNumberText = nullptr;

	// Nerve update parameters
	UPROPERTY(EditAnywhere, Category = "Nerve Update Parameters")
	float timeToUpdateNerveImage = 1.5f;

	FTimerHandle nerveTimerHandle;
};
