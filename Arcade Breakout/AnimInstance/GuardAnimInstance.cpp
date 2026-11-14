// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimInstance/GuardAnimInstance.h"
#include "Guard.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UGuardAnimInstance::UGuardAnimInstance(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGuardAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (!TryGetPawnOwner()) return;

	guard = Cast<AGuard>(TryGetPawnOwner());
	if (!guard) return;

	guardMovementComponent = guard->GetCharacterMovement();
}

void UGuardAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (guard && guardMovementComponent)
	{
		groundSpeed = UKismetMathLibrary::VSizeXY(guardMovementComponent->Velocity);
	}
}