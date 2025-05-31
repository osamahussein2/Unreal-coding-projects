// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SpotLightComponent.h"
#include "RoomSpotlight.generated.h"

UCLASS()
class HOMEALONE_API ARoomSpotlight : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoomSpotlight();

	static bool isSpotLightFlickering;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly) 
	USpotLightComponent* roomSpotLight;
  
	UPROPERTY(EditAnywhere)
	float maxSpotLightIntensity = 5000.0f;

	float currentSpotLightIntensity;

	bool spotLightIntensityShouldIncrease;

	UPROPERTY(EditAnywhere)
	float spotLightIntensitySpeed = 10000.0f;
};