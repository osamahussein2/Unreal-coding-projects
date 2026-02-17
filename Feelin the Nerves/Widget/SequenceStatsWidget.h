// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SequenceStatsWidget.generated.h"

class UCanvasPanel;
class UOverlay;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API USequenceStatsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USequenceStatsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void DestroyWidgetComponents();

	UFUNCTION()
	void DestroySequenceStats();

	UFUNCTION()
	void SpawnBodyNerves();

	UFUNCTION()
	void ShowNerveSequence();

	UFUNCTION()
	void ShowCompletion();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* SequenceStatsCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* SequenceStatsOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SequenceStatsText = nullptr;

	UPROPERTY(EditAnywhere, Category = "Wait Time Parameters")
	float timeToDestroySequenceStats = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Wait Time Parameters")
	float timeToSpawnBodyNerves = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Wait Time Parameters")
	float timeToShowNerveSequence = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Sequence Sounds")
	TArray<USoundBase*> sequenceSounds; // 0 for correct sequence sound, 1 for error sequence sound
};
