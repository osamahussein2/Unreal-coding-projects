// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/WidgetManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/MainMenuWidget.h"

AWidgetManager::AWidgetManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;
}

void AWidgetManager::BeginPlay()
{
	Super::BeginPlay();

	PushWidget(mainMenuClass);
	EnableMouseCursor();
}

void AWidgetManager::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ClearWidgetStack();
}

void AWidgetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWidgetManager::EnableMouseCursor()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false); // Don't hide the cursor during capture
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		playerController->SetInputMode(InputMode);

		if (playerController->bShowMouseCursor != true) playerController->bShowMouseCursor = true;
	}
}

void AWidgetManager::DisableMouseCursor()
{
	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		FInputModeGameOnly GameInputMode;
		GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

		playerController->SetInputMode(GameInputMode);

		// Make sure the mouse cursor is turned off
		if (playerController->bShowMouseCursor != false) playerController->bShowMouseCursor = false;
	}
}

void AWidgetManager::PushWidget(TSubclassOf<UUserWidget> widgetClass_)
{
	if (!IsValid(widgetClass_) || DoesWidgetExist(widgetClass_)) return;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	// Add whatever widget subclass is passed in into the widget stack array
	widgetStack.Add(CreateWidget<UUserWidget>(playerController, widgetClass_));

	// Set the last index to be the last added element in the widget stack array
	const int& lastIndex = widgetStack.Num() - 1;

	// If the widget stack at the last pushed index is valid, add it to viewport
	if (widgetStack[lastIndex]) widgetStack[lastIndex]->AddToViewport();

	// Otherwise, just remove it from the array as we don't want the array to hold an invalid widget
	else widgetStack.RemoveAt(lastIndex);
}

void AWidgetManager::PopWidget(TSubclassOf<UUserWidget> widgetClass_)
{
	for (int i = 0; i < widgetStack.Num(); ++i)
	{
		// If the widget class doesn't exist, keep iterating until we get the right widget class
		if (widgetStack[i]->GetClass() != widgetClass_) continue;

		// If the widget class exists in the array, remove the widget from memory and the array
		widgetStack[i]->RemoveFromParent();
		widgetStack[i] = nullptr;

		widgetStack.RemoveAt(i);
		break;
	}
}

void AWidgetManager::ClearWidgetStack()
{
	// This will clear all the widgets allocated into the widget stack
	for (int i = 0; i < widgetStack.Num(); ++i)
	{
		widgetStack[i]->RemoveFromParent();
		widgetStack[i] = nullptr;
	}

	if (!widgetStack.IsEmpty()) widgetStack.Empty();
}

void AWidgetManager::SetWidgetVisibility(TSubclassOf<UUserWidget> widgetClass_, const ESlateVisibility& widgetVisibility_)
{
	for (int i = 0; i < widgetStack.Num(); ++i)
	{
		// If the widget class doesn't exist, keep iterating until we get the right widget class
		if (widgetStack[i]->GetClass() != widgetClass_) continue;

		// If the widget class exists in the array, set the widget visibility
		widgetStack[i]->SetVisibility(widgetVisibility_);
		break;
	}
}

bool AWidgetManager::DoesWidgetExist(TSubclassOf<UUserWidget> widgetClass_)
{
	for (int i = 0; i < widgetStack.Num(); ++i)
	{
		// If the widget class doesn't exist, keep iterating until we get the right widget class
		if (widgetStack[i]->GetClass() != widgetClass_) continue;

		// Check if a specific widget class exists in the array to validate that widget
		return widgetStack.Contains(widgetStack[i]);
	}

	// Return false if the widget doesn't exist
	return false;
}