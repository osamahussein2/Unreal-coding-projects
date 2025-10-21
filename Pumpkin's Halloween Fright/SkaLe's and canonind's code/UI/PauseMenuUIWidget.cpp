// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenuUIWidget.h"
#include "UI/UIManager.h"
#include "UI/OptionsWidget.h"
#include "Core/HJGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/PlayerController.h"

UPauseMenuUIWidget::UPauseMenuUIWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bPauseGameWhenShown = true;
	bShowMouseCursor = true;
	bShouldHideLowerWidget = true;
	bHideOnPauseToggle = false; // This shouldn't be used in pause widget
}

void UPauseMenuUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetupInputBindings();
}

void UPauseMenuUIWidget::OnWidgetHidden_Implementation()
{
	Super::OnWidgetShown_Implementation();
	OnMenuOpened();
}

void UPauseMenuUIWidget::OnWidgetShown_Implementation()
{
	Super::OnWidgetHidden_Implementation();
	OnMenuClosed();
}

void UPauseMenuUIWidget::SetupInputBindings()
{
	SetIsFocusable(true);
}


void UPauseMenuUIWidget::OnResumeClicked()
{
	AHJGameModeBase* GM = Cast<AHJGameModeBase>(GetOwiningGameMode());
	GM->TogglePause();
}

void UPauseMenuUIWidget::OnRestartClicked()
{
	// THIS FUNCTION IS NOT USED
	// If we ever need it
// 	UGameplayStatics::SetGamePaused(GetWorld(), false);
// 
// 	APlayerController* PC = GetOwningPlayerController();
// 	if (PC)
// 	{
// 		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
// 	}
}

void UPauseMenuUIWidget::OnSettingsClicked()
{
	if (OwningManager)
	{
		OwningManager->PushWidget(OptionsWidgetClass);
	}
}

void UPauseMenuUIWidget::OnMainMenuClicked()
{
	AHJGameModeBase* GM = Cast<AHJGameModeBase>(GetOwiningGameMode());
	GM->GoToMainMenu();
}

void UPauseMenuUIWidget::OnQuitClicked()
{
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
	}
}






