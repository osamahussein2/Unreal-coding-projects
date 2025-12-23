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
class SCOREJAM37_API UPlayerInputDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* IA_Pause;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* IA_Shoot;
};
