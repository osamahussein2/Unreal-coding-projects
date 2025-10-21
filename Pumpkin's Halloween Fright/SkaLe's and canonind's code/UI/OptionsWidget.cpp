// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionsWidget.h"

#include "UI/UIManager.h"
#include "UI/ControlsWidget.h"

UOptionsWidget::UOptionsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowsPause = false;
	bShowMouseCursor = true;
	bHideOnPauseToggle = true;
}

void UOptionsWidget::OnControlsClicked()
{
	if (OwningManager)
	{
		OwningManager->PushWidget(ControlsWidgetClass);
	}
}
