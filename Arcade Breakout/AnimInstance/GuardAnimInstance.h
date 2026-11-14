// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GuardAnimInstance.generated.h"

class AGuard;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class SHROOMJAM2026_API UGuardAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UGuardAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = "References", meta = (AllowPrivateAccess = true))
	TObjectPtr<AGuard> guard = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "References", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCharacterMovementComponent> guardMovementComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	float groundSpeed = 0.0f;
};
