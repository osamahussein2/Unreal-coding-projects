// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Path.generated.h"

class UBoxComponent;

UCLASS(NotBlueprintType, Blueprintable, Placeable)
class COMFYAUTUMNJAM_API APath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenPath();

private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> sceneComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> boxComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> staticMeshComponent = nullptr;

	// Path Parameters
	UPROPERTY(VisibleAnywhere, Category = "Path Parameters")
	bool pathOpening = false;

	UPROPERTY(EditAnywhere, Category = "Path Parameters")
	float openPathSpeed = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Path Parameters")
	FRotator openPathRotation = FRotator::ZeroRotator;
};
