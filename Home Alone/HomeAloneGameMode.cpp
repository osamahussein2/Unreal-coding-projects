// Fill out your copyright notice in the Description page of Project Settings.

#include "HomeAloneGameMode.h"

AHomeAloneGameMode::AHomeAloneGameMode()
{
	if (DefaultPawnClass == ADefaultPawn::StaticClass() || !DefaultPawnClass)
	{
		// Set the default pawn class to the player character class
		DefaultPawnClass = APlayerCharacter::StaticClass();
	}
}
