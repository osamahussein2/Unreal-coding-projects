// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerInputDA.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API UPlayerInputDA : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	UInputAction* GetIA_Move() const { return IA_Move; }

	UFUNCTION()
	UInputAction* GetIA_Camera() const { return IA_Camera; }

	UFUNCTION()
	UInputAction* GetIA_PauseGame() const { return IA_PauseGame; }

	UFUNCTION()
	UInputAction* GetIA_OpenMap() const { return IA_OpenMap; }

	UFUNCTION()
	UInputAction* GetIA_RollDice() const { return IA_RollDice; }

	UFUNCTION()
	UInputAction* GetIA_Interact() const { return IA_Interact; }

	UFUNCTION()
	UInputAction* GetIA_Attack() const { return IA_Attack; }

	UFUNCTION()
	UInputAction* GetIA_OpenInventory() const { return IA_OpenInventory; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_PauseGame;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_OpenMap;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_RollDice;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Interact;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_Attack;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	UInputAction* IA_OpenInventory;
};
