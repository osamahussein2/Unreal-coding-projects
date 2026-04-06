// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "MapSceneCapture2D.generated.h"

class UTextureRenderTarget2D;

UENUM()
enum class EMapMovement : uint8
{
	MoveLeft = 0,
	MoveRight = 1,
	MoveDown = 2,
	MoveUp = 3
};

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API AMapSceneCapture2D : public ASceneCapture2D
{
	GENERATED_BODY()
	
public:
	AMapSceneCapture2D(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void MoveMap(EMapMovement mapMovement, float& DeltaTime);

	void ZoomOutMap(float& DeltaTime);
	void ZoomInMap(float& DeltaTime);

private:
	UPROPERTY(EditAnywhere, Category = "Map Camera Parameters")
	float zoomSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Map Camera Parameters")
	float moveSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Map Camera Parameters")
	float minFullMapOrthoWidth = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Map Camera Parameters")
	float maxFullMapOrthoWidth = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Map Bounds")
	float minPositionY = -50.0f;

	UPROPERTY(EditAnywhere, Category = "Map Bounds")
	float maxPositionY = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Map Bounds")
	float minPositionZ = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Map Bounds")
	float maxPositionZ = 50.0f;
};
