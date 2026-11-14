// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Guard.generated.h"

class AAIController;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartDetection, AGuard*, guard_);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDetection, AGuard*, guard_, float, detectionValue_);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDetectedAnimation, AGuard*, guard_);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopDetection, AGuard*, guard_);

UENUM()
enum class EGuardState : uint8
{
	Patrolling,
	SpottedPlayer
};

UCLASS()
class SHROOMJAM2026_API AGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetPlayerCharacter(APlayerCharacter* playerCharacter_) { playerCharacter = playerCharacter_; }

	FOnStartDetection& GetOnStartDetection() { return OnStartDetection; }
	FOnDetection& GetOnDetection() { return OnDetection; }
	FOnStopDetection& GetOnStopDetection() { return OnStopDetection; }
	FOnDetectedAnimation& GetOnDetectedAnimation() { return OnDetectedAnimation; }

	void SetGuardState(EGuardState guardState_) { guardState = guardState_; }

	float GetDetectionValue() const { return detectionValue; }

	void MoveCameraToGuardThatSpottedPlayer();

private:
	void SpawnSpottedCamera();

	void PlayerDetected();

	UFUNCTION()
	void OnGuardHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere, Category = "Guard State")
	EGuardState guardState = EGuardState::Patrolling;

	UPROPERTY()
	TObjectPtr<AAIController> aiController = nullptr;

	UPROPERTY(EditAnywhere, Category = "Patrol Routes")
	TArray<FVector> patrolRoutes = TArray<FVector>();

	UPROPERTY(VisibleAnywhere, Category = "Patrol Routes")
	int currentPatrolIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "Detection Settings")
	float detectionValue = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Detection Settings")
	float detectionSpeed = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Detection Settings")
	float detectionForwardMultiplier = 500.0f;

	bool hasDetectedPlayer = false;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> playerCharacter = nullptr;

	FOnStartDetection OnStartDetection{};
	FOnDetection OnDetection{};
	FOnStopDetection OnStopDetection{};
	FOnDetectedAnimation OnDetectedAnimation{};

	UPROPERTY()
	TObjectPtr<class ACameraActor> cameraActor = nullptr;

	// Sound effect
	UPROPERTY(EditAnywhere, Category = "Sound Effects")
	TObjectPtr<USoundBase> playerSpottedSound = TObjectPtr<USoundBase>();

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ignoredActors = TArray<AActor*>();
};
