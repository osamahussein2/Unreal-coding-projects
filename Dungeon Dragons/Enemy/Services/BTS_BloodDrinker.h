// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_BloodDrinker.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API UBTS_BloodDrinker : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_BloodDrinker();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CanEnemySeePlayer(AActor* player, AActor* enemy) const;
	bool IsEnemyBehindThePlayer(AActor* player, AActor* enemy) const;
	bool IsEnemyToRightOfPlayer(AActor* player, AActor* enemy) const;
	bool IsEnemyToLeftOfPlayer(AActor* player, AActor* enemy) const;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowAccessPrivate = true))
	FBlackboardKeySelector BloodDrinkerKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowAccessPrivate = true))
	FBlackboardKeySelector PlayerKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowAccessPrivate = true))
	FBlackboardKeySelector DistanceToPlayerKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowAccessPrivate = true))
	FBlackboardKeySelector PlayerWithinRange;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowAccessPrivate = true))
	FBlackboardKeySelector MeleeRange;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowAccessPrivate = true))
	FBlackboardKeySelector EnemySawPlayer;

	UPROPERTY(EditAnywhere, Category = "Distance to Player")
	float minDistanceToPlayer = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Distance to Player")
	float distanceToMeleePlayer = 550.0f;
};
