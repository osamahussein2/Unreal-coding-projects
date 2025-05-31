// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "KeyActor.generated.h"

UCLASS()
class HOMEALONE_API AKeyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when other actors collide hits the collider
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* keyMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cubeKey;

	UPROPERTY(EditAnywhere)
	UBoxComponent* keyCollider;

	UPROPERTY(EditDefaultsOnly)
	UPointLightComponent* keyLight;

	UPROPERTY(EditAnywhere)
	UAudioComponent* keyAudio;

	UPROPERTY(EditAnywhere)
	float currentKeyLightIntensity = 100.0f;

	bool canInteract;
};
