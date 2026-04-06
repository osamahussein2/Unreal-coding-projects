// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/HealthPotion/HealthPotion.h"
#include "SmallHealthPotion.generated.h"

UCLASS()
class DUNGEONCRAWLER2026_API ASmallHealthPotion : public AHealthPotion
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmallHealthPotion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnHealthPotionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void TakeHealthPotion();

private:
	UPROPERTY(EditAnywhere, Category = "Lifetime")
	float lifeTime = 1.0f;
};
