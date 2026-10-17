// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ComfyAutumnGM.generated.h"

class UUserWidget;
class ACharacter;
class ACineCameraActor;

/**
 * 
 */
UCLASS()
class COMFYAUTUMNJAM_API AComfyAutumnGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AComfyAutumnGM();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Loading/Unloading playing level
	UFUNCTION(BlueprintCallable)
	void LoadPlayingLevel();

	UFUNCTION(BlueprintCallable)
	void UnloadPlayingLevel(bool loadLevelAfterwards_);

	UFUNCTION()
	void OnGameLevelLoaded();

	// Main Menu functions
	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void DestroyMainMenu();

	// Loading/Unloading main menu background level
	UFUNCTION(BlueprintCallable)
	void LoadMainMenuBackgroundLevel();

	UFUNCTION(BlueprintCallable)
	void UnloadMainMenuBackgroundLevel();

	UFUNCTION()
	void OnMainMenuBackgroundLevelLoaded();

private:
	// Levels
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UWorld> gameLevel = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UWorld> mainMenuBackgroundLevel = nullptr;

	// Main Menu
	UPROPERTY(EditDefaultsOnly, Category = "Widget Class")
	TSubclassOf<UUserWidget> mainMenuClass = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> mainMenu = nullptr;

	// Player character
	UPROPERTY(EditDefaultsOnly, Category = "Player Class")
	TSubclassOf<ACharacter> playerCharacterClass = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> playerCharacter = nullptr;

	// Level load time
	UPROPERTY(EditDefaultsOnly, Category = "Level Time")
	float timeToReloadLevel = 0.1f;

	UPROPERTY(VisibleAnywhere, Category = "Level Time")
	float reloadLevelTimer = 0.0f;

	// Main Menu Camera
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Camera")
	FTransform mainMenuBackgroundCameraSpawn = FTransform::Identity;

	UPROPERTY(EditDefaultsOnly, Category = "Main Menu Camera")
	TSubclassOf<ACineCameraActor> mainMenuBackgroundCameraClass = nullptr;

	UPROPERTY()
	TObjectPtr<ACineCameraActor> mainMenuBackgroundCamera = nullptr;
};
