// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager/WidgetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Widget/PauseMenuWidget.h"

AWidgetManager::AWidgetManager()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;
}

void AWidgetManager::BeginPlay()
{
	Super::BeginPlay();
}

void AWidgetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWidgetManager::EnableMouseCursor()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false); // Don't hide the cursor during capture
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	playerController->SetInputMode(InputMode);

	if (playerController->bShowMouseCursor != true) playerController->bShowMouseCursor = true;
}

void AWidgetManager::DisableMouseCursor()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	playerController->SetInputMode(GameInputMode);

	// Make sure the mouse cursor is turned off
	if (playerController->bShowMouseCursor != false) playerController->bShowMouseCursor = false;
}

void AWidgetManager::PushWidget(TSubclassOf<UUserWidget> widgetClass_)
{
	// If that widget class is valid and it doesn't exist already, then try to push that widget into the widget stack
	if (WidgetDoesNotExist(widgetClass_))
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
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
}

void AWidgetManager::PopWidget(TSubclassOf<UUserWidget> widgetClass_)
{
	for (int i = 0; i < widgetStack.Num(); ++i)
	{
		// If the widget class exists in the array, remove the widget from memory and the array
		if (DoesWidgetExist(i, widgetClass_))
		{
			widgetStack[i]->RemoveFromParent();
			widgetStack[i] = nullptr;

			widgetStack.RemoveAt(i);
			break;
		}

		else continue;
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

	widgetStack.Empty();
}

void AWidgetManager::SetWidgetVisibility(TSubclassOf<UUserWidget> widgetClass_, const ESlateVisibility& widgetVisibility_)
{
	for (int i = 0; i < widgetStack.Num(); ++i)
	{
		// If the widget class exists in the array, set the widget visibility
		if (DoesWidgetExist(i, widgetClass_))
		{
			widgetStack[i]->SetVisibility(widgetVisibility_);
			break;
		}

		else continue;
	}
}

void AWidgetManager::ResumeGameplay()
{
	for (int i = 0; i < widgetStack.Num(); ++i)
	{
		if (UPauseMenuWidget* pauseMenu = Cast<UPauseMenuWidget>(widgetStack[i]))
		{
			pauseMenu->PlayResumeAnimation();
			break;
		}

		else continue;
	}
}

bool AWidgetManager::WidgetDoesNotExist(TSubclassOf<UUserWidget> widgetClass_)
{
	if (!widgetClass_) return false;

	TObjectPtr<UUserWidget> userWidget = nullptr;

	for (int i = 0; i < widgetStack.Num(); ++i)
	{
		// Check if a specific widget class exists in the array to validate that widget
		if (widgetStack[i]->GetClass() == widgetClass_)
		{
			userWidget = widgetStack[i];
			break;
		}

		else continue;
	}

	// Return true if widget class is valid and if the widget stack doesn't contain that widget
	return widgetClass_ && !widgetStack.Contains(userWidget);
}

bool AWidgetManager::DoesWidgetExist(int& index_, TSubclassOf<UUserWidget> widgetClass_)
{
	UUserWidget* userWidget = Cast<UUserWidget>(widgetStack[index_]);
	if (!widgetClass_ || !userWidget) return false;

	return widgetClass_ && widgetStack[index_]->GetClass() == widgetClass_ && widgetStack.Contains(userWidget);
}