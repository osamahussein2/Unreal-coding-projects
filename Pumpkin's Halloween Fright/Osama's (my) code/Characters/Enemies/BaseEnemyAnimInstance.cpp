// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/BaseEnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBaseEnemyAnimInstance::NativeInitializeAnimation()
{
	EnemyCharacter = Cast<ABaseEnemy>(TryGetPawnOwner());

	if (EnemyCharacter) EnemyCharacterMovement = EnemyCharacter->GetCharacterMovement();
}

void UBaseEnemyAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	Super::NativeUpdateAnimation(deltaTime);

	if (EnemyCharacter)
	{
		Velocity = EnemyCharacterMovement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(EnemyCharacterMovement->Velocity);
		bIsInAir = EnemyCharacterMovement->IsFalling();
		bShouldMove = GroundSpeed > 3.f && EnemyCharacterMovement->GetCurrentAcceleration() != FVector::ZeroVector;
		isDead = EnemyCharacter->GetHealthComponent()->IsDead();
	}
}