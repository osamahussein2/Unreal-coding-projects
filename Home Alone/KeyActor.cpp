// Fill out your copyright notice in the Description page of Project Settings.

#include "KeyActor.h"
#include "PlayerCharacter.h"

// Sets default values
AKeyActor::AKeyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the key mesh
	keyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	keyMesh->SetupAttachment(RootComponent);

	// Setup the key collider
	keyCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("KeyCollider"));
	keyCollider->SetupAttachment(keyMesh);

	// Setup the key light
	keyLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("KeyPointLight"));
	keyLight->SetupAttachment(keyMesh);

	cubeKey = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeKey"));
	cubeKey->SetupAttachment(keyMesh);

	keyAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("KeyAudio"));
	keyAudio->SetupAttachment(keyMesh);

	// Set the toy collider to use the signature of OnComponentBeginOverlap on the OnComponentBeginOverlap function
	keyCollider->OnComponentBeginOverlap.AddDynamic(this, &AKeyActor::OnComponentBeginOverlap);
	keyCollider->OnComponentEndOverlap.AddDynamic(this, &AKeyActor::OnComponentEndOverlap);
}

// Called when the game starts or when spawned
void AKeyActor::BeginPlay()
{
	Super::BeginPlay();

	if (cubeKey)
	{
		cubeKey->SetVisibility(false);
	}
	
	keyLight->SetIntensity(0.0f);
}

// Called every frame
void AKeyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Make sure when the toy isn't destroyed that the key audio never plays
	if (APlayerCharacter::toyDestroyed == false)
	{
		if (keyAudio->IsPlaying()) keyAudio->Stop();
	}

	// Otherwise, play the audio after setting the sound
	else if (APlayerCharacter::toyDestroyed == true)
	{
		if (keyAudio && IsValid(keyAudio) && !keyAudio->IsPlaying())
		{
			keyAudio->bAutoDestroy = true; // Make sure to destroy the sound after completion too

			keyAudio->SetSound(LoadObject<USoundBase>(nullptr, TEXT("/Game/Home_Alone/Sounds/Come_find_me")));
			keyAudio->Play();
		}
	}

	if (!APlayerCharacter::toyDestroyed) keyLight->SetIntensity(0.0f);
	else if (APlayerCharacter::toyDestroyed) keyLight->SetIntensity(currentKeyLightIntensity);

	if (canInteract && APlayerCharacter::hasInteracted)
	{
		APlayerCharacter::keyPickedUp = true;
		Destroy();
	}
}

void AKeyActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// If any key actor collides with the player character, enable interaction
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && APlayerCharacter::toyDestroyed &&
		!APlayerCharacter::keyPickedUp)
	{
		if (cubeKey) cubeKey->SetVisibility(true);
		canInteract = true;
	}
}

void AKeyActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (cubeKey) cubeKey->SetVisibility(false);
	canInteract = false;
}
