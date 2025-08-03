// Fill out your copyright notice in the Description page of Project Settings.

#include "GMTKGameMode.h"
#include "PlayerCharacter.h"

AGMTKGameMode::AGMTKGameMode()
{
	if (DefaultPawnClass == ADefaultPawn::StaticClass() || !DefaultPawnClass)
	{
		// Set the default pawn class to the player character class
		DefaultPawnClass = APlayerCharacter::StaticClass();
	}
}
