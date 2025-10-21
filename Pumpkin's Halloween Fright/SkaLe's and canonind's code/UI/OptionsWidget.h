// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "OptionsWidget.generated.h"

class UControlsWidget;
/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UOptionsWidget : public UBaseUIWidget
{
	GENERATED_BODY()

public:
	UOptionsWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void OnControlsClicked();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
	TSubclassOf<UControlsWidget> ControlsWidgetClass;
};
