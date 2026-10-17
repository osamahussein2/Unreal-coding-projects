// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaypointActor.generated.h"

class USplineComponent;
class USplineMeshComponent;
class UStaticMesh;
class APlayerCharacter;

UCLASS()
class COMFYAUTUMNJAM_API AWaypointActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWaypointActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateWaypoint(APlayerCharacter* playerCharacter_);

	void InitializeWaypointLocations(APlayerCharacter* playerCharacter_, FVector goalLocation_);
	void InitializeWaypoint(APlayerCharacter* playerCharacter_);
	void RemoveWaypoint();

private:
	void GetPath(APlayerCharacter* playerCharacter_, FVector targetLocation_);
	void CreateWaypoint(TArray<FVector> path_);

private:
	UPROPERTY(VisibleAnywhere)
	FVector goalLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float zOffset = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USplineComponent* splineComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> path = TArray<FVector>();

	UPROPERTY(EditAnywhere)
	UStaticMesh* staticMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<USplineMeshComponent*> splineMeshes = TArray<USplineMeshComponent*>();

	UPROPERTY(EditAnywhere)
	FVector2D meshSplineScale = FVector2D(0.03f, 0.03f);
};
