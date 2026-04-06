// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

class UPaperSpriteComponent;

UCLASS()
class DUNGEONCRAWLER2026_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetFloorNumberForDoorsUnlocked() const { return floorNumber; }

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> floorMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UPaperSpriteComponent> floorSprite;

	UPROPERTY(EditAnywhere, Category = "Floor")
	int floorNumber = 0;
};
