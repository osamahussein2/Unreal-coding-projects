// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewardActor.generated.h"

class USceneCaptureComponent2D;

UCLASS()
class BRACKEYSGAMEJAM_API ARewardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARewardActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void RotateAroundActor();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneCaptureComponent2D* sceneCaptureComponent;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float rotateSpeed = 5.0f;
};
