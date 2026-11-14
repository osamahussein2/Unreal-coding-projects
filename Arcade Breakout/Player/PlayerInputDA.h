// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerInputDA.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS(NotBlueprintType, NotPlaceable)
class SHROOMJAM2026_API UPlayerInputDA : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Just initialize all member data to default, no implentation in .cpp needed
	UPlayerInputDA() = default;

	// Get the input mapping context (this is public so the player character can access all available input mappings)
	TObjectPtr<UInputMappingContext> GetInputMappingContext() const { return inputMappingContext; }

	// Get all the other input actions
	TObjectPtr<UInputAction> GetMoveAction() const { return moveAction; }
	TObjectPtr<UInputAction> GetLookAction() const { return lookAction; }
	TObjectPtr<UInputAction> GetPauseAction() const { return pauseAction; }
	TObjectPtr<UInputAction> GetMagnetAction() const { return magnetAction; }

	// All these input mapping and action variables should be private so no other class can modify them (unless you want a friend class here)
private:
	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	TObjectPtr<UInputMappingContext> inputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	TObjectPtr<UInputAction> moveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	TObjectPtr<UInputAction> lookAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	TObjectPtr<UInputAction> pauseAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Player Input")
	TObjectPtr<UInputAction> magnetAction = nullptr;
};
