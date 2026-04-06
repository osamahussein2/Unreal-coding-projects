// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/Services/BTS_BloodDrinker.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/BloodDrinker.h"
#include "Kismet/KismetMathLibrary.h"
#include "DungeonCrawler2026/DungeonCrawler2026.h"

UBTS_BloodDrinker::UBTS_BloodDrinker()
{
	NodeName = "Blood Drinker Service";
}

void UBTS_BloodDrinker::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* BloodDrinkerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BloodDrinkerKey.SelectedKeyName));
	AActor* PlayerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(PlayerKey.SelectedKeyName));

	if (BloodDrinkerActor && PlayerActor)
	{
		float Distance = FVector::Dist(BloodDrinkerActor->GetActorLocation(), PlayerActor->GetActorLocation());
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceToPlayerKey.SelectedKeyName, Distance);

		const bool& playerWithinRange = Distance <= minDistanceToPlayer;
		const bool& playerWithinMeleeRange = Distance <= distanceToMeleePlayer;

		const bool& enemySawPlayer = CanEnemySeePlayer(PlayerActor, BloodDrinkerActor) ||
			IsEnemyBehindThePlayer(PlayerActor, BloodDrinkerActor) ||
			IsEnemyToRightOfPlayer(PlayerActor, BloodDrinkerActor) ||
			IsEnemyToLeftOfPlayer(PlayerActor, BloodDrinkerActor);

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(PlayerWithinRange.SelectedKeyName, playerWithinRange);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(EnemySawPlayer.SelectedKeyName, enemySawPlayer);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(MeleeRange.SelectedKeyName, playerWithinMeleeRange);

		if (Cast<ABloodDrinker>(BloodDrinkerActor))
		{
			// If the blood drinker enemy is no longer within the range of the player nor they can see the player
			// Stop running all behavior tasks
			if (!playerWithinRange && !enemySawPlayer) Cast<ABloodDrinker>(BloodDrinkerActor)->StopAllBehaviorTasks();

			else if (!playerWithinMeleeRange)
			{
				Cast<ABloodDrinker>(BloodDrinkerActor)->StopAttackingPlayer();
			}

			else if (playerWithinMeleeRange)
			{
				Cast<ABloodDrinker>(BloodDrinkerActor)->StopMovingToPlayer();
			}
		}
	}
}

bool UBTS_BloodDrinker::CanEnemySeePlayer(AActor* player, AActor* enemy) const
{
	FHitResult hit;
	FCollisionQueryParams collisionQueryParams;

	const FVector& traceStart = enemy->GetActorLocation();
	const FVector& traceEnd = enemy->GetActorLocation() + enemy->GetActorForwardVector() * 100000.0f;

	collisionQueryParams.AddIgnoredActor(enemy);

	if (!player || !enemy || !GetWorld())
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Invalid object"));
#endif
		return false;
	}

	return GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECC_Visibility, collisionQueryParams) &&
		hit.GetActor() == player;
}

bool UBTS_BloodDrinker::IsEnemyBehindThePlayer(AActor* player, AActor* enemy) const
{
	FHitResult hit;
	FCollisionQueryParams collisionQueryParams;

	const FVector& traceStart = enemy->GetActorLocation();
	const FVector& traceEnd = enemy->GetActorLocation() - enemy->GetActorForwardVector() * 100000.0f;

	collisionQueryParams.AddIgnoredActor(enemy);

	if (!player || !enemy || !GetWorld())
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Invalid object"));
#endif
		return false;
	}

	if (GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECC_Visibility, collisionQueryParams) &&
		hit.GetActor() == player)
	{
		const FRotator& lookAtPlayer = UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(),
			player->GetActorLocation());

		enemy->SetActorRotation(lookAtPlayer);
	}

	return GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECC_Visibility, collisionQueryParams) &&
		hit.GetActor() == player;
}

bool UBTS_BloodDrinker::IsEnemyToRightOfPlayer(AActor* player, AActor* enemy) const
{
	FHitResult hit;
	FCollisionQueryParams collisionQueryParams;

	const FVector& traceStart = enemy->GetActorLocation();
	const FVector& traceEnd = enemy->GetActorLocation() + enemy->GetActorRightVector() * 100000.0f;

	collisionQueryParams.AddIgnoredActor(enemy);

	if (!player || !enemy || !GetWorld())
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Invalid object"));
#endif
		return false;
	}

	if (GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECC_Visibility, collisionQueryParams) &&
		hit.GetActor() == player)
	{
		const FRotator& lookAtPlayer = UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(),
			player->GetActorLocation());

		enemy->SetActorRotation(lookAtPlayer);
	}

	return GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECC_Visibility, collisionQueryParams) &&
		hit.GetActor() == player;
}

bool UBTS_BloodDrinker::IsEnemyToLeftOfPlayer(AActor* player, AActor* enemy) const
{
	FHitResult hit;
	FCollisionQueryParams collisionQueryParams;

	const FVector& traceStart = enemy->GetActorLocation();
	const FVector& traceEnd = enemy->GetActorLocation() - enemy->GetActorRightVector() * 100000.0f;

	collisionQueryParams.AddIgnoredActor(enemy);

	if (!player || !enemy || !GetWorld())
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Invalid object"));
#endif
		return false;
	}

	if (GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECC_Visibility, collisionQueryParams) &&
		hit.GetActor() == player)
	{
		const FRotator& lookAtPlayer = UKismetMathLibrary::FindLookAtRotation(enemy->GetActorLocation(),
			player->GetActorLocation());

		enemy->SetActorRotation(lookAtPlayer);
	}

	return GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECC_Visibility, collisionQueryParams) &&
		hit.GetActor() == player;
}