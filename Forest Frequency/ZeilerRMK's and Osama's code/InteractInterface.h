// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class ADefaultPlayer;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEDEVTVGAMEJAM2026_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	bool CanInteract(ADefaultPlayer* Interactor) const;

	virtual bool CanInteract_Implementation(ADefaultPlayer* Interactor) const;

	UFUNCTION(BlueprintNativeEvent)
	void Interact(ADefaultPlayer* Interactor);

	virtual void Interact_Implementation(ADefaultPlayer* Interactor);
};
