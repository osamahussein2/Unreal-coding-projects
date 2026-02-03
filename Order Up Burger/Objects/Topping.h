// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Objects/Burger.h"
#include "Topping.generated.h"

UCLASS()
class PORTFOLIOBUILDERS_API ATopping : public ABurger
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATopping();

	virtual void DropFood() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnFoodBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
