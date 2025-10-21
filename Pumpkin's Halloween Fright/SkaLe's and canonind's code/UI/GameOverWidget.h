// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UGameOverWidget : public UBaseUIWidget
{
	GENERATED_BODY()

public:
	UGameOverWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "GameOver Menu")
	void OnRestartClicked();

	UFUNCTION(BlueprintCallable, Category = "GameOver Menu")
	void OnQuitToMainMenu();

protected:
	virtual void NativeConstruct() override;
	virtual void OnWidgetShown_Implementation() override;
	virtual void OnWidgetHidden_Implementation() override;
};
