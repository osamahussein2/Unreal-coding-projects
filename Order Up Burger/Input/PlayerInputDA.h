// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerInputDA.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UPlayerInputDA : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	UInputAction* GetIA_Move() const { return IA_Move; }

	UFUNCTION()
	UInputAction* GetIA_CameraSpin() const { return IA_CameraSpin; }

	UFUNCTION()
	UInputAction* GetIA_CameraZoom() const { return IA_CameraZoom; }

	UFUNCTION()
	UInputAction* GetIA_ToggleCook() const { return IA_ToggleCook; }

	UFUNCTION()
	UInputAction* GetIA_DropFood() const { return IA_DropFood; }

	UFUNCTION()
	UInputAction* GetIA_PauseGame() const { return IA_Pause; }

private:
	UPROPERTY(EditAnywhere)
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_CameraSpin;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_CameraZoom;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_ToggleCook;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_DropFood;

	UPROPERTY(EditAnywhere)
	UInputAction* IA_Pause;
};
