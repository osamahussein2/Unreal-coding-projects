// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/BloodDrinker.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/Floor.h"
#include "EngineUtils.h"
#include "Player/PlayerCharacter.h"
#include "Components/HealthComponent.h"
#include "Engine/DamageEvents.h"
#include "PaperSpriteComponent.h"

// Sets default values
ABloodDrinker::ABloodDrinker()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	GetMesh()->bHiddenInSceneCapture = true;
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("EnemySprite"));
	spriteComponent->SetupAttachment(GetMesh());

	spriteComponent->bVisibleInSceneCaptureOnly = true;

	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	Tags.Add("Enemy");
}

// Called when the game starts or when spawned
void ABloodDrinker::BeginPlay()
{
	Super::BeginPlay();
	
	if (behaviorTree)
	{
		Cast<AAIController>(GetController())->RunBehaviorTree(behaviorTree);
		Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsObject(bloodDrinkerKey, this);

#ifdef UE_EDITOR
		if (!Cast<AAIController>(GetController())->GetBlackboardComponent()->GetValueAsObject(bloodDrinkerKey))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Blood Drinker Key not found"));
		}
#endif

		GetWorld()->GetTimerManager().SetTimer(findPlayerHandle, this, &ABloodDrinker::FindPlayer, 0.1f, true);
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(moveToPlayerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(moveToPlayerHandle);
	}

	if (healthComponent)
	{
		healthComponent->OnHealthChanged.AddUniqueDynamic(this, &ABloodDrinker::UpdateHealth);
		healthComponent->OnDeath.AddUniqueDynamic(this, &ABloodDrinker::DestroyBloodDrinker);
	}

	if (GetMesh()) GetMesh()->SetAnimation(nullptr);
}

void ABloodDrinker::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld()->GetTimerManager().IsTimerActive(findPlayerHandle))
		GetWorld()->GetTimerManager().ClearTimer(findPlayerHandle);

	if (GetWorld()->GetTimerManager().IsTimerActive(moveToPlayerHandle))
		GetWorld()->GetTimerManager().ClearTimer(moveToPlayerHandle);

	if (GetWorld()->GetTimerManager().IsTimerActive(attackPlayerHandle))
		GetWorld()->GetTimerManager().ClearTimer(attackPlayerHandle);
}

// Called every frame
void ABloodDrinker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABloodDrinker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABloodDrinker::StartMovingToPlayer()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(moveToPlayerHandle))
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Blood Drinker starting to move..."));
#endif

		GetWorld()->GetTimerManager().SetTimer(moveToPlayerHandle, this, &ABloodDrinker::MoveToPlayer, timeToMove, true);
	}
}

void ABloodDrinker::StartAttackingPlayer()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(attackPlayerHandle))
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Blood Drinker starting to attack..."));
#endif

		GetWorld()->GetTimerManager().SetTimer(attackPlayerHandle, this, &ABloodDrinker::AttackPlayer, timeToAttack, true);
	}
}

void ABloodDrinker::StopAllBehaviorTasks()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(moveToPlayerHandle))
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Stopped all actions"));
#endif

		GetWorld()->GetTimerManager().ClearTimer(moveToPlayerHandle);

		if (GetWorld()->GetTimerManager().IsTimerActive(attackPlayerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(attackPlayerHandle);
		}
	}
}

void ABloodDrinker::StopMovingToPlayer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(moveToPlayerHandle))
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Stopped moving to player"));
#endif

		GetWorld()->GetTimerManager().ClearTimer(moveToPlayerHandle);
	}
}

void ABloodDrinker::StopAttackingPlayer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(attackPlayerHandle))
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Stopped trying to attack the player"));
#endif

		GetWorld()->GetTimerManager().ClearTimer(attackPlayerHandle);
	}
}

float ABloodDrinker::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (healthComponent)
	{
		healthComponent->TakeDamage(this, DamageAmount, DamageCauser);
		if (hitSounds[1]) UGameplayStatics::PlaySound2D(GetWorld(), hitSounds[1]);
	}

	return DamageAmount;
}

void ABloodDrinker::FindPlayer()
{
	if (behaviorTree)
	{
		if (!Cast<AAIController>(GetController())->GetBlackboardComponent()->GetValueAsObject(playerKeyName))
		{
			Cast<AAIController>(GetController())->GetBlackboardComponent()->SetValueAsObject(playerKeyName,
				UGameplayStatics::GetPlayerCharacter(this, 0));
		}

		else if (!Cast<AAIController>(GetController())->GetBlackboardComponent()->GetValueAsObject(playerKeyName))
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(findPlayerHandle))
			{
#ifdef UE_EDITOR
				GEngine->AddOnScreenDebugMessage(400, 2.0f, FColor::Green, TEXT("Blood Drinker found player info"));
#endif

				GetWorld()->GetTimerManager().ClearTimer(findPlayerHandle);
			}
		}
	}
}

void ABloodDrinker::MoveToPlayer()
{
	ABloodDrinker* nearestBloodDrinker = nullptr;

	for (TActorIterator<AFloor> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() && It->GetLevel()->bIsVisible)
		{
			const FVector& directionToFloorAhead = (It->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			const float& dotProduct = FVector::DotProduct(GetActorForwardVector(), directionToFloorAhead);

			for (TActorIterator<ABloodDrinker> bloodDrinkerIt(GetWorld()); bloodDrinkerIt; ++bloodDrinkerIt)
			{
				const FVector& directionToOtherEnemy = (bloodDrinkerIt->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				const float& isLookingAtOtherEnemy = FVector::DotProduct(GetActorForwardVector(), directionToOtherEnemy);

				// Looking at other enemy
				if (isLookingAtOtherEnemy > dotProductValue && *bloodDrinkerIt != this &&
					IsAnotherEnemyNearby(*bloodDrinkerIt) && IsAnotherEnemyNearby(*It, *bloodDrinkerIt))
				{
					nearestBloodDrinker = *bloodDrinkerIt;
					break;
				}

				else continue;
			}

			if (dotProduct > dotProductValue && WithinTheRangeOfNextFloor(*It) && !IsPlayerNearby(*It) &&
				!nearestBloodDrinker)
			{
				const FVector& nextLocation = FVector(It->GetActorLocation().X, It->GetActorLocation().Y,
					GetActorLocation().Z);

				SetActorLocation(nextLocation);

#ifdef UE_EDITOR
				GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, TEXT("Moving towards player..."));
#endif
				break;
			}

			else if (nearestBloodDrinker) break;
		}

		else continue;
	}
}

void ABloodDrinker::AttackPlayer()
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		if (player->GetHealthComponent())
		{
			// Play the blood drinker attack animation
			if (enemyAttackAnimation && GetMesh())
			{
				GetMesh()->SetAnimation(enemyAttackAnimation);
				GetMesh()->Play(false);

				FTimerHandle resetAnimationHandle{}; // Handle to reset animation to null
				GetWorld()->GetTimerManager().SetTimer(resetAnimationHandle, this, &ABloodDrinker::ResetAnimation,
					enemyAttackAnimation->GetPlayLength() * enemyAttackAnimation->RateScale, false);
			}

			player->GetHealthComponent()->TakeDamage(player, damageValue, this);
			if (hitSounds[0]) UGameplayStatics::PlaySound2D(GetWorld(), hitSounds[0]);
		}
	}
}

bool ABloodDrinker::WithinTheRangeOfNextFloor(AActor* floorActor_)
{
	if (!floorActor_)
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Error: Floor actor is null"));
#endif

		return false;
	}

	const float& distanceToNextFloor = FVector::Dist(GetActorLocation(), floorActor_->GetActorLocation());
	return distanceToNextFloor <= maxDistanceToNextFloor && distanceToNextFloor >= minDistanceToNextFloor;
}

bool ABloodDrinker::IsPlayerNearby()
{
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (!playerCharacter)
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Error: Player character is null"));
#endif

		return false;
	}

	const float& distanceToPlayer = FVector::Dist(GetActorLocation(), playerCharacter->GetActorLocation());
	return distanceToPlayer <= maxDistanceToNextFloor;
}

bool ABloodDrinker::IsPlayerNearby(AActor* floorActor_)
{
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (!playerCharacter)
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Error: Player character is null"));
#endif

		return false;
	}

	const float& distanceToPlayer = FVector::Dist(floorActor_->GetActorLocation(), playerCharacter->GetActorLocation());
	return distanceToPlayer <= minDistanceToNextFloor;
}

bool ABloodDrinker::IsAnotherEnemyNearby(AActor* otherEnemy_)
{
	if (!otherEnemy_)
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Error: Other enemy is null"));
#endif

		return false;
	}

	const float& distanceToOtherEnemy = FVector::Dist(GetActorLocation(), otherEnemy_->GetActorLocation());
	return distanceToOtherEnemy <= maxDistanceToNextFloor;
}

bool ABloodDrinker::IsAnotherEnemyNearby(AActor* floorActor_, AActor* otherEnemy_)
{
	if (!otherEnemy_ || !floorActor_)
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Error: Other enemy is null"));
#endif

		return false;
	}

	const float& distanceToOtherEnemy = FVector::Dist(floorActor_->GetActorLocation(), otherEnemy_->GetActorLocation());
	return distanceToOtherEnemy <= maxDistanceToNextFloor;
}

void ABloodDrinker::UpdateHealth(UHealthComponent* HealthComp, float Health, float HealthDelta, const UDamageType* DamageType)
{
	// TODO: Add sound effect upon damage
}

void ABloodDrinker::DestroyBloodDrinker()
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		switch (skillReward)
		{
		case ESkillReward::EDexterity:
			player->IncrementPlayerDexterity(incrementSkillValue);
			break;

		case ESkillReward::EStrength:
			player->IncrementPlayerStrength(incrementSkillValue);
			break;

		case ESkillReward::EIntelligence:
			player->IncrementPlayerIntelligence(incrementSkillValue);
			break;

		default:
			break;
		}

		//player->IncrementEnemiesDefeatedInRoom();
		Destroy();
	}
}

void ABloodDrinker::ResetAnimation()
{
	if (GetMesh()) GetMesh()->SetAnimation(nullptr);
}