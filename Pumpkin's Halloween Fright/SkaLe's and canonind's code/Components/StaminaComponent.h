// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaRecovered);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GDTV_HALLOWEEN2025_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStaminaComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* StaminaComponent API */
	void StartSprinting();
	void StopSprinting();
	bool IsSprinting() const { return bIsSprinting; }
	bool CanSprint() const;
	float GetStaminaPercent() const { return CurrentStamina / MaxStamina; }
	bool IsExhausted() const { return bIsExhausted; }

protected:
	void UpdateMovementSpeed(float DeltaTime);
	void UpdateStamina(float DeltaTime);

public:
	UPROPERTY(BlueprintAssignable)
	FOnStaminaDepleted OnStaminaDepleted;
	UPROPERTY(BlueprintAssignable)
	FOnStaminaRecovered OnStaminaRecovered;

	/* Stamina */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxStamina = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaDrainRate = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenRate = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float StaminaRegenDelay = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MinStaminaToStartSprinting = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float ExhaustedThreshold = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float RecoveryThreshold = 30.f;

	/* Movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 800.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SpeedTransitionRate = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintInputThreshold = 0.1f;

private:
	UPROPERTY(VisibleAnywhere)
	float CurrentStamina = 0.f;

	bool bIsSprinting = false;
	bool bWantsToSprint = false;
	bool bIsExhausted = false;
	float CurrentMaxSpeed = 0.f;
	float TargetMaxSpeed = 0.f;
	float TimeSinceStoppedRunning = 0.f;

	ACharacter* OwnerCharacter = nullptr;
};
