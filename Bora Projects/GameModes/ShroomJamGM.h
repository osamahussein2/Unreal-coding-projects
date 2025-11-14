// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"
#include "ShroomJamGM.generated.h"

/**
 * 
 */
UCLASS()
class SHROOMJAM2025_API AShroomJamGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AShroomJamGM();

protected:
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> PlayerCharacterClass;

	UPROPERTY(EditAnywhere)
	USceneComponent* defaultSceneComponent;
};
