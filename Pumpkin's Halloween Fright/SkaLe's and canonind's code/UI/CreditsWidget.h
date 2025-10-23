// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "CreditsWidget.generated.h"


/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UCreditsWidget : public UBaseUIWidget
{
	GENERATED_BODY()

public:
	UCreditsWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "CreditsMenu")
	void OnRestartClicked();

	void FlagEndGameCredits();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "CreditsMenu")
	bool bIsEndGame = false;
};
