// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grapplable.generated.h"

class UUserWidget;
class UWidgetComponent;
class ADefaultPlayer;

UCLASS()
class GAMEDEVTVGAMEJAM2026_API AGrapplable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrapplable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetCanGrapple() const { return canGrapple; }
	FVector GetGrapplePoint() const { return grapplePoint; }

	void DestroyWidgetComponent();

private:
	void SpawnGrapplePointWidget();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> staticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UWidgetComponent> widgetComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> widgetClass;

	// Player character
	UPROPERTY()
	ADefaultPlayer* playerCharacter = nullptr;

	// Grapple parameters
	UPROPERTY(VisibleAnywhere, Category = "Grapple Parameters")
	FVector grapplePoint = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = "Grapple Parameters")
	FRotator grappleRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Grapple Parameters")
	float traceDistance = 1000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Grapple Parameters")
	bool canGrapple = false;

	UPROPERTY(VisibleAnywhere, Category = "Grapple Parameters")
	float grapplePointZ = 0.0f;

};
