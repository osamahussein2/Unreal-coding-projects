// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ControlsWidget.h"

UControlsWidget::UControlsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowsPause = false;
	bShowMouseCursor = true;
	bHideOnPauseToggle = true;
}
