// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/TaskNodes/BTT_BD_StartMovingToPlayer.h"
#include "Enemy/BloodDrinker.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

UBTT_BD_StartMovingToPlayer::UBTT_BD_StartMovingToPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Blood Drinker Start Moving to Player";
}

EBTNodeResult::Type UBTT_BD_StartMovingToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABloodDrinker* bloodDrinker = Cast<ABloodDrinker>(OwnerComp.GetAIOwner()->GetPawn());

	// If the enemy or the world isn't valid, return failed execution task
	if (!bloodDrinker || !GetWorld()) return EBTNodeResult::Type::Failed;

	// Start moving towards player
	bloodDrinker->StartMovingToPlayer();

	return EBTNodeResult::Type::Succeeded;
}

EBTNodeResult::Type UBTT_BD_StartMovingToPlayer::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABloodDrinker* bloodDrinker = Cast<ABloodDrinker>(OwnerComp.GetAIOwner()->GetPawn());

	// If the enemy isn't valid, return failed execution task
	if (!bloodDrinker) return EBTNodeResult::Type::Failed;

#ifdef UE_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Stopped moving"));
#endif

	return EBTNodeResult::Type();
}