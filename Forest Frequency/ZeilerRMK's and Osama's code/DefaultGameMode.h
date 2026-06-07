// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DefaultGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEVTVGAMEJAM2026_API ADefaultGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ADefaultGameMode();

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "SFX")
	TObjectPtr<USoundBase> ambienceSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "SFX")
	TObjectPtr<USoundBase> gameMusic = nullptr;
};
