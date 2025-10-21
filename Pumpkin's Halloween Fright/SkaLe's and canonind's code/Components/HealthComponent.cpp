// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bIsDead = false;

	bEnableHealthRegen = false;
	HealthRegenRate = 5.0f;
	HealthRegenDelay = 3.0f;
	HealthRegenTickRate = 0.1f;

	bCanBecomeInvulnerable = false;
	bIsInvulnerable = false;

	deathTimer = 0.0f;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
	}
	
	CurrentHealth = MaxHealth;

	if (deathTimer != 0.0f) deathTimer = 0.0f;
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || bIsDead) return;

	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);

	float ActualHeal = CurrentHealth - OldHealth;
	if (ActualHeal > 0.0f)
	{
		OnHealthChanged.Broadcast(this, CurrentHealth, ActualHeal, nullptr);
	}
}

void UHealthComponent::SetHealth(float NewHealth)
{
	if (bIsDead) return;;

	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

	float HealthDelta = CurrentHealth - OldHealth;
	if (!FMath::IsNearlyZero(HealthDelta))
	{
		OnHealthChanged.Broadcast(this, CurrentHealth, HealthDelta, nullptr);
		if (CurrentHealth <= 0.0f)
		{
			bIsDead = true;
			StopHealthRegeneration();
			OnDeath.Broadcast(this, nullptr, nullptr);
		}
	}
}

void UHealthComponent::Kill()
{
	if (bIsDead) return;

	SetHealth(0.0f);
}

void UHealthComponent::Revive(float ReviveHealth /*= -1.0f*/)
{
	if (!bIsDead) return;

	bIsDead = false;
	if (ReviveHealth < 0.0f)
	{
		ReviveHealth = MaxHealth;
	}

	SetHealth(ReviveHealth);

	if (bEnableHealthRegen)
	{
		StartHealthRegeneration();
	}
}

void UHealthComponent::SetInvulnerable(bool bInvulnerable)
{
	if (!bCanBecomeInvulnerable) return;

	bIsInvulnerable = bInvulnerable;
}

void UHealthComponent::SetInvulnerableTemporarily(float Duration)
{
	if (!bCanBecomeInvulnerable || Duration <= 0.0f) return;

	SetInvulnerable(true);

	FTimerHandle Invulnerability_TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(Invulnerability_TimerHandle, [this]()
		{
			SetInvulnerable(false);
		}, Duration, false);
}

void UHealthComponent::SetDamageModifier(float Modifier)
{
	DamageModifier = Modifier;
}

void UHealthComponent::RestoreDamageModifier()
{
	DamageModifier = 1.f;
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead || bIsInvulnerable) return;

	StopHealthRegeneration();
	GetWorld()->GetTimerManager().ClearTimer(RegenDelay_TimerHandle);

	float OldHealth = CurrentHealth;
	float FinalDamage = Damage * DamageModifier;
	CurrentHealth = FMath::Clamp(CurrentHealth - FinalDamage, 0.0f, MaxHealth);

	float ActualDamage = OldHealth - CurrentHealth;

	OnHealthChanged.Broadcast(this, CurrentHealth, -ActualDamage, DamageType);

	if (CurrentHealth <= 0.0f && !bIsDead)
	{
		bIsDead = true;
		OnDeath.Broadcast(this, DamageCauser, DamageType);
	}
	else if (bEnableHealthRegen)
	{
		GetWorld()->GetTimerManager().SetTimer(RegenDelay_TimerHandle, this, &UHealthComponent::StartHealthRegeneration, HealthRegenDelay, false);
	}
}

void UHealthComponent::StartHealthRegeneration()
{
	if (!bEnableHealthRegen || bIsDead || IsAtFullHealth()) return;

	GetWorld()->GetTimerManager().SetTimer(RegenTick_TimerHandle, this, &UHealthComponent::RegenerateHealth, HealthRegenTickRate, true);
}

void UHealthComponent::RegenerateHealth()
{
	if (bIsDead || IsAtFullHealth())
	{
		StopHealthRegeneration();
		return;
	}

	float RegenAmount = HealthRegenRate * HealthRegenTickRate;
	Heal(RegenAmount);
}

void UHealthComponent::StopHealthRegeneration()
{
	GetWorld()->GetTimerManager().ClearTimer(RegenTick_TimerHandle); 
}

