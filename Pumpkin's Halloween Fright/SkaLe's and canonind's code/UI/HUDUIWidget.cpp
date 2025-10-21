// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDUIWidget.h"

void UHUDUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHUDUIWidget::UpdateHealth(float HealthPercent)
{
	OnHealthChanged(HealthPercent);
}

void UHUDUIWidget::UpdateStamina(float StaminaPercent)
{
	OnStaminaChanged(StaminaPercent);
}

void UHUDUIWidget::UpdateCandyCount(float Candies)
{
	OnCandyCountChanged(Candies);
}
