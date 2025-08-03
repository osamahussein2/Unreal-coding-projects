// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHUDWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerCharacter.h"

void UPlayerHUDWidget::SetPlayerHealthProgressBar()
{
	if (PlayerHealthBar)
	{
		PlayerHealthBar->SetPercent(float(APlayerCharacter::GetPlayerHealth()) / 
			float(APlayerCharacter::GetPlayerMaxHealth()));
	}
}

void UPlayerHUDWidget::SetEnemyHealthProgressBar(float currentHealth_, float maxHealth_)
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetPercent(currentHealth_ / maxHealth_);
	}
}

void UPlayerHUDWidget::SetPhaseNumberText()
{
	if (PhaseNumberText)
	{
		PhaseNumberText->SetText(FText::FromString("Phase " + FString::FromInt(APlayerCharacter::phaseNumber)));
	}
}

void UPlayerHUDWidget::UpdateBossFillHealth(float index_)
{
	if (EnemyHealthBar)
	{
		// Initialize fill brush image
		FSlateBrush FillBrush;

		if (FillBrush.GetResourceObject() != bossFillImages[index_])
			FillBrush.SetResourceObject(bossFillImages[index_]);

		if (FillBrush.GetImageSize() != FVector2D(100, 25)) FillBrush.SetImageSize(FVector2D(100, 25));
		if (FillBrush.DrawAs != ESlateBrushDrawType::Box) FillBrush.DrawAs = ESlateBrushDrawType::Box;
		if (FillBrush.Margin != FMargin(0.416667f)) FillBrush.Margin = FMargin(0.416667f);

		// Set the fill image
		if (EnemyHealthBar->GetWidgetStyle().FillImage != FillBrush)
			EnemyHealthBar->WidgetStyle.SetFillImage(FillBrush);

		// Use the modified widget style
		EnemyHealthBar->SetWidgetStyle(EnemyHealthBar->GetWidgetStyle());
	}
}