// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameWonTrigger.generated.h"

/**
 * 
 */
UCLASS()
class SHROOMJAM2025_API AGameWonTrigger : public AActor
{
	GENERATED_BODY()

public:
	AGameWonTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called when other actors collide hits the collider
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* trigger;

	class APlayerCharacter* player = nullptr;
};
