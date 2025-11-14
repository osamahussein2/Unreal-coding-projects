// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "PlayerInputDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SHROOMJAM2025_API UPlayerInputDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* IA_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* IA_Pause;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* IA_Crouch;

	// Mobile touch
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* IA_Touch;
};
