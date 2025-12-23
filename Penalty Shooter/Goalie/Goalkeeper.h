// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "Goalkeeper.generated.h"

UCLASS()
class SCOREJAM37_API AGoalkeeper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoalkeeper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ResetGoalkeeper();

	void DecreaseGoalkeeperSpeed(float decreasedValue_);

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UPaperSpriteComponent* spriteComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* boxComponent;

	UPROPERTY(VisibleAnywhere)
	FVector initialLocation;

	UPROPERTY(EditAnywhere)
	FVector rightPostLocation;

	UPROPERTY(EditAnywhere)
	FVector leftPostLocation;

	UPROPERTY(EditAnywhere)
	float goalkeeperSpeed = 10.0f;

	bool directionChanged;

	
};
