// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ImpactSystem/SurfaceImpactTypes.h"
#include "SurfaceImpactSet.generated.h"

/**
 * Data asset that defines impact effects for a specific surface type
 * Allows to create one asset per surface type
 */
UCLASS()
class GDTV_HALLOWEEN2025_API USurfaceImpactSet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Surface")
	TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (TitleProperty = "ImpactType"))
	TMap<ESurfaceImpactType, FSurfaceImpactEffect> ImpactEffects;

public:
	/* Get the effect for a specific impact type */
	UFUNCTION(BlueprintCallable, Category = "Surface Impact")
	bool GetImpactEffect(ESurfaceImpactType ImpactType, FSurfaceImpactEffect& OutEffect) const;

	UFUNCTION(BlueprintCallable, Category = "Surface Impact")
	bool HasImpactEffect(ESurfaceImpactType ImpactType) const;
};
