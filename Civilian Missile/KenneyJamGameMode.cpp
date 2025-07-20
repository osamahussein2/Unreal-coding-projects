// Fill out your copyright notice in the Description page of Project Settings.


#include "KenneyJamGameMode.h"
#include "PlayerCharacter.h"

AKenneyJamGameMode::AKenneyJamGameMode()
{
	if (DefaultPawnClass == ADefaultPawn::StaticClass() || !DefaultPawnClass)
	{
		// Set the default pawn class to the player character class
		DefaultPawnClass = APlayerCharacter::StaticClass();
	}
}