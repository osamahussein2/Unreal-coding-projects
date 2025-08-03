// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (playerCharacter)
	{
		playerCharacterMovement = playerCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// Basically, if the player isn't in the menu and not paused the game yet
	if (!APlayerCharacter::IsGamePaused() && !APlayerCharacter::isInMenu)
	{
		if (playerCharacterMovement)
		{
			moveSpeed = UKismetMathLibrary::VSizeXY(playerCharacterMovement->Velocity);
		}

		if (playerCharacter)
		{
			attackNumber = playerCharacter->GetAttackNumber();
			died = playerCharacter->GetIsDead();
			isAttacked = playerCharacter->GetIsAttacked();
		}

		// Resume animation
		if (GetOwningComponent()->GlobalAnimRateScale != 1.0f) GetOwningComponent()->GlobalAnimRateScale = 1.0f;
	}

	else if (APlayerCharacter::IsGamePaused() && !APlayerCharacter::isInMenu)
	{
		// Pause animation
		if (GetOwningComponent()->GlobalAnimRateScale != 0.0f) GetOwningComponent()->GlobalAnimRateScale = 0.0f;
	}
}