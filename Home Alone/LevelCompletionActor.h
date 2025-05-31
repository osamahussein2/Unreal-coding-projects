// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LevelCompletionActor.generated.h"

UCLASS()
class HOMEALONE_API ALevelCompletionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelCompletionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when other actors collide hits the collider
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* levelCompletionMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* levelCompletionCollider;

	UPROPERTY(EditAnywhere)
	UPointLightComponent* levelCompletionLight;

	UPROPERTY(EditAnywhere)
	FName levelName;

	UPROPERTY(EditAnywhere)
	float currentLightIntensity = 70.0f;
};
