// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossAIController.h"
#include "BossCharacter.generated.h"

UCLASS()
class GMTKJAM2025_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetStartingPosition() const;

	float GetHealth() const;
	static constexpr float GetMaxHealth() { return maxHealth; }

	void ResetBoss();

	static int GetAttackNumber() { return attackNumber; }
	static float GetAttackingTime() { return attackTime; }
	
	static bool GetIsDead() { return isDead; }
	float GetDeathTime() { return deathTime; }

	bool GetIsAttacked() { return isAttacked; }
	float GetDamagedTime() { return damagedTime; }

private:
	UPROPERTY(EditAnywhere)
	float distanceThreshold = 300.0f;

	FVector startingPosition;

	float health;
	static constexpr float maxHealth = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Boss Health Parameters")
	float minimumHealthLost = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Boss Health Parameters")
	float maximumHealthLost = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Player Health Parameters")
	float minimumPlayerHealthLost = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Player Health Parameters")
	float maximumPlayerHealthLost = 5.0f;

	bool canPlayerLandAttack;
	bool playerAttackLanded;

	bool canBossLandAttack;
	bool bossAttackLanded;

	int attackCombo;

private:
	void DamageLogic();
	void DamagePlayer();

	void LookAtPlayer();

	// Attack logic
	void PerformAttack(float DeltaTime_);
	void PerformAttackCombos();

	// Damaged logic
	void AnimateAttacked(float DeltaTime_);

	static int attackNumber;

	static float attackTime;

	UPROPERTY(EditAnywhere)
	float maxAttackTime = 3.0f;
	
	float deathTime;
	static bool isDead;

	bool isAttacked;
	float damagedTime;

	class APlayerCharacter* playerCharacter;
};
