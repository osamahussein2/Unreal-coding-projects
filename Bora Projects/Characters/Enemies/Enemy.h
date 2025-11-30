// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/MainEnemy.h"
#include "AIController.h"
//#include "Perception/AIPerceptionComponent.h"
//#include "Perception/AISenseConfig_Sight.h"
#include "Enemy.generated.h"

UENUM()
enum class EnemyState : uint8
{
	EPatrolling,
	ECheckingOutDuck,
	ELosingPlayer,
	ESeenPlayer,
	EDetectedPlayer
};

UCLASS()
class SHROOMJAM2025_API AEnemy : public AMainEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EnemyState GetEnemyState() const { return enemyState; }
	void SetEnemyState(EnemyState enemyState_);

	void EnemyIsCloseToDuck();
	void SetEnemyToDetectPlayer();
	void GoToDuckLocation(FVector&& duckLocation_);

private:
	FVector GetDirectionVector(FVector& otherLocation_);
	FVector GetDirectionVector(FVector&& otherLocation_);

	void UpdateEnemyDetectionBarUponSeen(float& DeltaTime);
	void UpdateEnemyDetectionBarUponLost(float& DeltaTime);

	void HideEnemyWidgetUponSeen();
	void HideEnemyWidgetUponLost();

	void DamagePlayer(float& DeltaTime);

	void StartAnimatingBlender();

	void PatrolAround();
	void PatrolToDuckLocation(float& DeltaTime);


	/*UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnTargetPerceptionForgotten(AActor* TargetActor);*/

private:
	/*UPROPERTY(EditAnywhere, Category = "Components")
	UAIPerceptionComponent* aiPerception;

	UPROPERTY(EditAnywhere, Category = "Components")
	UAISenseConfig_Sight* SightConfig;*/

	AAIController* aiController;

	UPROPERTY(EditAnywhere, Category = "Stop Radius")
	float stoppingRadius = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Detection Parameters")
	float distanceToDetect = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Detection Parameters", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float minDetectionDotProductValue = 0.7f;

	UPROPERTY(EditAnywhere, Category = "Detection Parameters")
	float timeToHideWidget = 1.0f;

	bool timerCleared;

	class APlayerCharacter* player;

	// Patrol route variables
	UPROPERTY(EditAnywhere, Category = "Patrol Routes")
	TArray<FVector> patrolRoutes;

	UPROPERTY(EditAnywhere, Category = "Patrol Routes")
	float stopAtPatrolRadius = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Patrol Routes")
	float distanceToSwitchPatrolRoute = 60.0f;

	int patrolIndex;

	// Duck patrol route variables
	UPROPERTY(EditAnywhere, Category = "Duck Patrol")
	float distanceToDuck = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Duck Patrol")
	float timeToPatrolAgain = 2.0f;

	float checkingOutDuckTimer;

	FVector duckLocation;

	EnemyState enemyState;
	EnemyState previousEnemyState;
};
