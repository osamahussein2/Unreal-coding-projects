// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/PlayerProjectile.h"
#include "ImpactSystem/SurfaceImpactTypes.h"
#include "BaseEnemyProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class ABaseEnemy;

UCLASS()
class GDTV_HALLOWEEN2025_API ABaseEnemyProjectile : public APlayerProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemyProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void TargetHomingProjectileOnPlayer();

	void DestroyProjectile();

private:
	float lifetime;

	ABaseEnemy* baseEnemy = nullptr;
};