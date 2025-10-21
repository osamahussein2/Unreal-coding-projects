// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CreditsWidget.h"

UCreditsWidget::UCreditsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowsPause = false;
	bShowMouseCursor = true;
	bHideOnPauseToggle = false;
	bPauseGameWhenShown = true;
}
