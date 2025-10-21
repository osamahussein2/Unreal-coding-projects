// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaminaComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CurrentStamina = MaxStamina;
	CurrentMaxSpeed = RunSpeed;
	TargetMaxSpeed = RunSpeed;

	if (OwnerCharacter)
	{
		if (UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement())
		{
			MovementComp->MaxWalkSpeed = RunSpeed;
		}
	}
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!OwnerCharacter) return;

	UpdateStamina(DeltaTime);
	UpdateMovementSpeed(DeltaTime);
}

void UStaminaComponent::StartSprinting()
{
	bWantsToSprint = true;
}

void UStaminaComponent::StopSprinting()
{
	bWantsToSprint = false;
}

bool UStaminaComponent::CanSprint() const
{
	if (!OwnerCharacter) return false;

	UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement();
	if (!MovementComp) return false;
	if (bIsExhausted) return false;
	if (!MovementComp->IsMovingOnGround()) return false;
	FVector Velocity = OwnerCharacter->GetVelocity();
	Velocity.Z = 0.0f;
	float CurrentSpeed = Velocity.Size();

	if (CurrentSpeed < SprintInputThreshold)
	{
		return false;
	}
	if (bIsSprinting)
	{
		return CurrentStamina > 0.f;
	}
	else
	{
		return CurrentStamina >= MinStaminaToStartSprinting;
	}
}

void UStaminaComponent::UpdateMovementSpeed(float DeltaTime)
{
	if (!OwnerCharacter) return;

	UCharacterMovementComponent* MovementComp = OwnerCharacter->GetCharacterMovement();
	if (!MovementComp) return;

	if (bWantsToSprint && CanSprint())
	{
		TargetMaxSpeed = SprintSpeed;
		bIsSprinting = true;
	}
	else
	{
		TargetMaxSpeed = RunSpeed;
		bIsSprinting = false;
	}

	CurrentMaxSpeed = FMath::FInterpTo(CurrentMaxSpeed, TargetMaxSpeed, DeltaTime, SpeedTransitionRate);
	MovementComp->MaxWalkSpeed = CurrentMaxSpeed;
}

void UStaminaComponent::UpdateStamina(float DeltaTime)
{
	if (bIsSprinting)
	{
		CurrentStamina -= StaminaDrainRate * DeltaTime;
		CurrentStamina = FMath::Max(CurrentStamina, 0.f);

		TimeSinceStoppedRunning = 0.f;

		if (CurrentStamina <= ExhaustedThreshold && !bIsExhausted)
		{
			bIsExhausted = true;
			OnStaminaDepleted.Broadcast();
		}
	}
	else
	{
		TimeSinceStoppedRunning += DeltaTime;

		if (TimeSinceStoppedRunning >= StaminaRegenDelay)
		{
			CurrentStamina += StaminaRegenRate * DeltaTime;
			CurrentStamina = FMath::Min(CurrentStamina, MaxStamina);

			if (bIsExhausted && CurrentStamina >= RecoveryThreshold)
			{
				bIsExhausted = false;
				OnStaminaRecovered.Broadcast();
			}
		}
	}
}

