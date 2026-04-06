// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Widgets/MainMenuWidget.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Widgets/PauseMenuWidget.h"
#include "Widgets/MapWidget.h"
#include "Widgets/GameOverWidget.h"
#include "Widgets/InventoryWidget.h"
#include "Widgets/LevelCompletionWidget.h"
#include "DungeonCrawlerGM.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API ADungeonCrawlerGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ADungeonCrawlerGM();

	// Mouse cursor functions
	void EnableMouseCursor();
	void DisableMouseCursor();

	void PushWidget(TSubclassOf<UUserWidget> widgetClass_);
	void PopWidget(TSubclassOf<UUserWidget> widgetClass_);

	bool DoesWidgetContainPlayerHUD();
	void UpdatePlayerHealth(const float& healthPercentage);
	void UpdatePlayerStats(const float& dexterity, const float& strength, const float& intelligence);
	void SpawnUnlockDoorWidgetOnHUD();
	void DestroyUnlockDoorWidgetOnHUD();
	void UpdateDiceRoll();
	void SpawnInventoryWidgetOnHUD();

	// Widget subclasses getter functions (preventing anyone from modifying the widget subclasses)
	TSubclassOf<UMainMenuWidget> GetMainMenuClass() const;
	TSubclassOf<UPlayerHUDWidget> GetPlayerHUDClass() const;
	TSubclassOf<UPauseMenuWidget> GetPauseMenuClass() const;
	TSubclassOf<UMapWidget> GetMapClass() const;
	TSubclassOf<UGameOverWidget> GetGameOverClass() const;
	TSubclassOf<UInventoryWidget> GetInventoryClass() const;
	TSubclassOf<ULevelCompletionWidget> GetLevelCompletionClass() const;

	// Loading/Unloading playing level
	void LoadPlayingLevel();
	void UnloadPlayingLevel();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnGameLevelLoaded();

private:
	// Player subclass
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<APawn> playerCharacterClass;

	// Widget array holding UUserWidget pointers
	UPROPERTY(VisibleAnywhere, Category = "Widgets")
	TArray<UUserWidget*> cachedWidgets;

	// Widget subclasses
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> mainMenuClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPlayerHUDWidget> playerHUDClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPauseMenuWidget> pauseMenuClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMapWidget> mapClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UGameOverWidget> gameOverClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UInventoryWidget> inventoryClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<ULevelCompletionWidget> levelCompletionClass;

	// Level name
	UPROPERTY(EditAnywhere, Category = "Level")
	FName levelName = FName();

	UPROPERTY(EditAnywhere, Category = "Music")
	TObjectPtr<USoundBase> gameMusic = nullptr;
};
