// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, UHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeath,  UHealthComponent*, HealthComp, AActor*, Killer, const class UDamageType*, DamageType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GDTV_HALLOWEEN2025_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercentage() const { return CurrentHealth / MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAtFullHealth() const { return FMath::IsNearlyEqual(CurrentHealth, MaxHealth); }

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Kill();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Revive(float ReviveHealth = -1.0f);

	UFUNCTION(BlueprintCallable, Category = "Health|Invulnerability")
	void SetInvulnerable(bool bInvulnerable);

	UFUNCTION(BlueprintCallable, Category = "Health|Invulnerability")
	void SetInvulnerableTemporarily(float Duration);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetDamageModifier(float Modifier);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void RestoreDamageModifier();

protected:
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void StartHealthRegeneration();
	void RegenerateHealth();
	void StopHealthRegeneration();

public:
	/* Delegates */
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

	float deathTimer;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (ClampMin = "0.0"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health");
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health");
	bool bIsDead;
	
	/* Regeneration */ // Not sure if we need this 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Health|Regeneration")
	bool bEnableHealthRegen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Health|Regeneration", meta = (EditCondition = "bEnableHealthRegen"))
	float HealthRegenRate; // Health per second

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Health|Regeneration", meta = (EditCondition = "bEnableHealthRegen"))
	float HealthRegenDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Health|Regeneration", meta = (EditCondition = "bEnableHealthRegen"))
	float HealthRegenTickRate;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health|Invulnerability")
	bool bCanBecomeInvulnerable;

	UPROPERTY(BlueprintReadOnly, Category = "Health|Invulnerability")
	bool bIsInvulnerable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float DamageModifier = 1.f;

	FTimerHandle RegenDelay_TimerHandle;
	FTimerHandle RegenTick_TimerHandle;

};
