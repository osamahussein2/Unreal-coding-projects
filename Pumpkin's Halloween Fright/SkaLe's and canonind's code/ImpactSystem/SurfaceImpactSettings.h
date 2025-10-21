// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ImpactSystem/SurfaceImpactSubsystem.h"
#include "SurfaceImpactSettings.generated.h"

/**
 * Editor-configurable settings for the Surface Impact System.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Surface Impact System"))
class GDTV_HALLOWEEN2025_API USurfaceImpactSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Map of Physical Surfaces to Surface Impact Sets */
    UPROPERTY(EditAnywhere, Config, Category = "Surface Impact")
    TMap<TEnumAsByte<EPhysicalSurface>, TSoftObjectPtr<USurfaceImpactSet>> SurfaceImpactSets;

    /** Default impact set to use when no specific set is found */
    UPROPERTY(EditAnywhere, Config, Category = "Surface Impact")
    TSoftObjectPtr<USurfaceImpactSet> DefaultImpactSet;

    /** Global volume multiplier */
    UPROPERTY(EditAnywhere, Config, Category = "Surface Impact", meta=(ClampMin="0.0", ClampMax="2.0"))
    float MasterVolume = 1.0f;

    /** Global VFX scale */
    UPROPERTY(EditAnywhere, Config, Category = "Surface Impact", meta=(ClampMin="0.1", ClampMax="5.0"))
    float MasterVFXScale = 1.0f;

    /** Enable debug visualization in editor */
    UPROPERTY(EditAnywhere, Config, Category = "Debug")
    bool bDebugDraw = false;

    /** Duration of debug draw */
    UPROPERTY(EditAnywhere, Config, Category = "Debug", meta=(EditCondition="bDebugDraw"))
    float DebugDrawDuration = 2.0f;
};
