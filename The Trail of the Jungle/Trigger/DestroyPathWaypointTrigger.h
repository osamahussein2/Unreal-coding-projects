// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyPathWaypointTrigger.generated.h"

class UBoxComponent;

UCLASS()
class COMFYAUTUMNJAM_API ADestroyPathWaypointTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestroyPathWaypointTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void DestroyPathWaypointTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> boxComponent = nullptr;

	// Path Trigger Settings
	UPROPERTY(EditAnywhere, Category = "Path Trigger Settings")
	int snapshotIndexForArea = 0;

	UPROPERTY(EditAnywhere, Category = "Path Trigger Settings")
	bool shouldLevelBeCompletedHere = false;

};
