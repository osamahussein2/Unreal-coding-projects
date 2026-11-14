// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WidgetManager.generated.h"

class UUserWidget;
class UMainMenuWidget;
class UPauseMenuWidget;
class UPlayerHUDWidget;
class UGameOverWidget;
class UDialogueWidget;
class ULevelCompleteWidget;
class UCreditsWidget;

/**
 * 
 */
UCLASS()
class SHROOMJAM2026_API AWidgetManager : public AHUD
{
	GENERATED_BODY()
	
public:
	AWidgetManager();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

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

	// Widget class getter functions
	TSubclassOf<UMainMenuWidget> GetMainMenuClass() const { return mainMenuClass; }
	TSubclassOf<UPauseMenuWidget> GetPauseMenuClass() const { return pauseMenuClass; }
	TSubclassOf<UPlayerHUDWidget> GetPlayerHUDClass() const { return playerHUDClass; }
	TSubclassOf<UGameOverWidget> GetGameOverClass() const { return gameOverClass; }
	TSubclassOf<UDialogueWidget> GetPrizeMachineDialogueClass() const { return prizeMachineDialogueClass; }
	TSubclassOf<ULevelCompleteWidget> GetLevelCompleteClass() const { return levelCompleteClass; }
	TSubclassOf<UCreditsWidget> GetCreditsMenuClass() const { return creditsMenuClass; }

private:
	bool DoesWidgetExist(TSubclassOf<UUserWidget> widgetClass_);

private:
	// Widget stack
	UPROPERTY(VisibleAnywhere, Category = "Widgets")
	TArray<TObjectPtr<UUserWidget>> widgetStack = TArray<TObjectPtr<UUserWidget>>();

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> mainMenuClass = TSubclassOf<UMainMenuWidget>();

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPauseMenuWidget> pauseMenuClass = TSubclassOf<UPauseMenuWidget>();

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPlayerHUDWidget> playerHUDClass = TSubclassOf<UPlayerHUDWidget>();

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UGameOverWidget> gameOverClass = TSubclassOf<UGameOverWidget>();

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UDialogueWidget> prizeMachineDialogueClass = TSubclassOf<UDialogueWidget>();

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<ULevelCompleteWidget> levelCompleteClass = TSubclassOf<ULevelCompleteWidget>();

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UCreditsWidget> creditsMenuClass = TSubclassOf<UCreditsWidget>();
};
