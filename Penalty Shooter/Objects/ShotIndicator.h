// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "ShotIndicator.generated.h"

UCLASS()
class SCOREJAM37_API AShotIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShotIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USceneComponent* GetSceneComponent() const;

	void InvalidateShotIndicatorReferences();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UPaperSpriteComponent* spriteComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* sceneComponent;

	float rotationTimer;

	bool rotationFlipped;
	bool didRotationFlipFirstTime;

	class APlayerCharacter* playerCharacter = nullptr;
};
