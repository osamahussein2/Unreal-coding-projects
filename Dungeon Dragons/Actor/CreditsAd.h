// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreditsAd.generated.h"

UCLASS()
class DUNGEONCRAWLER2026_API ACreditsAd : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreditsAd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* cubeMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Ads")
	TArray<UTexture2D*> adTextures;

	UPROPERTY(EditAnywhere, Category = "Ads")
	float startingMaterialTimer = -1.0f;

	UPROPERTY()
	UMaterialInstanceDynamic* materialDyn = nullptr;

	int adIndex = 0;

	float materialTimer = 0.0f;

	bool texturesUpdated = false;
};
