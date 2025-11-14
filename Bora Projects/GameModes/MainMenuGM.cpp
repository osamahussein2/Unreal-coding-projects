// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/MainMenuGM.h"
#include "EngineUtils.h"
#include "Engine/World.h"

AMainMenuGM::AMainMenuGM()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
}

void AMainMenuGM::BeginPlay()
{
	AMainMenuCharacter* MainMenuCharacter = nullptr;
	for (TActorIterator<AMainMenuCharacter> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() != nullptr && It->GetLevel()->bIsVisible)
		{
			MainMenuCharacter = *It;
			break;
		}
	}

	// If the main menu character is found in the level, make the player possess it
	if (MainMenuCharacter)
	{
		GetWorld()->GetFirstPlayerController()->Possess(MainMenuCharacter);
		MainMenuCharacter->EnableInput(GetWorld()->GetFirstPlayerController());
	}
}