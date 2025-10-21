// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightAnchor.generated.h"

class USphereComponent;

UCLASS()
class GDTV_HALLOWEEN2025_API ALightAnchor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALightAnchor();

	FVector GetAnchorPosition();
	float GetOuterRadius();
	float GetInnerRadius();
	float GetLightRadius();

protected:
	UPROPERTY(EditAnywhere, Category = "Properties")
	TObjectPtr<USphereComponent> LightSphereOuter;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TObjectPtr<USphereComponent> LightSphereInner;

	UPROPERTY(EditAnywhere, Category = "Properties")
	TObjectPtr<USphereComponent> LightRadius;

};
