// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GMTKJAM2025_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	APlayerCharacter* playerCharacter;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* playerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float moveSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Attack)
	int attackNumber;

	UPROPERTY(BlueprintReadOnly, Category = Attack)
	bool isAttacked;

	UPROPERTY(BlueprintReadOnly, Category = Death)
	bool died;
};
