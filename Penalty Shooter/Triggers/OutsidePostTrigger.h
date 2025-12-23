// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OutsidePostTrigger.generated.h"

// Outside post is meant for when the soccer ball has hit the post if it's NOT going in the net
UCLASS()
class SCOREJAM37_API AOutsidePostTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOutsidePostTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* boxComponent;
};
