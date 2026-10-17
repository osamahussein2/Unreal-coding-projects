// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sign.generated.h"

class AWaypointActor;
class APath;

UENUM()
enum class ETakenPictureOfSignStatus : uint8
{
	ENotTakenPictureOfIt UMETA(DisplayName = "Not Taken Picture Of It"),
	ETookInitialPictureOfIt UMETA(DisplayName = "Took Initial Picture Of It"),
	ESignChanged UMETA(DisplayName = "Sign Changed"),
	ETookUpdatedPictureOfIt UMETA(DisplayName = "Took Updated Picture Of It")
};

UCLASS()
class COMFYAUTUMNJAM_API ASign : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASign();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetInitialPictureTaken();
	void SetSignChanged();
	void SetUpdatedPictureTaken();

	UStaticMeshComponent* GetStaticMeshComponent() const { return staticMeshComponent; }

	int GetSnapshotIndex() const { return snapshotIndex; }

	ETakenPictureOfSignStatus GetSignStatus() const { return takenPictureOfSignStatus; }

	TSubclassOf<AWaypointActor> GetWaypointClass() const { return waypointClass; }

	FVector GetInitialWaypointLocation() const { return initialWaypointLocation; }
	FVector GetUpdatedWaypointLocation() const { return updatedWaypointLocation; }

private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> staticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Snapshot Sign Parameters", meta = (Tooltip = "Set the index for the current snapshot for player to take picture of it"))
	int snapshotIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Snapshot Sign Parameters")
	FRotator signUpdatedRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleAnywhere, Category = "Snapshot Sign Parameters")
	ETakenPictureOfSignStatus takenPictureOfSignStatus = ETakenPictureOfSignStatus::ENotTakenPictureOfIt;

	// Waypoint Sign
	UPROPERTY(EditAnywhere, Category = "Waypoint Sign")
	TObjectPtr<APath> pathToOpen = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Waypoint Sign")
	TSubclassOf<AWaypointActor> waypointClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Waypoint Sign")
	FVector initialWaypointLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Waypoint Sign")
	FVector updatedWaypointLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Waypoint Sign")
	FVector newPhotoMarkerLocation = FVector::ZeroVector;
};
