// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerInputDA.generated.h"

/**
 * 
 */
UCLASS()
class GOEDWARECUTSCENE2JAM_API UPlayerInputDA : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION()
	UInputAction* GetCutsceneInteractionIA() const { return IA_CutsceneInteraction; }
	
private:
	UPROPERTY(EditAnywhere)
	UInputAction* IA_CutsceneInteraction;
};
