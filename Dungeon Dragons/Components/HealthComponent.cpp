// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bTickEvenWhenPaused = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()) GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);

	currentHealth = maxHealth;
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//HandleTakeAnyDamage(GetOwner(), 2.0f, nullptr, nullptr, nullptr);
}

void UHealthComponent::TakeDamage(AActor* damagedActor_, float damagedAmount_, AActor* damageCauser_)
{
	HandleTakeAnyDamage(damagedActor_, damagedAmount_, nullptr, nullptr, damageCauser_);
}

void UHealthComponent::Heal(float healAmount_)
{
	currentHealth += healAmount_;

	currentHealth = FMath::Clamp(currentHealth, 0.0f, maxHealth);
}

void UHealthComponent::ResetHealth()
{
	if (currentHealth != maxHealth) currentHealth = maxHealth;
	if (isDead) isDead = false;
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	currentHealth = FMath::Clamp(currentHealth - Damage, 0.0f, maxHealth);

	OnHealthChanged.Broadcast(this, currentHealth, -Damage, DamageType);

	if (currentHealth <= 0.0f && OnDeath.IsBound() && !isDead) 
	{
		OnDeath.Broadcast();
		isDead = true;
	}
}