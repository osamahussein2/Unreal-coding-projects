// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Object/Weapon.h"
#include "Character/Enemy.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EHGAMEJAM_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Weapon functions
	void PickUpWeapon(ACharacter* playerCharacter_, FName tagName_, FName socketName_ = NAME_None);
	void AttachWeaponToSocket(ACharacter* playerCharacter_, FName tagName_, FName socketName_ = NAME_None);

	AWeapon* IsWeaponValid(const FName& tagName_) const;
	bool IsHoldingWeapon(FName tagName_) const;

	void RotateWeaponWhenHeld(ACharacter* playerCharacter_, FName tagName_, FName socketName_ = NAME_None);
	void RotateWeaponAtTarget(ACharacter* playerCharacter_, FName tagName_);

	void ResetWeaponRotation(ACharacter* playerCharacter_, const FName& tagName_);

	// Lock on/targeting functions
	void DetectEnemies(UCameraComponent* cameraComp, const FName& tagName_);
	void DetectEnemiesFromLeftSide(UCameraComponent* cameraComp, const FName& tagName_);
	void DetectEnemiesFromRightSide(UCameraComponent* cameraComp, const FName& tagName_);

	void StopDetectingEnemies();

	void DetectNextEnemy();

	bool CanContinueUponCheckingLineTraceCollision(AEnemy* enemy_, FVector location_);

	TObjectPtr<AEnemy> GetLockedOnEnemy() const { return lockedOnEnemy; }

	bool GetLockedOnTarget() const { return lockedOnTarget; }

	bool AreAllEnemiesKilled() const { return allEnemiesKilled; }

private:
	UPROPERTY(VisibleAnywhere)
	TArray<AWeapon*> weapons;

	UPROPERTY()
	TObjectPtr<AEnemy> lockedOnEnemy = nullptr;

	UPROPERTY()
	TObjectPtr<class ACharacter> characterRef = nullptr;

	bool lockedOnTarget = false;

	FCollisionQueryParams Params;

	UPROPERTY(EditAnywhere)
	FVector locationOffset = FVector(0.0f, 0.0f, 86.0f);

	UPROPERTY(EditAnywhere)
	float detectionValue = 0.9f;

	float globalNearestTarget = 0.0f;

	FVector cameraVector = FVector::ZeroVector;

	bool allEnemiesKilled = false;
};
