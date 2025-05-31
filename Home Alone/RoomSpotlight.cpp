// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomSpotlight.h"
#include "ToyActor.h"

bool ARoomSpotlight::isSpotLightFlickering = false;

// Sets default values
ARoomSpotlight::ARoomSpotlight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the room's spotlight
	roomSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("RoomSpotLight"));
	roomSpotLight->SetupAttachment(RootComponent);

	currentSpotLightIntensity = maxSpotLightIntensity;

	spotLightIntensityShouldIncrease = false;
}

// Called when the game starts or when spawned
void ARoomSpotlight::BeginPlay()
{
	Super::BeginPlay();

	currentSpotLightIntensity = maxSpotLightIntensity;

	isSpotLightFlickering = false;
	spotLightIntensityShouldIncrease = false;
}

// Called every frame
void ARoomSpotlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Make sure whenevr the light isn't flickering, set its current light intensity to max
	if (!isSpotLightFlickering)
	{
		currentSpotLightIntensity = maxSpotLightIntensity;

		// Set the light flicker iterations value to 0 just in case it isn't 0 already
		if (AToyActor::lightFlickerIterations != 0) AToyActor::lightFlickerIterations = 0;
	}

	// Otherwise, the light is flickering but SHOULDN'T increase in intensity just yet
	else if (isSpotLightFlickering && !spotLightIntensityShouldIncrease)
	{
		// Decrease the light intensity of the room light
		currentSpotLightIntensity -= spotLightIntensitySpeed * DeltaTime;
	}

	// Otherwise, the light is flickering and SHOULD increase in intensity
	else if (isSpotLightFlickering && spotLightIntensityShouldIncrease)
	{
		// Increase the light intensity of the room light
		currentSpotLightIntensity += spotLightIntensitySpeed * DeltaTime;
	}

	// Increase the light intensity once the light intensity reaches 0
	if (currentSpotLightIntensity <= 0.0f) spotLightIntensityShouldIncrease = true;

	// Decrease the light intensity once the light intensity reaches maximum intensity value
	else if (currentSpotLightIntensity >= maxSpotLightIntensity)
	{
		// If the light is flickering, increase the iterations value by 1 too
		if (isSpotLightFlickering == true) AToyActor::lightFlickerIterations += 1;

		spotLightIntensityShouldIncrease = false;
	}

	// Set the intensity of the room spotlight to its current light intensity
	roomSpotLight->SetIntensity(currentSpotLightIntensity);
}