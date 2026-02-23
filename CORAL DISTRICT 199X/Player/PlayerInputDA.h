// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerInputDA.generated.h"

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API UPlayerInputDA : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	UInputAction* GetIA_Move() const { return IA_Move; }

	UFUNCTION()
	UInputAction* GetIA_Look() const { return IA_Look; }

	UFUNCTION()
	UInputAction* GetIA_InteractWithFish() const { return IA_InteractWithFish; }

	UFUNCTION()
	UInputAction* GetIA_Pause() const { return IA_Pause; }

private:
	UPROPERTY(EditAnywhere, Category = "Input Actions")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input Actions")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, Category = "Input Actions")
	UInputAction* IA_InteractWithFish;

	UPROPERTY(EditAnywhere, Category = "Input Actions")
	UInputAction* IA_Pause;
};
