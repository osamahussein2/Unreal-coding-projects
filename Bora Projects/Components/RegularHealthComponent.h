// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RegularHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamaged, float, newValue);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHROOMJAM2025_API URegularHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URegularHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetMaxHealth() const;
	float GetUpdatedHealth() const;
	float GetCurrentHealth() const;

	bool IsDead() const;

	bool IsHealthDraining() const;

	void UpdateHealthDamaged(float& DeltaTime);

	void SetDamaged(float newValue);

	FOnDamaged StartDamaged;

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float maxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float currentHealth;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float updatedHealth;

	UPROPERTY(EditAnywhere, Category = "Health")
	float healthUpdateSpeed = 15.0f;

	bool isDead;
	bool isDamaged;
};
