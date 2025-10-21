// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "HUDUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UHUDUIWidget : public UBaseUIWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealth(float HealthPercent);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateStamina(float StaminaPercent);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateCandyCount(float Candies);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OnHealthChanged(float HealthPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OnStaminaChanged(float StaminaPercent);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OnCandyCountChanged(float CandyCount);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
