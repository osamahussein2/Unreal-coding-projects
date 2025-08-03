// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAnimInstance.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	bossCharacter = Cast<ABossCharacter>(TryGetPawnOwner());

	if (bossCharacter)
	{
		bossCharacterMovement = bossCharacter->GetCharacterMovement();
	}
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// Basically, if the player isn't in the menu and not paused the game yet
	if (!APlayerCharacter::IsGamePaused() && !APlayerCharacter::isInMenu)
	{
		if (bossCharacterMovement)
		{
			bossMoveSpeed = UKismetMathLibrary::VSizeXY(bossCharacterMovement->Velocity);
		}

		if (bossCharacter)
		{
			bossAttackNumber = bossCharacter->GetAttackNumber();
			bossDied = bossCharacter->GetIsDead();
			bossAttacked = bossCharacter->GetIsAttacked();
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
