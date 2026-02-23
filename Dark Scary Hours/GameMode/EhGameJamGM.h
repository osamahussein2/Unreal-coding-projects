// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Widget/MainMenuWidget.h"
#include "Widget/PauseGameWidget.h"
#include "EhGameJamGM.generated.h"

class UTouchInterface;

/**
 * 
 */
UCLASS()
class EHGAMEJAM_API AEhGameJamGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AEhGameJamGM();

	// Mouse cursor functions
	void EnableMouseCursor();
	void DisableMouseCursor();

	void PushWidget(TSubclassOf<UUserWidget> widgetClass_);
	void PopWidget(TSubclassOf<UUserWidget> widgetClass_);

	// Widget subclasses getter functions (preventing anyone from modifying the widget subclasses)
	TSubclassOf<UMainMenuWidget> GetMainMenuClass() const;
	TSubclassOf<UPauseGameWidget> GetPauseMenuClass() const;

	// Loading/Unloading playing level
	void LoadPlayingLevel();
	void UnloadPlayingLevel();

	void ShowPlayerHUD();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnGameLevelLoaded();

private:
	// Player subclasses
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<APawn> playerMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<APawn> playerGameClass;

	UPROPERTY()
	APawn* menuPlayer = nullptr;

	UPROPERTY()
	APawn* gamePlayer = nullptr;

	// Widget array holding UUserWidget pointers
	UPROPERTY(VisibleAnywhere, Category = "Widgets")
	TArray<UUserWidget*> cachedWidgets;

	// Widget subclasses
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> mainMenuClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPauseGameWidget> pauseMenuClass;
};
