// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "NiagaraSystem.h"
#include "SurfaceImpactTypes.generated.h"

class UNiagaraSystem;

/**
 * Enum defining different types of surface impacts
 */
UENUM(BlueprintType)
enum class ESurfaceImpactType : uint8
{
	Footstep_Walk UMETA(DisplayName = "Footstep - Walk"),
	Footstep_Run UMETA(DisplayName = "Footstep - Run"),
	Footstep_Sprint UMETA(DisplayName = "Footstep - Sprint"),
	Jump UMETA(DisplayName = "Jump"),
	Land_Soft UMETA(DisplayName = "Land - Soft"),
	Land_Hard UMETA(DisplayName = "Land - Hard"),
	Impact_Light UMETA(DisplayName = "Impact - Light"),
	Impact_Medium UMETA(DisplayName = "Impact - Medium"),
	Impact_Heavy UMETA(DisplayName = "Impact - Heavy"),
	Custom_01 UMETA(DisplayName = "Custom 01"),
	Custom_02 UMETA(DisplayName = "Custom 02"),
	Custom_03 UMETA(DisplayName = "Custom 03")
};

/*
* Config for a single surface
*/

USTRUCT(BlueprintType)
struct FSurfaceImpactEffect
{
	GENERATED_BODY()

	FSurfaceImpactEffect()
		: Sound(nullptr)
		, NiagaraSystem(nullptr)
		, DecalMaterial(nullptr)
		, DecalSize(10.f, 10.f, 10.f)
		, DecalLifeTime(10.f)
	{
	}


	/* Sound of impact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSoftObjectPtr<USoundBase> Sound;

	/* Visual effect for impact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSoftObjectPtr<UNiagaraSystem> NiagaraSystem;

	/* Decal for impact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSoftObjectPtr<UMaterialInterface> DecalMaterial;

	/* Size of decal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (EditCondition = "DecalMaterial != nullptr"))
	FVector DecalSize = FVector(10.f, 10.f, 10.f);

	/* Decal lifetime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (EditCondition = "DecalMaterial != nullptr"))
	float DecalLifeTime = 10.f;


	bool IsValid() const 
	{
		return !Sound.IsNull() || !NiagaraSystem.IsNull() || !DecalMaterial.IsNull();
	}
};

/**
* Run-time parameters for surface impact
*/
USTRUCT(BlueprintType)
struct FSurfaceImpactParams
{
	GENERATED_BODY()

	/* Impact location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	FVector Location = FVector::ZeroVector;

	/* Impact normal */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	FVector Normal = FVector::UpVector;

	/* Surface PhysMat */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	TWeakObjectPtr<UPhysicalMaterial> PhysicalMaterial;

	/* Type of impact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	ESurfaceImpactType ImpactType = ESurfaceImpactType::Footstep_Walk;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|MOdifiers", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float VFXScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|MOdifiers", meta = (ClampMin = "0.0", ClampMax = "4.0"))
	float VolumeMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|MOdifiers", meta = (ClampMin = "0.5", ClampMax = "2.0"))
	float PitchMultiplier = 1.f;

	/* Should the sound follow the actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Behavior")
	bool bAttachSound = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Behavior")
	bool bDisableSound = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Behavior")
	bool bDisableVFX = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Behavior")
	bool bDisableDecal = false;

	/* Custom user data that can be passed to effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Advanced")
	int32 UserData = 0;

	FSurfaceImpactParams()
		: Location(FVector::ZeroVector)
		,Normal(FVector::UpVector)
		, PhysicalMaterial(nullptr)
		, ImpactType(ESurfaceImpactType::Footstep_Walk)
		, VFXScale(1.f)
		, VolumeMultiplier(1.f)
		, PitchMultiplier(1.f)
		, bAttachSound(false)
		, bDisableSound(false)
		, bDisableVFX(false)
		, bDisableDecal(false)
		, UserData(0)
	{ }
};