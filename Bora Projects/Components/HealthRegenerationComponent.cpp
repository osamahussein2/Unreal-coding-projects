// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthRegenerationComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UHealthRegenerationComponent::UHealthRegenerationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	updatedHealth = 0.0f;
	currentHealth = 0.0f;

	isHealing = false;
}


// Called when the game starts
void UHealthRegenerationComponent::BeginPlay()
{
	Super::BeginPlay();
	
	currentHealth = maxHealth;
	updatedHealth = currentHealth;
}


// Called every frame
//void UHealthRegenerationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

float UHealthRegenerationComponent::GetMaxHealth() const
{
	return maxHealth;
}

float UHealthRegenerationComponent::GetUpdatedHealth() const
{
	return updatedHealth;
}

float UHealthRegenerationComponent::GetCurrentHealth() const
{
	return currentHealth;
}

float UHealthRegenerationComponent::GetTimeUntilHeal() const
{
	return timeUntilHeal;
}

bool UHealthRegenerationComponent::IsHealing() const
{
	return isHealing;
}

bool UHealthRegenerationComponent::IsDamaged() const
{
	return isDamaged;
}

bool UHealthRegenerationComponent::UpdatedHealthReachedCurrentHealth() const
{
	return FMath::IsNearlyEqual(currentHealth, updatedHealth, exposureThreshold);
}

bool UHealthRegenerationComponent::IsDead() const
{
	return isDead;
}

bool UHealthRegenerationComponent::IsHealthDraining() const
{
	return isDamaged && !FMath::IsNearlyEqual(currentHealth, updatedHealth, exposureThreshold);
}

bool UHealthRegenerationComponent::IsHealthRenegenerating() const
{
	return isHealing && !FMath::IsNearlyEqual(currentHealth, updatedHealth, exposureThreshold);
}

void UHealthRegenerationComponent::UpdateHealthDamaged(float& DeltaTime)
{
	if (!FMath::IsNearlyEqual(currentHealth, updatedHealth, exposureThreshold))
	{
		updatedHealth -= healthDrainSpeed * DeltaTime;
	}

	else
	{
		updatedHealth = currentHealth;
		isDamaged = false;
	}
}

void UHealthRegenerationComponent::UpdateHealthRenegeneration(float& DeltaTime)
{
	static bool musicPlayed;

	if (IsHealthRenegenerating())
	{
		if (!musicPlayed)
		{
			UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(this,
				TEXT("/Game/Sounds/health_regen_3__chord_higher_")));

			musicPlayed = true;
		}

		currentHealth += healthRegenerateSpeed * DeltaTime;
	}

	else
	{
		musicPlayed = false;

		currentHealth = updatedHealth;
		isHealing = false;
	}
}

void UHealthRegenerationComponent::SetDamaged(float newValue)
{
	currentHealth -= newValue;

	if (currentHealth < 0.0f && !isDead)
	{
		currentHealth = 0.0f;
		isDead = true;
	}

	if (isDamaged != true) isDamaged = true;
	if (isHealing != false) isHealing = false;
}

void UHealthRegenerationComponent::SetHealing()
{
	updatedHealth += maxHealth;

	if (updatedHealth > maxHealth) updatedHealth = maxHealth;
		
	isHealing = true;
}