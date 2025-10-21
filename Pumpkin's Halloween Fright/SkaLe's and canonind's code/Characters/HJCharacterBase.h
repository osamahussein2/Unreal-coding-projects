// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HJCharacterBase.generated.h"

class UHealthComponent;

UCLASS()
class GDTV_HALLOWEEN2025_API AHJCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	AHJCharacterBase();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	UHealthComponent* GetHealthComponent() const {return HealthComponent;}

protected:
	virtual void BeginPlay() override;

	/* Health Component callbacks*/
	// Override in PlayerCharacter for UI update
	UFUNCTION()
	virtual void OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType);

	UFUNCTION()
	virtual void OnDeath(UHealthComponent* HealthComp, AActor* Killer, const class UDamageType* DamageType);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

};
