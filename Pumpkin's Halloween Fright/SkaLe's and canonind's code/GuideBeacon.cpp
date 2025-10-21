// Fill out your copyright notice in the Description page of Project Settings.


#include "GuideBeacon.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AGuideBeacon::AGuideBeacon()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	// Usually these are decorative: block if needed
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGuideBeacon::BeginPlay()
{
	Super::BeginPlay();
	MatID = Mesh ? Mesh->CreateDynamicMaterialInstance(MatElement) : nullptr;
	if (MatID && EmissiveOnOff != NAME_None)
	{
		MatID->SetScalarParameterValue(EmissiveOnOff, 0);
	}
}

void AGuideBeacon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!bBlinking || !MatID) return;
	
	TimeAcc += DeltaSeconds;
	const float T = bUsePhaseOffset ? (TimeAcc + PhaseOffset) : TimeAcc;
	float alpha = (FMath::Sin(TimeAcc * BlinkSpeed * 2.f * PI) + 1.f) * 0.5f;
	const float v = FMath::Lerp(EmissiveMin, EmissiveMax, alpha);
	MatID->SetScalarParameterValue(EmissiveOnOff, 1.f);
	MatID->SetScalarParameterValue(EmissivePhase, PhaseOffset);
}

void AGuideBeacon::SetPhaseOffset(float Seconds)
{
	PhaseOffset = FMath::Max(0.f, Seconds);
}

void AGuideBeacon::SetBlinking(bool bEnable)
{
	bBlinking = bEnable;
	TimeAcc = 0.f;
	if (MatID && EmissiveOnOff != NAME_None)
	{
		//MatID->SetScalarParameterValue(EmissiveOnOff, bBlinking ? EmissiveMax : EmissiveMin);
		MatID->SetScalarParameterValue(EmissiveOnOff, bBlinking ? 1.f : 0.f);
	}
}

void AGuideBeacon::SetBlinkSpeed(float Hz)
{
	// Store the new blink frequency (in Hz, cycles per second)
	BlinkSpeed = FMath::Max(0.01f, Hz);  // prevent divide-by-zero or negative

	// Update the material instance if it exists
	if (MatID)
	{
		MatID->SetScalarParameterValue(EmissiveSpeed, BlinkSpeed);
	}
}

