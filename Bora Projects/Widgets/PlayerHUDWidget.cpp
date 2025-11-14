// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHUDWidget.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerHUDWidget::SetHealthBar(float currentHealth, float maxHealth)
{
	if (CurrentHealthBar)
	{
		CurrentHealthBar->SetPercent(currentHealth / maxHealth);
	}
}

void UPlayerHUDWidget::UpdateHealthBar(float updatedHealth, float maxHealth)
{
	if (DynamicHealthBar)
	{
		DynamicHealthBar->SetPercent(updatedHealth / maxHealth);
	}
}