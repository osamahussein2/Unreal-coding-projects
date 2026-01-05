// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinterScene.generated.h"

UCLASS()
class GOEDWARECUTSCENE2JAM_API AWinterScene : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWinterScene();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Components
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* winterMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* floorBox;
};
