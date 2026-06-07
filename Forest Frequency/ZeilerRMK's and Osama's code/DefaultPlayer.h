// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DefaultPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class UCableComponent;
class USpringArmComponent;
class UCameraComponent;
class APlayerCameraManager;

UENUM()
enum class ELocomotionState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	AboutToGrapple UMETA(DisplayName = "About to Grapple"),
	Grappling UMETA(DisplayName = "Grappling"),
	Climbing UMETA(DisplayName = "Climbing"),
	AboutToSwing UMETA(DisplayName = "About to Swing"),
	SwingingToPoint UMETA(DisplayName = "Swinging to Point"),
	SwingingFreely UMETA(DisplayName = "Swinging Freely"),
	SwingingInbounds UMETA(DisplayName = "Swinging Inbounds")
};

UCLASS()
class GAMEDEVTVGAMEJAM2026_API ADefaultPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void Move(const FInputActionValue& Value);
	void SwingAround();
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void Jump();
	void Pause();

private:
	void StopGrappling();

	UFUNCTION()
	void StopClimbing();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetPlayerCameraLocation() const;
	FVector GetPlayerCameraForwardVector() const;
	TObjectPtr<APlayerCameraManager> GetPlayerCameraManager() const;

	// Get grapplable/swing actor
	AActor* GetGrapplableOrSwingActor() const { return grapplableOrSwingActor; }
	void SetGrapplableOrSwingActor(AActor* actor_) { grapplableOrSwingActor = actor_; }

	UPROPERTY(EditAnywhere, Category = "Equipment")
	AActor* CurrentEquipment = nullptr;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	bool bIsHoldingCable = false;

protected:
	// Components
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UCableComponent> cableComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> hookMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> springArm = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> cameraComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UAudioComponent> audioComponent = nullptr;
		
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bIsSprinting = false;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpVelocity = 700.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 900.0f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 500.0f;

	// Current locomotion state for the player
	UPROPERTY(VisibleAnywhere, Category = "Locomotion State")
	ELocomotionState locomotionState = ELocomotionState::Normal;

	// About to grapple parameters
	UPROPERTY(VisibleAnywhere, Category = "About to Grapple Parameters")
	float grappleToPointTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "About to Grapple Parameters")
	float grappleToPointSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "About to Grapple Parameters")
	float distanceToGrappleToPoint = 0.5f;

	// Grapple parameters for the player
	UPROPERTY(VisibleAnywhere, Category = "Grapple Parameters")
	FVector grapplePoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Grapple Parameters")
	float grappleSpeed = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Grapple Parameters")
	float distanceToStopGrappling = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Climb Parameters")
	float timeToStopClimbing = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Climb Parameters")
	float climbSpeed = 10.0f;

	// Climbing timer handle
	FTimerHandle stopClimbingHandle{};

	// About to swing parameters
	UPROPERTY(VisibleAnywhere, Category = "About to Swing Parameters")
	float swingToPointTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "About to Swing Parameters")
	float swingToPointSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "About to Swing Parameters")
	float distanceToMoveToPoint = 0.5f;

	// Swinging to point parameters
	UPROPERTY(EditAnywhere, Category = "Swing to Point Parameters")
	float moveToPointSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Swing to Point Parameters")
	float distanceToStartSwinging = 10.0f;

	// Swinging parameters
	UPROPERTY(VisibleAnywhere, Category = "Swinging Parameters")
	FVector swingPoint = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = "Swinging Parameters")
	FVector initialCableEndPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Swinging Parameters")
	float upLaunchValue = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Swinging Parameters")
	float forwardLaunchValue = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Swinging Parameters", meta = (ClampMax = "0.0"))
	float valueBelowSwingingActor = -300.0f;

	UPROPERTY(EditAnywhere, Category = "Swinging Parameters", meta = (ClampMin = "0.0"))
	float swingingMultiplier = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Swinging Parameters", meta = (ClampMin = "0.0"))
	float maxSwingDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Swinging Inbounds Parameters", meta = (ClampMin = "0.0"))
	float swingInboundsSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Swinging Inbounds Parameters", meta = (ClampMin = "0.0"))
	float distanceThresholdToSwingFreelyAgain = 40.0f;

	UPROPERTY(VisibleAnywhere, Category = "Cable Component Location")
	FVector relativeCableComponentLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, Category = "Hook Mesh Location")
	FVector hookMeshRelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, Category = "Hook Mesh Location")
	FRotator hookMeshRelativeRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Hook Mesh Location")
	FRotator hookMeshRotation = FRotator(-90.0f, 0.0f, 0.0f);

	// Grapplable/swing actor
	UPROPERTY(VisibleAnywhere, Category = "Actor References")
	AActor* grapplableOrSwingActor = nullptr;

	//Interaction
	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance = 1300.0f;

	// Apply swinging movement automatically
	UPROPERTY(VisibleAnywhere, Category = "Swinging Around")
	float swingingX = 0.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Swinging Around")
	float swingingY = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Swinging Around")
	float minSwingValue = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Swinging Around")
	float distanceToStopSwinging = 5.0f;

	// Sound effects for the player
	UPROPERTY(EditAnywhere, Category = "SFX")
	TArray<TObjectPtr<USoundBase>> footstepSounds;

	UPROPERTY(EditAnywhere, Category = "SFX")
	TObjectPtr<USoundBase> grappleSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "SFX")
	TObjectPtr<USoundBase> jumpSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "SFX")
	TObjectPtr<USoundBase> swingSound = nullptr;
};
