// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/HJCharacterBase.h"
#include "Characters/HJPlayerCharacter.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/WidgetComponent.h"
#include "EnemyWidget.h"
#include "Combat/Enemies/BaseEnemyProjectile.h"
#include "BaseEnemy.generated.h"

UCLASS()
class GDTV_HALLOWEEN2025_API ABaseEnemy : public AHJCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Health Component callbacks from HJCharacterBase class
	virtual void OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType) override;
	virtual void OnDeath(UHealthComponent* HealthComp, AActor* Killer, const class UDamageType* DamageType) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AHJPlayerCharacter* GetPlayerCharacter() const;
	float GetThrowAngle() const;
	float GetHomingProjectileSpeed() const;

	UPROPERTY(EditAnywhere, Category = "Patrol Routes")
	TArray<FVector> patrolRoutes;

private:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnTargetPerceptionForgotten(AActor* TargetActor);

	void PerformLogicWhileAlive(float& DeltaTime);
	void ExecuteDeathLogic(float& DeltaTime);

	void PlayAttackAnimation();

	void PlayHitReactAnimation();

	void ChangeEnemyPatrolPoint();

	void SpawnProjectile();

	void EnableMovement();

private:
	/*UPROPERTY(EditAnywhere, Category = "Parameters")
	float maxTimeToSpawnProjectile = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectile> projectile;

	float projectileSpawnTime;*/

	AAIController* aiController;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactionMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAIPerceptionComponent* aiPerception;

	// Combat variables
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UWidgetComponent* HealthBar;

	UEnemyWidget* enemyWidget;

	bool isAttacking;
	bool canMove;

	bool seenPlayer;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	float distanceUntilAttacking = 180.0f;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	float stopAtPlayerRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Parameters")
	float loseSightRadius = 1200.0f;

	float alpha;

	UMaterialInstanceDynamic* matInstanceDynamic[4];

	UPROPERTY(EditDefaultsOnly, Category = "Array Size")
	int materialSlotsSize = 4;

	AHJPlayerCharacter* playerCharacter;

	UPROPERTY(EditAnywhere, Category = "Patrol Routes")
	float stopAtPatrolRadius = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Patrol Routes")
	float distanceToSwitchPatrolRoute = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Patrol Routes")
	float maxWaitTime = 0.7f;

	int patrolIndex;
	float waitTimer;

	float attackTimer;

	// Projectile variables
	bool enemySpawnedProjectile;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<ABaseEnemyProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (ClampMin = "0.0", ClampMax = "90.0"))
	float ThrowAngle;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float homingProjectileSpeed = 2.0f;

	// Combat variables
	UPROPERTY(EditAnywhere, Category = "Combat")
	float timeUntilAttackAt = 1.3f;

	bool isEnemyHit;

	// Made it visible anywhere just in case you want to see what it's spawned rotation is
	UPROPERTY(VisibleAnywhere)
	FRotator spawnRotation;
};
