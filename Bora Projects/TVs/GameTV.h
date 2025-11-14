// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "GameTV.generated.h"

UCLASS()
class SHROOMJAM2025_API AGameTV : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameTV();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* cubeMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* planeMesh;

	class APlayerCharacter* playerCharacter;
	class USceneCaptureComponent2D* sceneCapture;
};
