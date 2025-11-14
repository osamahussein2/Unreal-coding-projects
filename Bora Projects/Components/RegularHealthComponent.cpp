// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/RegularHealthComponent.h"

// Sets default values for this component's properties
URegularHealthComponent::URegularHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	updatedHealth = 0.0f;
	currentHealth = 0.0f;

	isDamaged = false;
}


// Called when the game starts
void URegularHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	currentHealth = maxHealth;
	updatedHealth = currentHealth;
}


// Called every frame
/*void URegularHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}*/

float URegularHealthComponent::GetMaxHealth() const
{
	return maxHealth;
}

float URegularHealthComponent::GetUpdatedHealth() const
{
	return updatedHealth;
}

float URegularHealthComponent::GetCurrentHealth() const
{
	return currentHealth;
}

bool URegularHealthComponent::IsDead() const
{
	return isDead;
}

bool URegularHealthComponent::IsHealthDraining() const
{
	return isDamaged && !FMath::IsNearlyEqual(currentHealth, updatedHealth, 0.2f);
}

void URegularHealthComponent::UpdateHealthDamaged(float& DeltaTime)
{
	if (IsHealthDraining())
	{
		updatedHealth -= healthUpdateSpeed * DeltaTime;
	}

	else if (isDamaged && FMath::IsNearlyEqual(currentHealth, updatedHealth, 0.2f))
	{
		updatedHealth = currentHealth;
		isDamaged = false;
	}

}

void URegularHealthComponent::SetDamaged(float newValue)
{
	currentHealth -= newValue;
	isDamaged = true;

	if (currentHealth <= 0.0f && !isDead)
	{
		currentHealth = 0.0f;
		isDead = true;

		isDamaged = false;
	}
}