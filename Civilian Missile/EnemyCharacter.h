// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PauseMenuActor.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class KENNEYJAM2025_API AEnemyCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(EditAnywhere)
	APauseMenuActor* pauseMenuActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when other actors collide hits the collider
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static int maxEnemyHealth;
	static int currentEnemyHealth;

	static int maxEnemyPower;
	static int currentEnemyPower;

private:
	UPROPERTY(EditAnywhere)
	UObject* playerFish;

	UPROPERTY(EditAnywhere)
	UObject* playerLaser;

	UPROPERTY(EditAnywhere)
	UObject* playerRocket;

	UPROPERTY(EditAnywhere)
	UObject* playerSmoke;
};
