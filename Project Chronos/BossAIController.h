// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class GMTKJAM2025_API ABossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABossAIController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	static bool playerCloseToAI;
};
