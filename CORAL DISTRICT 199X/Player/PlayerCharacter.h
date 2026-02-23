// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Component/ActionComponent.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UPlayerInputDA;

UENUM()
enum class ELocomotionState : uint8
{
	EIdle UMETA (DisplayName = "Idle"),
	EMovingLeft UMETA (DisplayName = "Moving Left"),
	EMovingRight UMETA (DisplayName = "Moving Right"),
	EMovingForward UMETA (DisplayName = "Moving Forward"),
	EMovingBackward UMETA(DisplayName = "Moving Backward")
};

UENUM()
enum class EFootstepState : uint8
{
	EMovingOnLand UMETA(DisplayName = "Moving On Land"),
	EMovingOnWater UMETA(DisplayName = "Moving On Water")
};

UCLASS()
class BRACKEYSGAMEJAM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void ResetPlayerActions();
	void SetPlayerActionToPlayingRhythmMinigame();
	void SetPlayerActionToFinalScore();

	void ResetPlayerBackToWorld();

	AActor* GetInteractableFish() const { return interactableFish; }

	void InvalidateFishActor();

	void InteractWithFish();
	void SpawnBoxTriggerForFish();

	// Dance animations
	UFUNCTION()
	void PlayDanceAnimation();

	void PlayMissedNoteDanceAnimation();

	void PlayWinDanceAnimation();

	void StopAllAnimations();

	bool IsInFreeRoam() const { return actionComponent && actionComponent->HasAction(freeRoam); }

	void SetGameVolumes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnPlayerHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, 
		const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveCharacter(const struct FInputActionValue& value_);
	void PlayIdleAnimation();

	void PlayFootstepSounds();

	void MoveCamera(const FInputActionValue& value_);

	void PauseGame();

	void UpdateWalkingAnimations(FVector2D& direction);

	// Footstep sound functions
	void PlayDryFootstepSound();
	void PlayWaterFootstepSound();

private:
	// Player components
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> springArm = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> cameraComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UActionComponent> actionComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* inputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UPlayerInputDA* playerInputDA;

	// Check for interactable fish trigger to start interacting
	AActor* interactableFish = nullptr;

	// Actions
	const FString freeRoam = "Free Roam";
	const FString playingRhythmMinigame = "Playing Rhythm Minigame";
	const FString checkingOutFinalScore = "Checking Out Final Score";

	// Locomotion Animations
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* idleAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animations")
	TArray<UAnimSequence*> walkingAnimations;

	UPROPERTY(VisibleAnywhere, Category = "Locomotion")
	ELocomotionState locomotionState = ELocomotionState::EIdle;

	// Player last position and rotation
	FVector lastPosition = FVector::ZeroVector;
	FRotator lastActorRotation = FRotator::ZeroRotator;
	FRotator lastControlRotation = FRotator::ZeroRotator;

	// Dance Animations
	UPROPERTY(EditAnywhere, Category = "Animations")
	TArray<UAnimSequence*> danceAnimations;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* missNoteDanceAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* winDanceAnimation;

	// Mobile related variables
	FVector2D startTouchLocation = FVector2D::ZeroVector;

	// Footsteps
	UPROPERTY(EditAnywhere, Category = "Footsteps")
	FName floorTag = "Floor";

	UPROPERTY(EditAnywhere, Category = "Footsteps")
	FName waterTag = "Water";

	UPROPERTY(EditAnywhere, Category = "Footsteps")
	TArray<USoundBase*> footstepSounds; // 0 for dry footstep, 1 for water footstep

	UPROPERTY(EditAnywhere, Category = "Footsteps")
	float timeToPlayFootstepSoundAgain = 0.6f;

	UPROPERTY(VisibleAnywhere, Category = "Footsteps")
	EFootstepState footstepState = EFootstepState::EMovingOnLand;

	float moveTimer = 1.0f;

	float currentMusicVolume = 1.0f;
	float currentSFXVolume = 1.0f;

	FTimerHandle randomDanceHandle;
};
