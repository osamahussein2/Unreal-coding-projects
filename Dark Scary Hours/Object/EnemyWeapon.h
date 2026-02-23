// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyWeapon.generated.h"

class UBoxComponent;

UCLASS()
class EHGAMEJAM_API AEnemyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyWeapon();

	virtual void FireWeapon(FVector&& directionVector_);
	virtual void SpawnWeapon(FVector& directionVector_);

	virtual void ReloadWeapon();

	virtual float GetFiringSpeed() const { return fireSpeed; }
	float GetWeaponRange() const { return weaponRange; }
	float GetReloadSpeed() const { return 1.0f / reloadSpeed; }

	USceneComponent* GetSceneComponentDestination() const { return sceneComponentDestination; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	virtual float GetWeaponDamage() const { return weaponDamage; }

private:
	UPROPERTY(VisibleAnywhere, Category = "Ammo", meta = (ClampMin = "1.0"))
	int clipsInMagazine = 0;

	UPROPERTY(EditAnywhere, Category = "Ammo", meta = (ClampMin = "1.0"))
	int maxClips = 0;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* weaponMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* sceneComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* sceneComponentDestination = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* weaponSolidCollision = nullptr;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	TSubclassOf<class AEnemyBullet> bulletClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	float weaponDamage = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	FVector bulletSize = FVector(5.0f, 5.0f, 5.0f);

	UPROPERTY(EditAnywhere, Category = "Weapon Fire")
	float fireSpeed = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Aim Speed")
	float aimSpeed = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Weapon Range")
	float weaponRange = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Reload")
	float reloadSpeed = 2.0f;
};
