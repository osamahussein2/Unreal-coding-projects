// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightAnchorsSystem.generated.h"

class ALightAnchor;

USTRUCT(BlueprintType)
struct FLightAnchorData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Lights")
	FVector Position;
	UPROPERTY(EditAnywhere, Category = "Lights")
	float RadiusOuter;
	UPROPERTY(EditAnywhere, Category = "Lights")
	float RadiusInner;

	UPROPERTY(EditAnywhere, Category = "Lights")
	float LightRadius;
};


USTRUCT(BlueprintType)
struct FLightAnchorSection
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Lights")
    TArray<ALightAnchor*> Anchors;
};

UCLASS()
class GDTV_HALLOWEEN2025_API ALightAnchorsSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	ALightAnchorsSystem();

	FVector CaclulateLightPosition(FVector PlayerLocation, float& OutRadius);
	void ActivateAnchorsForSolvedPuzzle(int32 PuzzleId);
	void ResetLightAnchorsSections();
	void ActivateInitialAnchors();

protected:
	UPROPERTY(EditAnywhere, Category = "Lights")
	TMap<int32,FLightAnchorSection> LightAnchorsSections;

	UPROPERTY(VisibleAnywhere, Category = "Lights")
	TArray<FLightAnchorData> ActiveLightAnchors;

	UPROPERTY(VisibleAnywhere, Category = "Lights")
	TSet<int32> ActivatedIds;
};
