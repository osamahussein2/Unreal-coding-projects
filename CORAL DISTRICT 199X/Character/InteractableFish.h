// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractableFish.generated.h"

class UBoxComponent;

UCLASS()
class BRACKEYSGAMEJAM_API AInteractableFish : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInteractableFish();

	void DestroyBoxTrigger();
	void InvalidateDynamicMaterial();

	void SpawnBoxTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnFishBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnFishEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//bool GetPlayerOnFishTrigger() const { return playerOnFishTrigger; }

private:
	void ConstructInputWidget();
	void DestroyInputWidget();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* fishInteractionTrigger;

	UPROPERTY(VisibleAnywhere, Category = "Static Material")
	UMaterialInterface* startingFishMaterial = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Dynamic Material")
	UMaterialInstanceDynamic* fishDynamicMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Dynamic Material")
	float updateMaterialSpeed = 0.5f;

	float saturation = 1.0f;

	bool emissiveColorChanged = false;

	//bool playerOnFishTrigger = false;

	// Input Widget
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<class UWidgetComponent> widgetComponent;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UUserWidget> inputWidgetClass;

	// Widget spawn parameters
	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FVector widgetComponentSpawnLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FRotator widgetComponentSpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FVector widgetComponentSpawnScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Widget Transform")
	FVector2D widgetComponentDrawSize = FVector2D(500.0f, 500.0f);

	FVector startingBoxExtents = FVector::ZeroVector;
};
