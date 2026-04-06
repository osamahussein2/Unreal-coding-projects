// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BD_StartMovingToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API UBTT_BD_StartMovingToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_BD_StartMovingToPlayer(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
