// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class EHGAMEJAM_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABullet();

	void SetDamage(float& damageAmount_);
	void SetBulletRange(float& rangeAmount_, float offsetValue_);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetBulletDirection(FVector direction_);

private:
	float GetActorMagnitude() const;

	void DestroyBullet();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* bulletMesh;

	UPROPERTY(EditAnywhere, Category = "Bullet Parameters")
	float bulletSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "Bullet Parameters")
	float bulletLifeTime = 3.0f;

	FVector bulletDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<class UBoxComponent> bulletCollision;

	float damage = 0.0f;
	float range = 0.0f;

	FVector startingPosition = FVector::ZeroVector;
};
