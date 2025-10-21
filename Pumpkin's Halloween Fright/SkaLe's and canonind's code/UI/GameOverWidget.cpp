// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOverWidget.h"
#include "UI/UIManager.h"
#include "Core/HJGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"


UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bPauseGameWhenShown = true;
	bAllowsPause = false;

}

void UGameOverWidget::OnRestartClicked()
{
	AHJGameModeBase* GM = Cast<AHJGameModeBase>(GetOwiningGameMode());
	GM->RestartGame();
}

void UGameOverWidget::OnQuitToMainMenu()
{
	AHJGameModeBase* GM = Cast<AHJGameModeBase>(GetOwiningGameMode());
	GM->GoToMainMenu();
}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
}

void UGameOverWidget::OnWidgetShown_Implementation()
{
	Super::OnWidgetShown_Implementation();
}
void UGameOverWidget::OnWidgetHidden_Implementation()
{
	Super::OnWidgetHidden_Implementation();

}

