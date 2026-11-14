// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PrizeMachine.generated.h"

class UBoxComponent;

UCLASS()
class SHROOMJAM2026_API APrizeMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrizeMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TurnFlashOn();

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> prizeMachine = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> boxComponent = nullptr;

	// Material
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> matInstanceDynamic = nullptr;

	UPROPERTY(EditAnywhere, Category = "Material Settings")
	float flashSpeed = 1.0f;

	bool colorLerpChanged = false;

	float timer = 0.0f;
};
