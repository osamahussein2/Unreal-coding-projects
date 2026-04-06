// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/TaskNodes/BTT_BD_MeleeAttack.h"
#include "Enemy/BloodDrinker.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

UBTT_BD_MeleeAttack::UBTT_BD_MeleeAttack(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Blood Drinker Melee Attack";
}

EBTNodeResult::Type UBTT_BD_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABloodDrinker* bloodDrinker = Cast<ABloodDrinker>(OwnerComp.GetAIOwner()->GetPawn());

	// If the enemy or the world isn't valid, return failed execution task
	if (!bloodDrinker || !GetWorld()) return EBTNodeResult::Type::Failed;

	// Start attacking the player using melee
	bloodDrinker->StartAttackingPlayer();

	return EBTNodeResult::Type::Succeeded;
}

EBTNodeResult::Type UBTT_BD_MeleeAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABloodDrinker* bloodDrinker = Cast<ABloodDrinker>(OwnerComp.GetAIOwner()->GetPawn());

	// If the enemy isn't valid, return failed execution task
	if (!bloodDrinker) return EBTNodeResult::Type::Failed;

#ifdef UE_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Stopped attacking"));
#endif

	return EBTNodeResult::Type();
}