// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HJAnimInstance.h"
#include "Characters/HJPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UHJAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<AHJPlayerCharacter>(TryGetPawnOwner());
	PlayerCharacterMovement = PlayerCharacter ? PlayerCharacter->GetCharacterMovement() : nullptr;
}

void UHJAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	Super::NativeUpdateAnimation(deltaTime);

	if (PlayerCharacterMovement)
	{
		Velocity = PlayerCharacterMovement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);
		bIsInAir = PlayerCharacterMovement->IsFalling();
		bShouldMove = GroundSpeed > 3.f && PlayerCharacterMovement->GetCurrentAcceleration() != FVector::ZeroVector;
	}
	if (PlayerCharacter)
	{
		bIsSprinting = PlayerCharacter->IsSprinting();
		bIsDead = PlayerCharacter->IsDead();
		bIsBlocking = PlayerCharacter->IsBlocking();
	}


}
