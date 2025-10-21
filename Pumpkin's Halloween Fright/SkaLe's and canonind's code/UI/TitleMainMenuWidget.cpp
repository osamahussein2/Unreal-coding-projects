// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleMainMenuWidget.h"
#include "UI/UIManager.h"
#include "UI/OptionsWidget.h"
#include "UI/CreditsWidget.h"
#include "Core/HJGameModeBase.h"

UTitleMainMenuWidget::UTitleMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bShouldHideLowerWidget = true;
	bAllowsPause = false;
}

void UTitleMainMenuWidget::OnPlayClicked()
{
	AHJGameModeBase* GM = Cast<AHJGameModeBase>(GetOwiningGameMode());
	GM->StartGame();
}

void UTitleMainMenuWidget::OnOptionsClicked()
{
	if (OwningManager)
	{
		OwningManager->PushWidget(OptionsWidgetClass);
	}
}

void UTitleMainMenuWidget::OnCreditsClicked()
{
	if (OwningManager)
	{
		OwningManager->PushWidget(CreditsWidgetClass);
	}
}

void UTitleMainMenuWidget::OnQuitClicked()
{
	AHJGameModeBase* GM = Cast<AHJGameModeBase>(GetOwiningGameMode());
	GM->EndGame();
}
