// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputActionValue.h"
#include "InputConfigDataAsset.generated.h"

class UInputAction;

/**
 * Data asset that stores references to all input actions used by the character.
 * Used to centralize input configuration and make it easier to manage from Blueprints or the editor.
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UInputConfigDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* SpinCameraAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* EatAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* ThrowAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* ZoomAction;

};
