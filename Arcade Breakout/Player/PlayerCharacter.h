// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UPlayerInputDA;
struct FInputActionValue;
class UCameraComponent;
class ACoin;
class AArcadeMachine;
class UAudioComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinCollected, int, coinsCollected_);

UENUM()
enum class EMagnetState : uint8
{
	WithPlayer,
	CollectCoin,
	ResetBackIntoPlace
};

UCLASS()
class SHROOMJAM2026_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	int GetCoinsAmount() const { return coinsAmount; }
	int GetTotalCoins() const { return totalCoins; }

	FOnCoinCollected& GetOnCoinCollected() { return OnCoinCollected; }

	UCameraComponent* GetCameraComponent() const { return cameraComponent; }

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void PauseGame();

	void UseMagnet();

	void FindNearestCoin();

private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> cameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> magnetComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UAudioComponent> footstepAudioComponent = nullptr;

	// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UPlayerInputDA> playerInputDA = nullptr;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float cameraSensitivity = 1.0f;

	// Coins
	UPROPERTY(VisibleAnywhere, Category = "Coins")
	int coinsAmount = 0;

	UPROPERTY(VisibleAnywhere, Category = "Coins")
	int totalCoins = 0;

	FOnCoinCollected OnCoinCollected{};

	// Magnet
	UPROPERTY(VisibleAnywhere, Category = "Magnet Settings")
	EMagnetState magnetState = EMagnetState::WithPlayer;

	UPROPERTY(EditAnywhere, Category = "Magnet Settings")
	float magnetMoveSpeed = 5.0f;

	ACoin* nearestCoin = nullptr;
	AArcadeMachine* arcadeMachine = nullptr;

	FVector magnetRelativeLocation = FVector::ZeroVector;
	FRotator magnetRelativeRotation = FRotator::ZeroRotator;

	// Sound effect
	UPROPERTY(EditAnywhere, Category = "Sound Effects")
	TArray<TObjectPtr<USoundBase>> footstepSounds = TArray<TObjectPtr<USoundBase>>();

	UPROPERTY(EditAnywhere, Category = "Sound Effects")
	TArray<TObjectPtr<USoundBase>> coinCollectedSounds = TArray<TObjectPtr<USoundBase>>();
};
