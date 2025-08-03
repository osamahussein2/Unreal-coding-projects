// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class GMTKJAM2025_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Movement keys
	static FKey currentMoveBackKey;
	static FKey currentMoveForwardKey;
	static FKey currentMoveLeftKey;
	static FKey currentMoveRightKey;

	// Action keys
	static FKey currentPauseKey;
	static FKey currentAttackKey;

	static bool IsGamePaused() { return gamePaused; }
	static bool isInMenu;

	// Modify movement keys booleans
	static bool modifyMoveForwardKey;
	static bool modifyMoveBackKey;
	static bool modifyMoveLeftKey;
	static bool modifyMoveRightKey;

	// Modify action keys bool
	static bool modifyPauseKey;
	static bool modifyAttackKey;

	static void SetGamePaused(bool gamePaused_)
	{
		if (gamePaused != gamePaused_) gamePaused = gamePaused_;
	}

	static int GetAttackNumber() { return attackNumber; }

	static unsigned int phaseNumber;

	static float GetPlayerHealth() { return playerHealth; }
	static constexpr float GetPlayerMaxHealth() { return maxPlayerHealth; }

	static void SetPlayerHealth(float maxHealth_) { playerHealth = maxHealth_; }
	static void SetDamaged(float damagedValue_) { playerHealth -= damagedValue_; }

	static bool GetIsDead() { return isDead; }

	static float GetAttackingTime() { return attackTime; }
	static float GetDeathTime() { return deathTime; }

	static bool GetIsAttacked() { return isAttacked; }
	static float GetDamagedTime() { return damagedTime; }

	static void SetIsAttacked(bool isAttacked_) { isAttacked = isAttacked_; }

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float playerSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float cameraSpeed = 3.0f;

	static bool gamePaused;

	// Private functions
	void CheckForMovementInput();
	void CheckForActionInput();

	// Mouse input functions
	void MoveCameraHorizontally(float value_);
	void MoveCameraVertically(float value_);

	// Attack logic
	void PerformAttack(float DeltaTime_);
	void PerformComboAttacks();

	void HandleAttackComboLogic(float DeltaTime_);

	// Attacked logic
	void AnimateAttacked(float DeltaTime_);

	UPROPERTY(EditAnywhere)
	USpringArmComponent* springArmComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* cameraComponent;

	static int attackNumber;

	static float attackTime;

	static float playerHealth;
	static constexpr float maxPlayerHealth = 100.0f;

	static float deathTime;
	static bool isDead;

	static bool isAttacked;
	static float damagedTime;

	float timeAfterAttack;

	bool canPerformAttackCombos;
	int comboAttackNumber;

	float attackComboTime;
};
