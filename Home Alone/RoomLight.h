// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "RoomLight.generated.h"

UCLASS()
class HOMEALONE_API ARoomLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomLight();

	static bool isLightFlickering;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	UPointLightComponent* roomLight;

	UPROPERTY(EditAnywhere)
	float maxLightIntensity = 5000.0f;

	float currentLightIntensity;

	bool lightIntensityShouldIncrease;

	UPROPERTY(EditAnywhere)
	float lightIntensitySpeed = 10000.0f;
};
