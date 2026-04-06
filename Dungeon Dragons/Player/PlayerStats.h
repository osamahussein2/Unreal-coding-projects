// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStats.generated.h"

USTRUCT()
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int dexterity = 1;

	UPROPERTY(VisibleAnywhere)
	int strength = 1;

	UPROPERTY(VisibleAnywhere)
	int intelligence = 1;
};
