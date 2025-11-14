// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EnemyWidget.h"

void UEnemyWidget::SetEnemyDetectionBar(float& value_)
{
	if (EnemyCanvas && EnemyDetectionBar)
	{
		EnemyDetectionBar->SetPercent(value_);
	}
}

UWidgetAnimation* UEnemyWidget::GetBarAnimation() const
{
	return BarAnimation;
}

bool UEnemyWidget::GetIsBarAnimationPlaying() const
{
	return isBarAnimationPlaying;
}

void UEnemyWidget::SetIsBarAnimationPlaying(bool value_)
{
	isBarAnimationPlaying = value_;
}