// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ImpactSystem/SurfaceImpactTypes.h"
#include "SurfaceImpactInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USurfaceImpactInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors to customize surface impact behavior
 */
class GDTV_HALLOWEEN2025_API ISurfaceImpactInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Surface Impact")
	bool OnPreSurfaceImpact(UPARAM(ref) FSurfaceImpactParams& Params);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Surface Impact")
	void OnPostSurfaceImpact(const FSurfaceImpactParams& Params);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Surface Impact")
	bool GetCustomImpactSocket(ESurfaceImpactType ImpactType, FName& OutSocketName);
};
