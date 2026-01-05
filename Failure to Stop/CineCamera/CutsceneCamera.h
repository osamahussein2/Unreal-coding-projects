// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "CutsceneCamera.generated.h"

/**
 * 
 */
UCLASS()
class GOEDWARECUTSCENE2JAM_API ACutsceneCamera : public ACineCameraActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACutsceneCamera(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void RotateCutsceneCamera(const FVector2D& value_);

	UFUNCTION()
	void ReverseCutsceneCameraRotation();

	void ResetCutsceneCameraParameters();

	void SetConstrainAspectRatio(bool value_);

private:
	UPROPERTY()
	float rotationX;

	UPROPERTY()
	float rotationY;

	UPROPERTY(EditAnywhere, Category = "Camera Rotation")
	float rotationSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Rotation")
	float maxRotationX = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Rotation")
	float maxRotationY = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "Camera Rotation")
	bool hasCutsceneCameraFinishedRotating;

	UPROPERTY()
	float deltaTime;

	UPROPERTY()
	class AGoedWareCutsceneGM* cutsceneGM;

	//TWeakObjectPtr<class ACutscenePlayer> cutscenePlayer = nullptr;

	//float timer = 0.0f;
};
