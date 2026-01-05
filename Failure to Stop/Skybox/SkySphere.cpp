// Fill out your copyright notice in the Description page of Project Settings.

#include "Skybox/SkySphere.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASkySphere::ASkySphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Base = CreateDefaultSubobject<USceneComponent>(TEXT("Base"));
	Base->SetupAttachment(GetRootComponent());

	SkySphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkySphereMesh"));
	SkySphereMesh->SetupAttachment(Base);

	SkySphereMesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("/Engine/EngineSky/SM_SkySphere")));

	// Assign the material first
	SkySphereMesh->SetMaterial(0, LoadObject<UMaterialInterface>(this, TEXT("/Engine/EngineSky/M_Sky_Panning_Clouds2")));
}

// Called when the game starts or when spawned
void ASkySphere::BeginPlay()
{
	Super::BeginPlay();
	
	// Convert the material into a material instance
	if (!SkyMaterial)
	{
		SkyMaterial = UMaterialInstanceDynamic::Create(SkySphereMesh->GetMaterial(0), this);
		SkySphereMesh->SetMaterial(0, SkyMaterial); // Assign the dynamic material instance

		RefreshMaterial();
	}
}

void ASkySphere::RefreshMaterial()
{
	// If the directional light actor is valid
	if (DirectionalLightActor)
	{
		const FRotator& makeRotator = FRotator(DirectionalLightActor->GetActorRotation().Roll, 0.0f, 0.0f);

		SkyMaterial->SetVectorParameterValue(TEXT("Light direction"), makeRotator.Vector());
		SkyMaterial->SetVectorParameterValue(TEXT("Sun color"), DirectionalLightActor->GetLightComponent()->GetLightColor());

		static_cast<float&>(sunHeight) = UKismetMathLibrary::MapRangeUnclamped(DirectionalLightActor->GetActorRotation().Pitch,
			0.0f, -90.0f, 0.0f, 1.0f);

		if (ColorsDeterminedBySunPosition)
		{
			SkyMaterial->SetVectorParameterValue(TEXT("Horizon color"),
				HorizonColorCurve->GetClampedLinearColorValue(sunHeight));

			SkyMaterial->SetVectorParameterValue(TEXT("Zenith color"),
				ZenithColorCurve->GetClampedLinearColorValue(sunHeight));

			SkyMaterial->SetVectorParameterValue(TEXT("Cloud color"),
				CloudColorCurve->GetClampedLinearColorValue(sunHeight));

			SkyMaterial->SetScalarParameterValue(TEXT("Horizon falloff"),
				FMath::Lerp(3.0f, 7.0f, FMath::Abs(sunHeight)));

			SkyMaterial->SetScalarParameterValue(TEXT("Cloud speed"), CloudSpeed);
			SkyMaterial->SetScalarParameterValue(TEXT("Sun brightness"), SunBrightness);

			SkyMaterial->SetScalarParameterValue(TEXT("Sun height"), FMath::Abs(UKismetMathLibrary::SelectFloat(sunHeight,
				0.0f, sunHeight < 0.0f)));

			SkyMaterial->SetScalarParameterValue(TEXT("Cloud opacity"), CloudOpacity);
			SkyMaterial->SetScalarParameterValue(TEXT("Stars brightness"), StarsBrightness);
		}

		else
		{
			SkyMaterial->SetVectorParameterValue(TEXT("Horizon color"), HorizonColor);
			SkyMaterial->SetVectorParameterValue(TEXT("Zenith color"), ZenithColor);
			SkyMaterial->SetVectorParameterValue(TEXT("Overall color"), OverallColor);
			SkyMaterial->SetVectorParameterValue(TEXT("Cloud color"), CloudColor);

			SkyMaterial->SetScalarParameterValue(TEXT("Horizon falloff"), 3.0f);

			SkyMaterial->SetScalarParameterValue(TEXT("Cloud speed"), CloudSpeed);
			SkyMaterial->SetScalarParameterValue(TEXT("Sun brightness"), SunBrightness);

			SkyMaterial->SetScalarParameterValue(TEXT("Sun height"), FMath::Abs(UKismetMathLibrary::SelectFloat(sunHeight,
				0.0f, sunHeight < 0.0f)));

			SkyMaterial->SetScalarParameterValue(TEXT("Cloud opacity"), CloudOpacity);
			SkyMaterial->SetScalarParameterValue(TEXT("Stars brightness"), StarsBrightness);
		}
	}

	else // Otherwise the directional light actor is invalid
	{
		static_cast<float&>(sunHeight) = UKismetMathLibrary::MapRangeUnclamped(sunHeight, 0.0f, -90.0f, 0.0f, 1.0f);

		const FRotator& makeRotator = FRotator(0.0f, sunHeight, GetActorRotation().Yaw);

		SkyMaterial->SetVectorParameterValue(TEXT("Light direction"), makeRotator.Vector());

		const FLinearColor& orange = FLinearColor(1.0f, 0.221f, 0.04f, 1.0f);
		const FLinearColor& white = FLinearColor(0.954f, 0.901f, 0.744f, 1.0f);

		SkyMaterial->SetVectorParameterValue(TEXT("Sun color"),
			UKismetMathLibrary::LinearColorLerp(orange, white, FMath::Clamp(sunHeight + 0.2f, 0.0f, 1.0f)));

		if (ColorsDeterminedBySunPosition)
		{
			SkyMaterial->SetVectorParameterValue(TEXT("Horizon color"),
				HorizonColorCurve->GetClampedLinearColorValue(sunHeight));

			SkyMaterial->SetVectorParameterValue(TEXT("Zenith color"),
				ZenithColorCurve->GetClampedLinearColorValue(sunHeight));

			SkyMaterial->SetVectorParameterValue(TEXT("Cloud color"),
				CloudColorCurve->GetClampedLinearColorValue(sunHeight));

			SkyMaterial->SetScalarParameterValue(TEXT("Horizon falloff"),
				FMath::Lerp(3.0f, 7.0f, FMath::Abs(sunHeight)));

			SkyMaterial->SetScalarParameterValue(TEXT("Cloud speed"), CloudSpeed);
			SkyMaterial->SetScalarParameterValue(TEXT("Sun brightness"), SunBrightness);

			SkyMaterial->SetScalarParameterValue(TEXT("Sun height"), FMath::Abs(UKismetMathLibrary::SelectFloat(sunHeight,
				0.0f, sunHeight < 0.0f)));

			SkyMaterial->SetScalarParameterValue(TEXT("Cloud opacity"), CloudOpacity);
			SkyMaterial->SetScalarParameterValue(TEXT("Stars brightness"), StarsBrightness);
		}

		else
		{
			SkyMaterial->SetVectorParameterValue(TEXT("Horizon color"), HorizonColor);
			SkyMaterial->SetVectorParameterValue(TEXT("Zenith color"), ZenithColor);
			SkyMaterial->SetVectorParameterValue(TEXT("Overall color"), OverallColor);
			SkyMaterial->SetVectorParameterValue(TEXT("Cloud color"), CloudColor);

			SkyMaterial->SetScalarParameterValue(TEXT("Horizon falloff"), horizonFalloff);

			SkyMaterial->SetScalarParameterValue(TEXT("Cloud speed"), CloudSpeed);
			SkyMaterial->SetScalarParameterValue(TEXT("Sun brightness"), SunBrightness);

			SkyMaterial->SetScalarParameterValue(TEXT("Sun height"), FMath::Abs(UKismetMathLibrary::SelectFloat(sunHeight,
				0.0f, sunHeight < 0.0f)));

			SkyMaterial->SetScalarParameterValue(TEXT("Cloud opacity"), CloudOpacity);
			SkyMaterial->SetScalarParameterValue(TEXT("Stars brightness"), StarsBrightness);
		}
	}
}

