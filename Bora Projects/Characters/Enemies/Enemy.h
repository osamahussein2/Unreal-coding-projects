// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/MainEnemy.h"
#include "AIController.h"
//#include "Perception/AIPerceptionComponent.h"
//#include "Perception/AISenseConfig_Sight.h"
#include "Enemy.generated.h"

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

private:
	FVector GetDirectionVector(FVector& otherLocation_);
	FVector GetDirectionVector(FVector&& otherLocation_);

	void UpdatePlayerSeenBoolean();

	void UpdateEnemyDetectionBarUponSeen(float& DeltaTime);
	void UpdateEnemyDetectionBarUponLost(float& DeltaTime);

	void HideEnemyWidgetUponSeen();
	void HideEnemyWidgetUponLost();

	void DamagePlayer(float& DeltaTime);


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

	bool seenPlayer;

	UPROPERTY(EditAnywhere, Category = "Stop Radius")
	float stoppingRadius = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Detection Parameters")
	float distanceToDetect = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Detection Parameters", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float minDetectionDotProductValue = 0.7f;

	UPROPERTY(EditAnywhere, Category = "Detection Parameters")
	float timeToHideWidget = 1.0f;

	bool timerCleared;
	bool playerNear;

	bool playerDetected;

	class APlayerCharacter* player;
};
