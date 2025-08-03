// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GMTKJAM2025_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	ABossCharacter* bossCharacter;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* bossCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float bossMoveSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Attack)
	int bossAttackNumber;

	UPROPERTY(BlueprintReadOnly, Category = Attack)
	bool bossAttacked;

	UPROPERTY(BlueprintReadOnly, Category = Death)
	bool bossDied;
};
