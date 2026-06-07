// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ConnectionInterface.generated.h"

class APowerCable;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UConnectionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEDEVTVGAMEJAM2026_API IConnectionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	bool CanConnectCable(APowerCable* Cable) const; 

	virtual bool CanConnectCable_Implementation(APowerCable* Cable) const;

	UFUNCTION(BlueprintNativeEvent)
	void ConnectCable(APowerCable* Cable);

	virtual void ConnectCable_Implementation(APowerCable* Cable);

	UFUNCTION(BlueprintNativeEvent)
	void DisconnectCable(APowerCable* Cable);

	virtual void DisconnectCable_Implementation(APowerCable* Cable);
};
