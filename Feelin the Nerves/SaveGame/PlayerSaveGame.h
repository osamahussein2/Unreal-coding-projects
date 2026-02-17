// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Get saved player current/max sequence values
	UFUNCTION()
	int GetPlayerMaxPlayedSequences();

	UFUNCTION()
	int GetPlayerMaxCorrectSequences();

	UFUNCTION()
	int GetPlayerCurrentCompletedSequences();

	// Set saved player current/max sequence values
	UFUNCTION()
	void SetPlayerMaxPlayedSequences(int value_);

	UFUNCTION()
	void SetPlayerMaxCorrectSequences(int value_);

	UFUNCTION()
	void SetPlayerCurrentCompletedSequences(int value_);

	// Getter for random nerves array
	UFUNCTION()
	TArray<int> GetSavedRandomNerves();

	UFUNCTION()
	int GetSavedRandomNerveAtReferenceIndex(int& i);

	// Setter for random nerves array
	UFUNCTION()
	void SetSavedRandomNerves(int index_);

	UFUNCTION()
	void EmptySavedRandomNerves();

private:
	UPROPERTY()
	int savedMaxPlayedSequences = 0;

	UPROPERTY()
	int savedMaxCorrectSequences = 0;

	UPROPERTY()
	int savedCurrentCompletedSequences = 0;

	UPROPERTY()
	TArray<int> savedRandomNerves;
};
