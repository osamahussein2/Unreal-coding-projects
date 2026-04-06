// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CreditsAd.h"

// Sets default values
ACreditsAd::ACreditsAd()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	cubeMesh->SetupAttachment(RootComponent);

	cubeMesh->bHiddenInSceneCapture = true;
}

// Called when the game starts or when spawned
void ACreditsAd::BeginPlay()
{
	Super::BeginPlay();

	if (adIndex != 0) adIndex = 0;
	if (materialTimer != static_cast<float&>(startingMaterialTimer)) materialTimer = static_cast<float&>(startingMaterialTimer);
	if (texturesUpdated) texturesUpdated = false;

	materialDyn = UMaterialInstanceDynamic::Create(cubeMesh->GetMaterial(0), this);
	cubeMesh->SetMaterial(0, materialDyn); // Assign the dynamic material instance

	// Make sure the dynamic material is valid and there are at least 2 ad textures to set both textures
	if (materialDyn && adTextures.Num() >= 2)
	{
		materialDyn->SetTextureParameterValue(TEXT("Texture1"), adTextures[adIndex]);
		materialDyn->SetTextureParameterValue(TEXT("Texture2"), adTextures[adIndex + 1]);

		materialDyn->SetScalarParameterValue(TEXT("Time"), materialTimer); // Initialize material timer
	}
}

// Called every frame
void ACreditsAd::Tick(float DeltaTime)
{
	// Make sure the dynamic material is valid and there are at least 2 ad textures to update texture transition
	if (materialDyn && adTextures.Num() >= 2)
	{
		Super::Tick(DeltaTime);

		// Increment material timer to transition between 2 textures
		materialTimer += DeltaTime;
		materialDyn->SetScalarParameterValue(TEXT("Time"), materialTimer);

		// If the material timer fully shows the 2nd texture, update new textures
		if (materialTimer >= 1.0f && !texturesUpdated)
		{
			// If ad index hasn't reached the last index of ad textures yet, increment ad index
			if (adIndex < adTextures.Num() - 1) ++adIndex;

			// Otherwise the ad index reaches the final index, reset ad index to 0
			else adIndex = 0; // Reset index to 0

			materialTimer = static_cast<float&>(startingMaterialTimer); // Reset material timer to restart texture transition
			texturesUpdated = true; // Start updating new textures using the ad textures array
		}

		// If textures have been update
		else if (texturesUpdated)
		{
			// Update first texture to the new index
			materialDyn->SetTextureParameterValue(TEXT("Texture1"), adTextures[adIndex]);

			// Update second texture to the new index + 1 or set it to 0 (only after ad index reaches the final index)
			if (adIndex >= adTextures.Num() - 1) materialDyn->SetTextureParameterValue(TEXT("Texture2"), adTextures[0]);
			else materialDyn->SetTextureParameterValue(TEXT("Texture2"), adTextures[adIndex + 1]);

			texturesUpdated = false; // Don't update textures anymore
		}
	}
}

void ACreditsAd::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (materialDyn) materialDyn = nullptr;
}