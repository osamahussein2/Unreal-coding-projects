// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseUIWidget.h"
#include "UI/UIManager.h"
#include "Kismet/GameplayStatics.h"


void UBaseUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
	OnWidgetInitialized();
}

void UBaseUIWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UBaseUIWidget::TriggerHide_Implementation()
{
	OwningManager->OnTriggerHideFinished();
}

void UBaseUIWidget::OnWidgetShown_Implementation()
{
	// Override in child C++ or BP
	PlayShowAnimation();
}

void UBaseUIWidget::OnWidgetHidden_Implementation()
{
	// Override in child C++ or BP
	PlayHideAnimation();
}

void UBaseUIWidget::OnWidgetInitialized_Implementation()
{
	// Override in child C++ or BP
}



class APlayerController* UBaseUIWidget::GetOwningPlayerController() const
{
	return GetOwningPlayer();
}

class AGameModeBase* UBaseUIWidget::GetOwiningGameMode() const
{
	return UGameplayStatics::GetGameMode(GetWorld());
}

// class APlayerState* UBaseUIWidget::GetOwningPlayerState() const
// {
// 	APlayerController* PC = GetOwningPlayerController();
// 	return PC ? PC->GetPlayerState<APlayerState>() : nullptr;
// }

// class APawn* UBaseUIWidget::GetOwningPlayerPawn() const
// {
// 	APlayerController* PC = GetOwningPlayerController();
// 	return PC ? PC->GetPawn() : nullptr;
// }
