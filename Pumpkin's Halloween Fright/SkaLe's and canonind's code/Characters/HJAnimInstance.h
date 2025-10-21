// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HJAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UHJAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float deltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "References")
	class AHJPlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "References")
	class UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsDead;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsBlocking;
	
};
