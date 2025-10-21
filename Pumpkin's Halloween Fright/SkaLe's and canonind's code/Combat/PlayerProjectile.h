// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpactSystem/SurfaceImpactTypes.h"
#include "PlayerProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class GDTV_HALLOWEEN2025_API APlayerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerProjectile();

	void LaunchProjectile(const FVector& Direction);
	/* Fast solution to avoid deriving this class */
	void LaunchProjectileWithSpeed(const FVector& InDirection, float InInitialSpeed);

	void SetInstigatorActor(AActor* InInstigator) { InstigatorActor = InInstigator; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,FVector NormalImpulse, const FHitResult& Hit);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	UNiagaraSystem* TrailEffect;

	UPROPERTY()
	UNiagaraComponent* TrailComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX")
	ESurfaceImpactType ImpactType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Damage = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float LifeTime = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float InitialSpeed = 1500.f;

	UPROPERTY()
	AActor* InstigatorActor = nullptr;

};
