// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorTrigger.h"
#include "PlayerCharacter.h"

// Sets default values
ADoorTrigger::ADoorTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the door collider
	doorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorTrigger"));
	doorTrigger->SetupAttachment(RootComponent);

	// Setup the cube key mesh
	cubeKey = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeKey"));
	cubeKey->SetupAttachment(doorTrigger);

	// Set the toy collider to use the signature of OnComponentBeginOverlap on the OnComponentBeginOverlap function
	doorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoorTrigger::OnComponentBeginOverlap);
	doorTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoorTrigger::OnComponentEndOverlap);

	canInteract = false;
	collidedForTheFirstTime = false;
	doorOpened = false;
}

// Called when the game starts or when spawned
void ADoorTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (cubeKey)
	{
		cubeKey->SetVisibility(false);
	}
	
	canInteract = false;
	collidedForTheFirstTime = false;
	doorOpened = false;
}

// Called every frame
void ADoorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Make sure the player has picked up the key, can interact with the door trigger, not collided with it for the first
	time yet, has pressed the interaction key but the door hasn't opened yet to be able to open the door */
	if (APlayerCharacter::keyPickedUp && canInteract && !collidedForTheFirstTime &&
		APlayerCharacter::hasPressedInteractionKey && !doorOpened)
	{
		collidedForTheFirstTime = true;
		doorOpened = true;
	}

	/* Make sure the player has picked up the key, can interact with the door trigger, collided with it for the first
	time and has pressed the interaction key so that the door can open */
	else if (APlayerCharacter::keyPickedUp && canInteract && collidedForTheFirstTime &&
		APlayerCharacter::hasPressedInteractionKey && doorOpened)
	{
		if (cubeKey && IsValid(cubeKey)) cubeKey->DestroyComponent();

		rotationValue += 1.0f; // Increment the rotation value

		// Destroy the key actor once the door's yaw rotation reaches 0 in game
		if (door->GetActorRotation().Yaw >= -1.5f)
		{
			APlayerCharacter::hasPressedInteractionKey = false;
			Destroy();
		}

		// Rotate the door by delta time
		FRotator rotation = FRotator(0.0f, rotationValue * DeltaTime, 0.0f);

		door->AddActorLocalRotation(rotation);
	}

	/* Make sure the player has picked up the key, cannot interact with the door trigger, collided with it for the first
	time and has pressed the interaction key so that the door can still open */
	else if (APlayerCharacter::keyPickedUp && !canInteract && collidedForTheFirstTime &&
		APlayerCharacter::hasPressedInteractionKey && doorOpened)
	{
		rotationValue += 1.0f; // Increment the rotation value

		// Destroy the key actor once the door's yaw rotation reaches 0 in game
		if (door->GetActorRotation().Yaw >= -1.5f)
		{
			APlayerCharacter::hasPressedInteractionKey = false;
			Destroy();
		}

		// Rotate the door by delta time
		FRotator rotation = FRotator(0.0f, rotationValue * DeltaTime, 0.0f);

		door->AddActorLocalRotation(rotation);
	}
}

void ADoorTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// If any key actor collides with the player character, enable interaction
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && APlayerCharacter::toyDestroyed &&
		APlayerCharacter::keyPickedUp && door)
	{
		// So that the door doesn't automatically open when the player collided with door trigger
		if (!collidedForTheFirstTime) APlayerCharacter::hasPressedInteractionKey = false;

		if (cubeKey && IsValid(cubeKey)) cubeKey->SetVisibility(true);
		canInteract = true;
	}
}

void ADoorTrigger::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (cubeKey && IsValid(cubeKey)) cubeKey->SetVisibility(false);
	
	// So that the door doesn't automatically open when the player re-entered the door trigger
	if (!doorOpened)
	{
		APlayerCharacter::hasPressedInteractionKey = false;
		collidedForTheFirstTime = false;
	}

	canInteract = false;
}