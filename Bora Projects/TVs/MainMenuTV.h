// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "MainMenuTV.generated.h"

UCLASS()
class SHROOMJAM2025_API AMainMenuTV : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainMenuTV();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void ProcessInteraction(float DeltaTime);
	void PollMouseInput();
	bool TraceForUIMesh(FVector& OutHitLocation, FVector2D& OutUVCoordinates);
	FVector2D WorldToWidgetSpace(const FVector2D& UV);
	void UpdateWidgetInteractionTransform(const FVector2D& WidgetPosition);
	void SendPointerEvent(const FKey& Key, bool bIsPressed);

	void HandleMouseButton(const FKey& Key, bool& WasPressed);

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cubeMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* planeMesh;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* widgetComponent;

	UPROPERTY()
	UWidgetInteractionComponent* widgetInteraction;

	UMaterialInstanceDynamic* matInstanceDynamic;

	FVector2D lastWidgetMousePosition;
	FVector2D renderTargetSize;

	bool leftMousePressed = false;

	FVector currentHitLocation;

	UPROPERTY(EditAnywhere, Category = "UI Interaction", meta = (ClampMin = "100.0", ClampMax = "100000.0"))
	float maxInteractionDistance = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "UI Interaction")
	APlayerController* playerController;
};
