// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAIController.h"
#include "BossCharacter.h"
#include "PlayerCharacter.h"

bool ABossAIController::playerCloseToAI = false;

ABossAIController::ABossAIController(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ABossAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (playerCloseToAI && !ABossCharacter::GetIsDead() && !APlayerCharacter::IsGamePaused())
	{
		// Move towards player
		MoveToActor(GetWorld()->GetFirstPlayerController(), 50.0f);
	}

	else if (!playerCloseToAI && !ABossCharacter::GetIsDead() && !APlayerCharacter::IsGamePaused())
	{
		// For now, just stop moving the AI
		StopMovement();
	}

	if (ABossCharacter::GetIsDead() || APlayerCharacter::IsGamePaused())
	{
		StopMovement();
	}
}