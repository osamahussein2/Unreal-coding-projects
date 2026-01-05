// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveLinearColor.h"
#include "SkySphere.generated.h"

class ADirectionalLight;

UCLASS()
class GOEDWARECUTSCENE2JAM_API ASkySphere : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASkySphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void RefreshMaterial();

private:
	// Components
	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* Base;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* SkySphereMesh;

	// Sky parameters
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* SkyMaterial = nullptr;

	UPROPERTY(EditAnywhere)
	bool refreshMaterial = false;

	UPROPERTY(EditAnywhere)
	ADirectionalLight* DirectionalLightActor = nullptr;

	UPROPERTY(EditAnywhere)
	bool ColorsDeterminedBySunPosition = true;

	UPROPERTY(EditAnywhere)
	float SunBrightness = 50.0f;

	UPROPERTY(EditAnywhere)
	float CloudSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	float CloudOpacity = 0.7f;

	UPROPERTY(EditAnywhere)
	float StarsBrightness = 0.1f;

	UPROPERTY(EditAnywhere)
	UCurveLinearColor* HorizonColorCurve = LoadObject<UCurveLinearColor>(this, TEXT("/Engine/EngineSky/C_Sky_Horizon_Color"));

	UPROPERTY(EditAnywhere)
	UCurveLinearColor* ZenithColorCurve = LoadObject<UCurveLinearColor>(this, TEXT("/Engine/EngineSky/C_Sky_Zenith_Color"));

	UPROPERTY(EditAnywhere)
	UCurveLinearColor* CloudColorCurve = LoadObject<UCurveLinearColor>(this, TEXT("/Engine/EngineSky/C_Sky_Cloud_Color"));

	UPROPERTY(EditAnywhere, Category = "Override Settings", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float sunHeight = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Override Settings")
	float horizonFalloff = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Override Settings")
	FLinearColor HorizonColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Override Settings")
	FLinearColor ZenithColor = FLinearColor(0.034f, 0.109f, 0.295f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Override Settings")
	FLinearColor OverallColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Override Settings")
	FLinearColor CloudColor = FLinearColor(0.856f, 0.919f, 1.0f, 1.0f);
};
