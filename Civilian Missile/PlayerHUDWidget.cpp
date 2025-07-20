// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHUDWidget.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

void UPlayerHUDWidget::SetHealthProgressBar()
{
	if (healthBar) healthBar->SetPercent(float(APlayerCharacter::currentHealth) / float(APlayerCharacter::maxHealth));
}

void UPlayerHUDWidget::SetPowerProgressBar()
{
	if (powerBar) powerBar->SetPercent(float(APlayerCharacter::currentPower) / float(APlayerCharacter::maxPower));
}

void UPlayerHUDWidget::SetEnemyHealthProgressBar()
{
	if (enemyHealthBar) enemyHealthBar->SetPercent(float(AEnemyCharacter::currentEnemyHealth) / 
		float(AEnemyCharacter::maxEnemyHealth));
}

void UPlayerHUDWidget::UpdateTimerText()
{
	if (timerText) timerText->SetText(FText::FromString(FString::FromInt(APlayerCharacter::currentTime)));
}