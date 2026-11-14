// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShroomJamGM.generated.h"

class ACharacter;

/**
 * 
 */
UCLASS()
class SHROOMJAM2026_API AShroomJamGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AShroomJamGM();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	// Loading/Unloading playing level
	void LoadPlayingLevel();
	void UnloadPlayingLevel();

	void SetPlayerCharacterToNull();

private:
	UFUNCTION()
	void OnGameLevelLoaded();

private:
	// Levels
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level", meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<UWorld> gameLevel = nullptr;

	// Player character
	UPROPERTY(EditDefaultsOnly, Category = "Player Class")
	TSubclassOf<ACharacter> playerCharacterClass = nullptr;

	UPROPERTY()
	TObjectPtr<ACharacter> playerCharacter = nullptr;

	// Game music
	UPROPERTY(EditDefaultsOnly, Category = "Music")
	TObjectPtr<USoundBase> gameMusic = nullptr;
};
