// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthPotion.generated.h"

class APlayerCharacter;
class UBoxComponent;
//class UProjectileMovementComponent;

UCLASS()
class DUNGEONCRAWLER2026_API AHealthPotion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void OnHealthPotionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> potionMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> potionCollider = nullptr;

	/*UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> potionProjectile = nullptr;*/

	UPROPERTY(EditAnywhere, Category = "Health")
	float healthToIncrementBy = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float potionMoveSpeed = 10.0f;

	/*UPROPERTY(EditAnywhere, Category = "Projectile", meta = (ClampMin = "0.0", ClampMax = "90.0"))
	float throwAngle = 20.0f;*/

	//FRotator initialRotation = FRotator::ZeroRotator;
};
