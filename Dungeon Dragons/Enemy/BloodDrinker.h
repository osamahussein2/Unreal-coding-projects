// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BloodDrinker.generated.h"

class UBehaviorTree;
class UHealthComponent;
class UPaperSpriteComponent;

UENUM()
enum class ESkillReward : uint8
{
	EDexterity = 0 UMETA(DisplayName = "Dexterity Reward"),
	EStrength = 1 UMETA(DisplayName = "Strength Reward"),
	EIntelligence = 2 UMETA(DisplayName = "Intelligence Reward")
};

UCLASS()
class DUNGEONCRAWLER2026_API ABloodDrinker : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABloodDrinker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartMovingToPlayer();
	void StartAttackingPlayer();

	// Stop moving/attacking
	void StopAllBehaviorTasks();

	void StopMovingToPlayer();
	void StopAttackingPlayer();

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
	void FindPlayer();

	UFUNCTION()
	void MoveToPlayer();

	UFUNCTION()
	void AttackPlayer();

	bool WithinTheRangeOfNextFloor(AActor* floorActor_);
	bool IsPlayerNearby();
	bool IsPlayerNearby(AActor* floorActor_);
	bool IsAnotherEnemyNearby(AActor* otherEnemy_);
	bool IsAnotherEnemyNearby(AActor* floorActor_, AActor* otherEnemy_);

	UFUNCTION()
	void UpdateHealth(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType);

	UFUNCTION()
	void DestroyBloodDrinker();

	UFUNCTION()
	void ResetAnimation();

private:
	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
	UBehaviorTree* behaviorTree = nullptr;

	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
	FName bloodDrinkerKey = "BloodDrinkerKey";

	UPROPERTY(EditAnywhere, Category = "Behavior Tree")
	FName playerKeyName = "PlayerKey";

	FTimerHandle findPlayerHandle{};

	// Moving action
	UPROPERTY(EditAnywhere, Category = "Action Time")
	float timeToMove = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Action Time")
	float timeToAttack = 0.5f;

	FTimerHandle moveToPlayerHandle{};
	FTimerHandle attackPlayerHandle{};

	UPROPERTY(EditAnywhere, Category = "Distance to Floor")
	float maxDistanceToNextFloor = 550.0f;

	UPROPERTY(EditAnywhere, Category = "Distance to Floor")
	float minDistanceToNextFloor = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Distance to Floor", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float dotProductValue = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (ClampMin = "0.01"))
	float damageValue = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UHealthComponent> healthComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UPaperSpriteComponent> spriteComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Enemy Killed Rewards")
	ESkillReward skillReward = ESkillReward::EDexterity;

	UPROPERTY(EditAnywhere, Category = "Enemy Killed Rewards")
	int incrementSkillValue = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Sounds")
	TArray<TObjectPtr<USoundBase>> hitSounds; // 0 for player hit sound, 1 for enemy hit sound

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Animations")
	TObjectPtr<UAnimSequence> enemyAttackAnimation = nullptr;
};
