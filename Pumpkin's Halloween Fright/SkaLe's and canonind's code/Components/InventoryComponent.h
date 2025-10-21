// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCandyPickedUp, float, CandyCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCandyEaten, float, CandyCount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GDTV_HALLOWEEN2025_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	void AddCandy();
	void EatCandy();
	uint32 GetCandyCount() const; 
	float GetCandyHealAmount() const;
	bool HasCandy() const;
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Candy")
	FOnCandyPickedUp OnCandyPickedUp;

	UPROPERTY(BlueprintAssignable, Category = "Candy")
	FOnCandyPickedUp OnCandyEaten;

protected:
	UPROPERTY(EditAnywhere, Category = "Candy")
	uint32 CandyCounter = 0;

	UPROPERTY(EditAnywhere, Category = "Candy")
	float CandyHealthAmount = 20.f;
};
