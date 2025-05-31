// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "ToyActor.h"

bool APlayerCharacter::toyDestroyed = false;
bool APlayerCharacter::keyPickedUp = false;
bool APlayerCharacter::hasInteracted = false;
bool APlayerCharacter::hasPressedInteractionKey = false;

UStaticMeshComponent* APlayerCharacter::flashlightMesh;

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup the flashlight mesh
	flashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMesh"));
	flashlightMesh->SetupAttachment(GetCapsuleComponent());

	// Setup the flashlight
	flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	flashlight->SetupAttachment(flashlightMesh);

	flashlightOn = true;

	currentLightIntensity = maxLightIntensity;

	toyDestroyed = false;
	keyPickedUp = false;
	hasInteracted = false;
	hasPressedInteractionKey = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (flashlightMesh)
	{
		flashlightMesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL,
			TEXT("/Game/Home_Alone/Meshes/RocketFlashlight/SM_RocketFlashlight_Final")));

		flashlightMesh->SetRelativeLocation(FVector(flashlightMesh->GetRelativeLocation().X,
			flashlightMesh->GetRelativeLocation().Y, flashlightMesh->GetRelativeLocation().Z));
	}

	if (flashlight)
	{
		// Move the flashlight just ahead of the flashlight mesh
		flashlight->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	// Just in case
	flashlightOn = true;

	currentLightIntensity = maxLightIntensity;

	toyDestroyed = false;
	keyPickedUp = false;
	hasInteracted = false;
	hasPressedInteractionKey = false;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the flashlight mesh depending on where the player camera is
	flashlightMesh->SetWorldRotation(FRotator(GetOwner()->GetActorRotation().Pitch,
		GetOwner()->GetActorRotation().Yaw, GetOwner()->GetActorRotation().Roll));

	// Make sure to check if the flashlight is valid and the room lights array isn't empty
	if (flashlight && !AToyActor::roomLightInstances.IsEmpty() &&
		!AToyActor::roomSpotlightInstances.IsEmpty())
	{
		for (int i = 0; i < AToyActor::roomLightInstances.Num(); i++)
		{
			for (int j = 0; j < AToyActor::roomSpotlightInstances.Num(); j++)
			{
				// If room lights aren't flickering, change the flashlight's intensity based on bool
				if (!AToyActor::roomLightInstances[i]->isLightFlickering &&
					!AToyActor::roomSpotlightInstances[j]->isSpotLightFlickering)
				{
					if (flashlightOn)
					{
						// Set the light intensity to whatever the current light intensity is
						flashlight->SetIntensity(currentLightIntensity);

						// Flip the flashlight y rotation to match the flashlight mesh's y rotation
						flashlight->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
					}

					else if (!flashlightOn)
					{
						flashlight->SetIntensity(0.0f);
					}
				}

				// If the room lights are flickering, set the flashlight intensity to 0 (flashlight cut off)
				else if (AToyActor::roomLightInstances[i]->isLightFlickering &&
					AToyActor::roomSpotlightInstances[j]->isSpotLightFlickering)
				{
					flashlight->SetIntensity(0.0f);

					if (!flashlightOn) flashlightOn = true;
				}
			}
		}
	}

	// Make sure to check if the flashlight is valid and the room point light array isn't empty
	else if (flashlight && !AToyActor::roomLightInstances.IsEmpty() &&
		AToyActor::roomSpotlightInstances.IsEmpty())
	{
		for (int i = 0; i < AToyActor::roomLightInstances.Num(); i++)
		{
			// If room lights aren't flickering, change the flashlight's intensity based on bool
			if (!AToyActor::roomLightInstances[i]->isLightFlickering)
			{
				if (flashlightOn)
				{
					// Set the light intensity to whatever the current light intensity is
					flashlight->SetIntensity(currentLightIntensity);

					// Flip the flashlight y rotation to match the flashlight mesh's y rotation
					flashlight->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
				}

				else if (!flashlightOn)
				{
					flashlight->SetIntensity(0.0f);
				}
			}

			// If the room lights are flickering, set the flashlight intensity to 0 (flashlight cut off)
			else if (AToyActor::roomLightInstances[i]->isLightFlickering)
			{
				flashlight->SetIntensity(0.0f);

				if (!flashlightOn) flashlightOn = true;
			}
		}
	}

	// Make sure to check if the flashlight is valid and the room spotlight array isn't empty
	else if (flashlight && AToyActor::roomLightInstances.IsEmpty() &&
		!AToyActor::roomSpotlightInstances.IsEmpty())
	{
		for (int j = 0; j < AToyActor::roomSpotlightInstances.Num(); j++)
		{
			// If room lights aren't flickering, change the flashlight's intensity based on bool
			if (!AToyActor::roomSpotlightInstances[j]->isSpotLightFlickering)
			{
				if (flashlightOn)
				{
					// Set the light intensity to whatever the current light intensity is
					flashlight->SetIntensity(currentLightIntensity);

					// Flip the flashlight y rotation to match the flashlight mesh's y rotation
					flashlight->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
				}

				else if (!flashlightOn)
				{
					flashlight->SetIntensity(0.0f);
				}
			}

			// If the room lights are flickering, set the flashlight intensity to 0 (flashlight cut off)
			else if (AToyActor::roomSpotlightInstances[j]->isSpotLightFlickering)
			{
				flashlight->SetIntensity(0.0f);

				if (!flashlightOn) flashlightOn = true;
			}
		}
	}

	// Make sure to check if the flashlight is valid and the room lights are all empty as well
	else if (flashlight && AToyActor::roomLightInstances.IsEmpty() &&
		AToyActor::roomSpotlightInstances.IsEmpty())
	{
		if (flashlightOn)
		{
			// Set the light intensity to whatever the current light intensity is
			flashlight->SetIntensity(currentLightIntensity);

			// Flip the flashlight y rotation to match the flashlight mesh's y rotation
			flashlight->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		}

		else if (!flashlightOn)
		{
			flashlight->SetIntensity(0.0f);
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind the axis mappings found inside the engine->input project settings to the functions below
	PlayerInputComponent->BindAxis(TEXT("MoveForwardOrBack"), this, &APlayerCharacter::MovePlayerForwardOrBack);
	PlayerInputComponent->BindAxis(TEXT("MoveHorizontally"), this, &APlayerCharacter::MovePlayerLeftOrRight);

	// Camera movement
	PlayerInputComponent->BindAxis(TEXT("MoveCameraHorizontally"), this, &APlayerCharacter::MoveCameraHorizontally);
	PlayerInputComponent->BindAxis(TEXT("MoveCameraVertically"), this, &APlayerCharacter::MoveCameraVertically);

	// Toggle flashlight
	PlayerInputComponent->BindAction(TEXT("ToggleFlashlight"), IE_Pressed, this, &APlayerCharacter::ToggleFlashlight);

	// Interact
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &APlayerCharacter::Interacted);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Released, this, &APlayerCharacter::NotInteracting);

	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &APlayerCharacter::InteractWithoutHoldingKey);
}

void APlayerCharacter::MovePlayerForwardOrBack(float value_)
{
	AddMovementInput(GetActorForwardVector(), value_ * playerSpeed);
}

void APlayerCharacter::MovePlayerLeftOrRight(float value_)
{
	AddMovementInput(GetActorRightVector(), value_ * playerSpeed);
}

void APlayerCharacter::MoveCameraHorizontally(float value_)
{
	AddControllerYawInput(value_ * cameraSpeed);
}

void APlayerCharacter::MoveCameraVertically(float value_)
{
	AddControllerPitchInput(-value_ * cameraSpeed);
}

void APlayerCharacter::ToggleFlashlight()
{
	flashlightOn = !flashlightOn;
}

void APlayerCharacter::Interacted()
{
	hasInteracted = true;
}

void APlayerCharacter::NotInteracting()
{
	hasInteracted = false;
}

void APlayerCharacter::InteractWithoutHoldingKey()
{
	hasPressedInteractionKey = true;
}