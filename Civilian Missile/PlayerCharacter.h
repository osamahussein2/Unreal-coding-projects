// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class KENNEYJAM2025_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "GamePause")
	static bool IsGamePaused();
	static void SetGamePaused(bool gamePaused_);

	static int maxHealth;
	static int currentHealth;

	static int maxPower;
	static int currentPower;

	static float currentTime;

	static bool inMenu;

private:
	void PauseGame();

	static bool gamePaused;

	UPROPERTY(EditAnywhere)
	float maxTime;

	UPROPERTY(EditAnywhere)
	UObject* enemyFish;

	UPROPERTY(EditAnywhere)
	UObject* enemyLaser;

	UPROPERTY(EditAnywhere)
	UObject* enemyRocket;

	UPROPERTY(EditAnywhere)
	UObject* enemySmoke;
};
