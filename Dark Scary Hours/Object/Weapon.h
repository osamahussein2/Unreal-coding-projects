// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class EHGAMEJAM_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeapon();

	int GetClipsInMagazine() const { return clipsInMagazine; }
	int GetTotalAmmo() const { return totalAmmo; }

	float GetAimSpeed() const { return aimSpeed; }

	UFUNCTION()
	USceneComponent* GetSceneComponent() const { return sceneComponent; }

	FVector GetDestinationLocation() const;

	float GetWeaponRange() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when other actors collide hits the collider
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void FireWeapon(FVector&& directionVector_);
	virtual void SpawnWeapon(FVector& directionVector_);

	virtual void ReloadWeapon();

	virtual void RefillAmmo();

	virtual bool GetPlayerIsOnWeapon() const;

	virtual void DisableCollisions();
	virtual void EnableCollisions();

	virtual float GetFiringSpeed() const { return fireSpeed; }
	virtual float GetWeaponDamage() const { return weaponDamage; }

	virtual void ShowGlowingMesh();
	virtual void HideGlowingMesh();

private:
	UPROPERTY(VisibleAnywhere, Category = "Ammo", meta = (ClampMin = "1.0"))
	int clipsInMagazine = 0;

	UPROPERTY(EditAnywhere, Category = "Ammo", meta = (ClampMin = "1.0"))
	int maxClips = 0;

	UPROPERTY(VisibleAnywhere, Category = "Ammo")
	int totalAmmo = 0;

	UPROPERTY(EditAnywhere, Category = "Ammo")
	int maxAmmo = 0;

	UPROPERTY(VisibleAnywhere, Category = "Ammo")
	int lastRecordedAmmo = 0;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* weaponMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* sceneComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* sceneComponentDestination = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* glowingMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* boxComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* weaponSolidCollision = nullptr;

	bool playerOnWeapon = false;

	UPROPERTY(EditAnywhere, Category = "Bullet")
	TSubclassOf<class ABullet> bulletClass = nullptr;

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
};
