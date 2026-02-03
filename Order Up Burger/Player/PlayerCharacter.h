// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCustomerInteractionComponent;
class UDropFoodComponent;
class UCookingComponent;

UENUM()
enum class EPlayerState : uint8
{
	EFreeRoam UMETA(DisplayName = "Free Roam"),
	ECooking UMETA(DisplayName = "Cooking"),
	EDroppingFood UMETA(DisplayName = "Dropping Food"),
	ECarryingFood UMETA(DisplayName = "Carrying Food")
};

UCLASS()
class PORTFOLIOBUILDERS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void SetPlayerStateToCooking();
	void SetPlayerStateToCarryingFood();

	void AllowPlayerToFreeRoamAgain();

	void StartDroppingBurger();
	void StartDroppingTopping(FString& toppingName_);
	void StartDroppingTopBun();

	EPlayerState GetEPlayerState() const { return playerState; }

	bool GetReceivedOrderFromCustomer() const { return receivedOrderFromCustomer; }

	int GetCustomerNumber() const { return customerNumber; }

	int GetFoodsDropped() const { return foodsDropped; }
	void IncrementFoodsDropped();

	FORCEINLINE float GetFoodAttachOffsetZ() const { return foodAttachOffsetZ; }

	FORCEINLINE bool GetToggleOverlayVisiblity() const { return toggleOverlayVisibility; }

	void ShowOverlayOrder();

	void ResetValues();

	// Spawning/destroying component functions
	void SpawnCustomerInteractionComponent();
	void DestroyCustomerInteractionComponent();

	void SpawnDropFoodComponent();
	void DestroyDropFoodComponent();

	void SpawnCookingComponent();
	void DestroyCookingComponent();

	void DestroyAnyComponentReferences();

	// Spawning/destroying component getters
	TObjectPtr<UCustomerInteractionComponent> GetCustomerInteraction() const { return customerInteraction; }
	TObjectPtr<UCookingComponent> GetCooking() const { return cooking; }

	void Interact();
	void StartDroppingFood();
	void ToggleCustomerOrderOverlay();

	// Interaction related functions
	void StartOrEndCooking();
	void TakeOrderFromCustomer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private: // Private functions
	void MoveCharacter(const struct FInputActionValue& value_);
	void SpinCamera(const struct FInputActionValue& value_);
	void UpdateCameraZoom(const struct FInputActionValue& value_);

	void PauseGame();

private: // Private variables
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> springArm = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCameraComponent> cameraComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* inputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UPlayerInputDA* playerInputDA;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float spinSensitivity = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float zoomSensitivity = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float minArmTargetLength = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float maxArmTargetLength = 1000.0f;

	EPlayerState playerState = EPlayerState::EFreeRoam;

	UPROPERTY(EditAnywhere, Category = "Cine Camera Settings")
	FVector cameraSpawnLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Cine Camera Settings")
	FRotator cameraSpawnRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Cine Camera Settings")
	FVector playerCookingLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Food Settings")
	float foodAttachOffsetZ = 0.0f;

	bool receivedOrderFromCustomer = false;

	int customerNumber = 0;
	int foodsDropped = 0;

	bool toggleOverlayVisibility = false;

	// Spawn components on this player
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCustomerInteractionComponent> customerInteraction = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDropFoodComponent> dropFood = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCookingComponent> cooking = nullptr;

	FVector startingLocation = FVector::ZeroVector;
	FRotator startingRotation = FRotator::ZeroRotator;

public:
	// Mobile touch input
	void StartTouchInput(const FInputActionValue& Value);

private:
	// Mobile related variables
	float startTouchLocation = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Mobile")
	bool isForMobile = false;
};
