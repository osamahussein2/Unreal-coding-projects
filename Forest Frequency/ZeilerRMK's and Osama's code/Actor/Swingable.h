// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Swingable.generated.h"

class UUserWidget;
class UWidgetComponent;
class ADefaultPlayer;

UCLASS()
class GAMEDEVTVGAMEJAM2026_API ASwingable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwingable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetCanSwing() const { return canSwing; }
	float GetTraceDistance() const { return traceDistance; }

	void DestroyWidgetComponent();

private:
	void SpawnSwingingWidget();

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

	// Swinging parameters
	UPROPERTY(EditAnywhere, Category = "Swinging Parameters")
	float traceDistance = 1000.0f;

	UPROPERTY(VisibleAnywhere, Category = "Swinging Parameters")
	bool canSwing = false;
};
