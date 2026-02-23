// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MarkerWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class EHGAMEJAM_API UMarkerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMarkerWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void UpdateDistanceText(FVector location_, FVector otherLocation_);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UVerticalBox* MarkerVerticalBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* MarkerImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DistanceToMarkerText;
};
