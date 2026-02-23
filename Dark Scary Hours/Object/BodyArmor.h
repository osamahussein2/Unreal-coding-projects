// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BodyArmor.generated.h"

class UBoxComponent;

UCLASS()
class EHGAMEJAM_API ABodyArmor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABodyArmor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when other actors collide hits the collider
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void DestroyGlowingMesh();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* bodyArmorMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* glowingMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* boxComponent = nullptr;
};
