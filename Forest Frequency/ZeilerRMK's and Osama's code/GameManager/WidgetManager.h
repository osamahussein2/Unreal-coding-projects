// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WidgetManager.generated.h"

class UUserWidget;
class UMainMenuWidget;
class UControlsWidget;
class UCreditsWidget;
class UPauseMenuWidget;

/**
 * 
 */
UCLASS()
class GAMEDEVTVGAMEJAM2026_API AWidgetManager : public AHUD
{
	GENERATED_BODY()
	
public:
	AWidgetManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// Mouse cursor functions
	void EnableMouseCursor();
	void DisableMouseCursor();

	// Widget stack functions
	void PushWidget(TSubclassOf<UUserWidget> widgetClass_);
	void PopWidget(TSubclassOf<UUserWidget> widgetClass_);

	void ClearWidgetStack();

	// Control widget visibility if needed
	void SetWidgetVisibility(TSubclassOf<UUserWidget> widgetClass_, const ESlateVisibility& widgetVisibility_);

	void ResumeGameplay();

	// Widget class getter functions
	TSubclassOf<UMainMenuWidget> GetMainMenuClass() const { return mainMenuClass; }
	TSubclassOf<UControlsWidget> GetControlsClass() const { return controlsClass; }
	TSubclassOf<UCreditsWidget> GetCreditsClass() const { return creditsClass; }
	TSubclassOf<UPauseMenuWidget> GetPauseMenuClass() const { return pauseMenuClass; }

private:

	// Widget exists in the widget stack getter functions
	bool WidgetDoesNotExist(TSubclassOf<UUserWidget> widgetClass_);
	bool DoesWidgetExist(int& index_, TSubclassOf<UUserWidget> widgetClass_);

private:
	UPROPERTY(VisibleAnywhere, Category = "Widgets")
	TArray<TObjectPtr<UUserWidget>> widgetStack;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> mainMenuClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UControlsWidget> controlsClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UCreditsWidget> creditsClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPauseMenuWidget> pauseMenuClass;
};
