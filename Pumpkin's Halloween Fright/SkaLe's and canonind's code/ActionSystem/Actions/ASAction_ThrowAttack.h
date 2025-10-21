// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/ASAction_MontagePlay.h"
#include "ASAction_ThrowAttack.generated.h"

class UActionSystemComponent;
class APlayerProjectile;

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UASAction_ThrowAttack : public UASAction_MontagePlay
{
	GENERATED_BODY()

public:
	virtual bool OnAnimNotifyReceived(UActionSystemComponent* ActionComp, FName EventName) override;

protected:
	UFUNCTION()
	void SpawnProjectile(UActionSystemComponent* ActionComp);

	/*
	* Calculates launch angle and speed to hit TargetPos from StartPos under Gravity
	* Returns true if the target is reachable within MaxSpeed limits
	* 
	* It tries to pick the lowest angle from given range that satisfies MaxSpeed restriction
	*/
	bool CalculateBalisticTrajectory(const FVector& StartPos, const FVector& TargetPos, float MaxSpeed, float Gravity, float& OutPitch, float& OutSpeed, FVector& OutLaunchDirection);

protected:

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<APlayerProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	FName SpawnSocket;

	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (ClampMin="0.0", ClampMax="90.0"))
	float MinPitch = 15.f;

	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (ClampMin="0.0", ClampMax="90.0"))
	float MaxPitch = 75.f;

	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (ClampMin="0.0", ClampMax="5000.0"))
	float MaxProjectileSpeed = 3000.f;

	/* Default project gravity */
	UPROPERTY(EditAnywhere, Category = "Projectile")
	float DefaultGravity = 980.f;

	// This is set by target detection or calculated from character location + forward vector
	FVector TargetLocation;

};
