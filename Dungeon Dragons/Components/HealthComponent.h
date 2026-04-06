// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, UHealthComponent*, HealthComp, float, Health, 
	float, HealthDelta, const class UDamageType*, DamageType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCRAWLER2026_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetHealth() const { return currentHealth; }
	float GetMaxHealth() const { return maxHealth; }

	float GetHealthPercentage() const { return currentHealth / maxHealth; }

	bool IsHealthFull() const { return FMath::IsNearlyEqual(currentHealth, maxHealth); }
	bool IsDead() const { return isDead; }

	void TakeDamage(AActor* damagedActor_, float damagedAmount_, AActor* damageCauser_);
	void Heal(float healAmount_);

	void ResetHealth();

public: // Public variables
	UPROPERTY(VisibleAnywhere, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	FOnDeath OnDeath;

private:
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
		class AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = "0.0"))
	float maxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float currentHealth = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	bool isDead = false;
};
