// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThreeLaneSign.generated.h"

class AWaypointActor;
class APath;

UENUM()
enum class EThreeLaneSignStatus : uint8
{
	ENotTakenPictureOfIt UMETA(DisplayName = "Not Taken Picture Of It"),
	ETakenFirstPicture UMETA(DisplayName = "Taken First Picture"),
	ESignChangedOnce UMETA(DisplayName = "Sign Changed Once"),
	ETakenSecondPicture UMETA(DisplayName = "Taken Second Picture"),
	ESignChangedTwice UMETA(DisplayName = "Sign Changed Twice"),
	ETakenThirdPicture UMETA(DisplayName = "Taken Third Picture")
};

UCLASS()
class COMFYAUTUMNJAM_API AThreeLaneSign : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThreeLaneSign();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTakenFirstPicture();
	void SetSignChangedOnce();
	void SetTakenSecondPicture();
	void SetSignChangedTwice();
	void SetTakenThirdPicture();

	UStaticMeshComponent* GetStaticMeshComponent() const { return staticMeshComponent; }

	int GetSnapshotIndex() const { return snapshotIndex; }

	EThreeLaneSignStatus GetSignStatus() const { return takenPictureOfSignStatus; }

	TSubclassOf<AWaypointActor> GetWaypointClass() const { return waypointClass; }

	FVector GetInitialWaypointLocation() const { return initialWaypointLocation; }
	FVector GetSecondWaypointLocation() const { return secondWaypointLocation; }
	FVector GetThirdWaypointLocation() const { return finalWaypointLocation; }

private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> staticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Snapshot Sign Parameters", meta = (Tooltip = "Set the index for the current snapshot for player to take picture of it"))
	int snapshotIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Snapshot Sign Parameters")
	FRotator signUpdatedFirstRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Snapshot Sign Parameters")
	FRotator signUpdatedSecondRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleAnywhere, Category = "Snapshot Sign Parameters")
	EThreeLaneSignStatus takenPictureOfSignStatus = EThreeLaneSignStatus::ENotTakenPictureOfIt;

	// Waypoint Sign
	UPROPERTY(EditAnywhere, Category = "Waypoint Sign")
	TObjectPtr<APath> pathToOpen = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Waypoint Sign")
	TSubclassOf<AWaypointActor> waypointClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Waypoint Sign")
	FVector initialWaypointLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Waypoint Sign")
	FVector secondWaypointLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Waypoint Sign")
	FVector finalWaypointLocation = FVector::ZeroVector;
};
