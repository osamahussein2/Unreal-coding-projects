// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerConnector.h"
#include "Generator.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDEVTVGAMEJAM2026_API AGenerator : public APowerConnector
{
	GENERATED_BODY()
	
public:
	AGenerator();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
