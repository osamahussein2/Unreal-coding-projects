// Fill out your copyright notice in the Description page of Project Settings.


#include "ToyActor.h"
#include "PlayerCharacter.h"

TArray<ARoomLight*> AToyActor::roomLightInstances = {};
TArray<ARoomSpotlight*> AToyActor::roomSpotlightInstances = {};

int AToyActor::lightFlickerIterations = 0;

// Sets default values
AToyActor::AToyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the toy mesh
	toyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToyMesh"));
	toyMesh->SetupAttachment(RootComponent);

	// Setup the toy collider
	toyCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ToyCollider"));
	toyCollider->SetupAttachment(toyMesh);

	// Setup the toy light
	toyLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("ToyPointLight"));
	toyLight->SetupAttachment(toyMesh);

	// Setup audio
	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComponent->SetupAttachment(toyMesh);

	audioLightFlickerComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioLightFlickerComponent"));
	audioLightFlickerComponent->SetupAttachment(toyMesh);

	// Setup the cube key mesh
	cubeKey = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeKey"));
	cubeKey->SetupAttachment(toyMesh);

	cubeKeyLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CubeKeyLight"));
	cubeKeyLight->SetupAttachment(cubeKey);

	// Set the toy collider to use the signature of OnComponentBeginOverlap on the OnComponentBeginOverlap function
	toyCollider->OnComponentBeginOverlap.AddDynamic(this, &AToyActor::OnComponentBeginOverlap);
	toyCollider->OnComponentEndOverlap.AddDynamic(this, &AToyActor::OnComponentEndOverlap);

	playerFoundToy = false;
	canInteract = false;

	currentToyLightIntensity = 0.0f;

	lightFlickerIterations = 0;
}

// Called when the game starts or when spawned
void AToyActor::BeginPlay()
{
	Super::BeginPlay();

	if (cubeKey)
	{
		cubeKey->SetVisibility(false);
	}

	// Set the static TArrays to the non-static TArrays that are important for checking light flickering
	roomLightInstances = roomLights;
	roomSpotlightInstances = roomSpotlights;

	currentToyLightIntensity = 0.0f;

	playerFoundToy = false;
	canInteract = false;

	lightFlickerIterations = 0;
}

// Called every frame
void AToyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!roomLightInstances.IsEmpty())
	{
		for (ARoomLight* roomLight : roomLightInstances)
		{
			if (!roomLight->isLightFlickering && audioLightFlickerComponent)
				audioLightFlickerComponent->Stop();

			if (!roomLight->isLightFlickering && audioComponent)
				audioComponent->Stop();
		}
	}

	if (!roomSpotlightInstances.IsEmpty())
	{
		for (ARoomSpotlight* roomSpotlight : roomSpotlightInstances)
		{
			if (!roomSpotlight->isSpotLightFlickering && audioLightFlickerComponent)
				audioLightFlickerComponent->Stop();

			if (!roomSpotlight->isSpotLightFlickering && audioComponent)
				audioLightFlickerComponent->Stop();
		}
	}

	ToyFound();

	if (toyLight && IsValid(toyLight) && playerFoundToy == false)
	{
		if (currentToyLightIntensity >= maxToyLightIntensity)
		{
			toyLightIntensitySpeed = -toyLightIntensitySpeed;
		}

		else if (currentToyLightIntensity <= 0.0f)
		{
			toyLightIntensitySpeed = -toyLightIntensitySpeed;
		}

		currentToyLightIntensity += toyLightIntensitySpeed * DeltaTime;

		toyLight->SetIntensity(currentToyLightIntensity);

		if (cubeKey && IsValid(cubeKey))
		{
			// Rotate the cube's key instruction to look at the flashlight mesh on the player
			cubeKey->SetRelativeRotation(FRotator(APlayerCharacter::flashlightMesh->GetComponentRotation()));
		}

		if (cubeKeyLight && IsValid(cubeKeyLight)) cubeKeyLight->SetIntensity(currentToyLightIntensity);
	}

	if (playerFoundToy == true)
	{
		if (toyLight) toyLight->DestroyComponent();

		if (IsValid(rocketRadio->FindComponentByClass(UAudioComponent::StaticClass())) && rocketRadio)
		{
			rocketRadio->FindComponentByClass(UAudioComponent::StaticClass())->DestroyComponent();
		}

		if (IsValid(audioComponent)) audioComponent->bAutoDestroy = true;
		if (IsValid(audioLightFlickerComponent)) audioLightFlickerComponent->bAutoDestroy = true;

		// Destroy this actor after the current delta time exceeds max time
		if (lightFlickerIterations >= maxLightFlickerIterations)
		{
			if (!roomLightInstances.IsEmpty())
			{
				for (ARoomLight* roomLight : roomLightInstances)
				{
					if (roomLight->isLightFlickering) roomLight->isLightFlickering = false;
				}
			}

			if (!roomSpotlightInstances.IsEmpty())
			{
				for (ARoomSpotlight* roomSpotlight : roomSpotlightInstances)
				{
					if (roomSpotlight->isSpotLightFlickering) roomSpotlight->isSpotLightFlickering = false;
				}
			}

			APlayerCharacter::toyDestroyed = true;
			Destroy();
		}
	}
}

void AToyActor::ToyFound()
{
	if (canInteract && APlayerCharacter::hasInteracted && !playerFoundToy)
	{
		playerFoundToy = true;
	}

	else if (canInteract && playerFoundToy)
	{
		if (cubeKey && IsValid(cubeKey)) cubeKey->DestroyComponent();

		// Only set the room point lights boolean if the array isn't empty
		if (!roomLightInstances.IsEmpty())
		{
			if (!audioComponent->IsPlaying() && IsValid(audioComponent)) audioComponent->Play();

			if (!audioLightFlickerComponent->IsPlaying() && IsValid(audioLightFlickerComponent))
				audioLightFlickerComponent->Play();

			for (ARoomLight* roomLight : roomLightInstances)
			{
				if (!roomLight->isLightFlickering) roomLight->isLightFlickering = true;
			}
		}

		// Only set the room spotlights boolean if the array isn't empty
		if (!roomSpotlightInstances.IsEmpty())
		{
			if (!audioComponent->IsPlaying() && IsValid(audioComponent)) audioComponent->Play();

			if (!audioLightFlickerComponent->IsPlaying() && IsValid(audioLightFlickerComponent))
				audioLightFlickerComponent->Play();

			for (ARoomSpotlight* roomSpotlight : roomSpotlightInstances)
			{
				if (!roomSpotlight->isSpotLightFlickering) roomSpotlight->isSpotLightFlickering = true;
			}
		}
	}

	else if (!canInteract && playerFoundToy)
	{
		if (cubeKey && IsValid(cubeKey)) cubeKey->DestroyComponent();

		// Only set the room point lights boolean if the array isn't empty
		if (!roomLightInstances.IsEmpty())
		{
			if (!audioComponent->IsPlaying() && IsValid(audioComponent)) audioComponent->Play();

			if (!audioLightFlickerComponent->IsPlaying() && IsValid(audioLightFlickerComponent))
				audioLightFlickerComponent->Play();

			for (ARoomLight* roomLight : roomLightInstances)
			{
				if (!roomLight->isLightFlickering) roomLight->isLightFlickering = true;
			}
		}

		// Only set the room spotlights boolean if the array isn't empty
		if (!roomSpotlightInstances.IsEmpty())
		{
			if (!audioComponent->IsPlaying() && IsValid(audioComponent)) audioComponent->Play();

			if (!audioLightFlickerComponent->IsPlaying() && IsValid(audioLightFlickerComponent))
				audioLightFlickerComponent->Play();

			for (ARoomSpotlight* roomSpotlight : roomSpotlightInstances)
			{
				if (!roomSpotlight->isSpotLightFlickering) roomSpotlight->isSpotLightFlickering = true;
			}
		}
	}
}

void AToyActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// If any toy actor collides with the player character, set the lights flickering booleans in the room
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		if (cubeKey && IsValid(cubeKey)) cubeKey->SetVisibility(true);
		canInteract = true;
	}
}

void AToyActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (cubeKey && IsValid(cubeKey)) cubeKey->SetVisibility(false);
	canInteract = false;
}
