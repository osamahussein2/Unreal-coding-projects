// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "RoomLight.h"
#include "RoomSpotlight.h"
#include "ToyActor.generated.h"

UCLASS()
class HOMEALONE_API AToyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToyActor();

	static int lightFlickerIterations;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when other actors collide hits the collider
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static TArray<ARoomLight*> roomLightInstances;
	static TArray<ARoomSpotlight*> roomSpotlightInstances;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* toyMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cubeKey;

	UPROPERTY(EditAnywhere)
	UBoxComponent* toyCollider;

	UPROPERTY(EditAnywhere)
	UAudioComponent* audioComponent;

	UPROPERTY(EditAnywhere)
	UAudioComponent* audioLightFlickerComponent;

	UPROPERTY(EditAnywhere)
	TArray<ARoomLight*> roomLights;

	UPROPERTY(EditAnywhere)
	TArray<ARoomSpotlight*> roomSpotlights;

	UPROPERTY(EditDefaultsOnly)
	UPointLightComponent* toyLight;

	UPROPERTY(EditDefaultsOnly)
	UPointLightComponent* cubeKeyLight;

	UPROPERTY(EditAnywhere)
	float maxToyLightIntensity = 75.0f;

	UPROPERTY(EditAnywhere)
	float toyLightIntensitySpeed = 20.0f;

	UPROPERTY(EditAnywhere)
	AActor* rocketRadio;

	float currentToyLightIntensity;

	bool playerFoundToy;
	bool canInteract;

	void ToyFound();

	UPROPERTY(EditAnywhere)
	int maxLightFlickerIterations = 5.0f;
};
