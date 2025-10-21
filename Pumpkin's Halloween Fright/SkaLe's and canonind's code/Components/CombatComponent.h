// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class ATargetIndicator;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GDTV_HALLOWEEN2025_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	FVector GetTargetLocation();
	bool IsBlocking() const;
	void StartBlocking(float DamageReductionFactor);
	void StopBlocking();
private:
	void DetectEnemies();
	void UpdateIndicator(float DeltaTime);
	FVector GetTargetLocationXY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat");
	TSubclassOf<ATargetIndicator> IndicatorClass;

	/* Distance for projectile to fly if no target detected */
	UPROPERTY(EditAnywhere, Category = "Combat")
	float DefaultThrowDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat");
	float DetectionRange = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DetectionAngle = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FName EnemyTag = FName("Enemy");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float IndicatorPulseScale = 0.25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float IndicatorScale = 1.f;

private:
	UPROPERTY()
	TObjectPtr<AActor> ClosestEnemy;
	UPROPERTY()
	TObjectPtr<AActor> Indicator;

	bool bIsBlocking;

};
