// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomLight.h"
#include "ToyActor.h"

bool ARoomLight::isLightFlickering = false;

// Sets default values
ARoomLight::ARoomLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the room's point light
	roomLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RoomPointLight"));
	roomLight->SetupAttachment(RootComponent);

	currentLightIntensity = maxLightIntensity;

	lightIntensityShouldIncrease = false;
}

// Called when the game starts or when spawned
void ARoomLight::BeginPlay()
{
	Super::BeginPlay();

	currentLightIntensity = maxLightIntensity;

	isLightFlickering = false;
	lightIntensityShouldIncrease = false;
}

// Called every frame
void ARoomLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Make sure whenevr the light isn't flickering, set its current light intensity to max
	if (!isLightFlickering)
	{
		currentLightIntensity = maxLightIntensity;

		// Set the light flicker iterations value to 0 just in case it isn't 0 already
		if (AToyActor::lightFlickerIterations != 0) AToyActor::lightFlickerIterations = 0;
	}

	// Otherwise, the light is flickering but SHOULDN'T increase in intensity just yet
	else if (isLightFlickering && !lightIntensityShouldIncrease)
	{
		// Decrease the light intensity of the room light
		currentLightIntensity -= lightIntensitySpeed * DeltaTime;
	}

	// Otherwise, the light is flickering and SHOULD increase in intensity
	else if (isLightFlickering && lightIntensityShouldIncrease)
	{
		// Increase the light intensity of the room light
		currentLightIntensity += lightIntensitySpeed * DeltaTime;
	}

	// Increase the light intensity once the light intensity reaches 0
	if (currentLightIntensity <= 0.0f) lightIntensityShouldIncrease = true;

	// Decrease the light intensity once the light intensity reaches maximum intensity value
	else if (currentLightIntensity >= maxLightIntensity)
	{
		// If the light is flickering, increase the iterations value by 1 too
		if (isLightFlickering == true) AToyActor::lightFlickerIterations += 1;

		lightIntensityShouldIncrease = false;
	}

	// Set the intensity of the room light to its current light intensity
	roomLight->SetIntensity(currentLightIntensity);
}

