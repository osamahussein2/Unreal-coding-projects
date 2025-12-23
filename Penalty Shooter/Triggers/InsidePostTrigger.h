// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InsidePostTrigger.generated.h"

// Inside post is meant for when the soccer ball has hit the post if it's going in the net
UCLASS()
class SCOREJAM37_API AInsidePostTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInsidePostTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* boxComponent;
};
