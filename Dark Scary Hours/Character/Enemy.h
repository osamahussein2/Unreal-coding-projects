// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AEnemyWeapon;
class APlayerCharacter;
//class USpringArmComponent;
class USceneCaptureComponent2D;
class UPaperSpriteComponent;

UCLASS()
class EHGAMEJAM_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	void SpawnCrosshair();
	void DestroyCrosshair();

	bool GetLockedOn() const { return lockedOn; }

private:
	UFUNCTION()
	void UpdateSocketLocation();

private:
	UPROPERTY(EditAnywhere)
	float maxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere)
	float currentHealth = 0.0f;

	UPROPERTY()
	TObjectPtr<class UWidgetComponent> crosshairWidgetComponent;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> crosshairWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widget")
	FVector widgetComponentSpawnLocation = FVector(0.0f, 0.0f, 120.0f);

	UPROPERTY(EditAnywhere, Category = "Widget")
	FVector2D widgetComponentDrawSize = FVector2D(100.0f, 100.0f);

	bool lockedOn = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AEnemyWeapon> enemyWeaponClass;

	UPROPERTY()
	AEnemyWeapon* enemyWeapon = nullptr;

	UPROPERTY()
	APlayerCharacter* player = nullptr;

	float fireTimer = 0.0f;

	// Minimap for the enemy icon
	/*UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> minimapSpringArm;*/

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneCaptureComponent2D> sceneCaptureComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperSpriteComponent> spriteComponent;
};
