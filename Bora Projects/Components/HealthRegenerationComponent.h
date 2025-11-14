// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/Delegate.h"
#include "HealthRegenerationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartHealing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetDamaged, float, newValue);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHROOMJAM2025_API UHealthRegenerationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthRegenerationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetMaxHealth() const;
	float GetUpdatedHealth() const;
	float GetCurrentHealth() const;

	float GetTimeUntilHeal() const;

	bool IsHealing() const;
	bool IsDamaged() const;

	bool UpdatedHealthReachedCurrentHealth() const;

	bool IsDead() const;

	bool IsHealthDraining() const;
	bool IsHealthRenegenerating() const;

	void UpdateHealthDamaged(float& DeltaTime);
	void UpdateHealthRenegeneration(float& DeltaTime);

	void SetDamaged(float newValue);
	void SetHealing();

	FOnStartHealing StartHealing;
	FOnSetDamaged StartDamaged;

private:

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float maxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float currentHealth;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float updatedHealth;

	UPROPERTY(EditAnywhere, Category = "Health")
	float timeUntilHeal = 3.0f;

	bool isHealing;
	bool isDamaged;

	UPROPERTY(EditAnywhere, Category = "Health")
	float healthRegenerateSpeed = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float healthDrainSpeed = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float exposureThreshold = 0.2f;

	bool isDead;
};
