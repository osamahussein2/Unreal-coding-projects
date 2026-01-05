// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CineCamera/CutsceneCamera.h"
#include "CutscenePlayer.generated.h"

UCLASS()
class GOEDWARECUTSCENE2JAM_API ACutscenePlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACutscenePlayer();

	// Getter and setter for cutscene camera
	ACutsceneCamera* GetCutsceneCamera() const { return cutsceneCamera; }
	void SetCutsceneCamera();

	UFUNCTION()
	void InteractCutsceneCamera(const FInputActionValue& value_);

	UFUNCTION()
	void ReverseCutsceneCamera();

public:
	void ResetCutsceneCameraTransform(bool changeCameraFocus_ = false);
	void SetCreditsCameraTransform();
	void SetBlackAreaCameraTransform();

	void MoveToCreditsCamera(float& DeltaTime);
	void MoveToMainMenuCamera(float& DeltaTime);

	bool ReachedCreditsBillboard() const;
	bool ReachedMainMenu() const;

	UPROPERTY()
	bool isCutscenePlaying = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Cutscene camera
	UPROPERTY()
	ACutsceneCamera* cutsceneCamera;

	UPROPERTY(EditAnywhere, Category = "Main Menu Camera Settings")
	FVector cameraLocation = FVector(217.64547, -111.53298, 46.137199);

	UPROPERTY(EditAnywhere, Category = "Main Menu Camera Settings")
	FRotator cameraRotation = FRotator(-19.0, -26.0, 0.0);

	UPROPERTY(EditAnywhere, Category = "Credits Menu Camera Settings")
	FVector cameraCreditsLocation = FVector(902.0, 189.0, 203.0);

	UPROPERTY(EditAnywhere, Category = "Credits Menu Camera Settings")
	FRotator cameraCreditsRotation = FRotator(0.0, -180.0, 0.0);

	UPROPERTY(EditAnywhere, Category = "Game Camera Settings")
	FVector cameraBlackLocation = FVector(1002.0, 189.0, 203.0);

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float animateCameraMoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float animateCameraRotateSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	float minDistanceToDestination = 1.0f;

	// Input objects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* inputMapping;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	class UPlayerInputDA* playerDataAsset;

//public:
//	// Mobile touch input
//	void StartTouchInput(const FInputActionValue& Value);
//
//	// Mobile related functions
//	void MobileCutsceneCamera(const FInputActionValue& Value);
//
//private:
//	// Mobile related variables
//	FVector2D startTouchLocation = FVector2D::ZeroVector;
};
