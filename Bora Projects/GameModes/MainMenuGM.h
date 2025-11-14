// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Characters/Player/MainMenuCharacter.h"
#include "MainMenuGM.generated.h"

/**
 * 
 */
UCLASS()
class SHROOMJAM2025_API AMainMenuGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMainMenuGM();

protected:
		virtual void BeginPlay() override;

private:
	//TSubclassOf<AMainMenuCharacter> playerPawnClass;
};
