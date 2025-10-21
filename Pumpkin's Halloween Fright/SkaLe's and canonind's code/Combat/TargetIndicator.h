// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetIndicator.generated.h"


UCLASS()
class GDTV_HALLOWEEN2025_API ATargetIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	ATargetIndicator();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> IndicatorMesh;
};
