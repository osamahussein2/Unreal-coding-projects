// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/StaticMesh.h"
#include "Components/SpotLightComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class HOMEALONE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	static bool toyDestroyed;
	static bool keyPickedUp;

	static bool hasInteracted;
	static bool hasPressedInteractionKey;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	static UStaticMeshComponent* flashlightMesh;

private:
	void MovePlayerForwardOrBack(float value_);
	void MovePlayerLeftOrRight(float value_);
	void MoveCameraHorizontally(float value_);
	void MoveCameraVertically(float value_);

	void ToggleFlashlight();
	void Interacted();
	void NotInteracting();

	void InteractWithoutHoldingKey();

	UPROPERTY(EditAnywhere)
	float playerSpeed = 3.0f;

	UPROPERTY(EditAnywhere)
	float cameraSpeed = 2.0f;

	UPROPERTY(EditDefaultsOnly)
	USpotLightComponent* flashlight;

	bool flashlightOn;

	UPROPERTY(EditAnywhere)
	float maxLightIntensity = 10000.0f;

	float currentLightIntensity;
};
