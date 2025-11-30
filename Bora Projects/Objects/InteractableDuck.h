// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "InteractableDuck.generated.h"

UCLASS()
class SHROOMJAM2025_API AInteractableDuck : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableDuck();

	void ThrowDuck(float& DeltaTime);
	void MakeDuckFall(float& DeltaTime);

	void RemoveProjectileComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when other actors collide hits the collider
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& Hit);

	bool GetInsideTrigger() const { return insideTrigger; }
	UProjectileMovementComponent* GetProjectileMovement() const { return projectileMovement; }

	void CreateProjectileComponent();

	void SetTargetLocation(FVector&& targetLocation_);

	void SetThrowingAngleModifier(float value_);

	void SetDuckDiffuseValue(bool value_);

	void SetDuckCanTick(bool value_);

	void ResetToOriginalLocation();

private:
	bool IsPlayerNotThrowing() const;
	bool IsPlayerCurrentlyThrowing() const;

	void DisableAnyMovement();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* duckMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> boxComponent;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* projectileMovement;

	class APlayerCharacter* player = nullptr;

	TArray<class AEnemy*> enemies;

	UPROPERTY(EditAnywhere, Category = "Duck Projectile")
	float initialSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Duck Projectile")
	float maxSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Duck Projectile")
	float throwAngle = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Duck Projectile")
	float throwAngleOffset = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Duck Projectile")
	float fallOnFloorLocation = 10.26603f;

	bool insideTrigger;

	FVector targetLocation;

	float startingThrowingAngle;

	UMaterialInstanceDynamic* materialInst;

	bool isFalling;
	bool threwPastOutOfBoundsTrigger;

	UPROPERTY(EditAnywhere, Category = "Duck-Enemy Distance")
	float duckEnemyDistance = 200.0f;

	FVector startingLocation;
	FRotator startingRotation;
};
