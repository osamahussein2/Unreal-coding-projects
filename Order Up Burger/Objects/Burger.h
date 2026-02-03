// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Burger.generated.h"

UENUM()
enum class FoodState
{
	EMovingSideways UMETA(DisplayName = "Moving Sideways"),
	EDroppingFood UMETA(DisplayName = "Dropping Food"),
	EToppingDropped UMETA(DisplayName = "Food Dropped")
};

UCLASS()
class PORTFOLIOBUILDERS_API ABurger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABurger();

	virtual void DropFood();

	bool IfFoodHasNotDroppedYet() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnFoodBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> foodMesh;

	UPROPERTY(EditAnywhere, Category = "Food")
	FString foodName = "";

	UPROPERTY(VisibleAnywhere, Category = "Food")
	FoodState foodState = FoodState::EMovingSideways;

	UPROPERTY(EditAnywhere, Category = "Food")
	float maxRightPos = 1.025f;

	UPROPERTY(EditAnywhere, Category = "Food", meta = (ClampMin = "0.0"))
	float dropSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Food")
	float maxDistanceToBottomBun = 30.0f;

	UPROPERTY(EditAnywhere, Category = "Score Parameters")
	float scoreLoss = 20.0f;

	float movePos = 0.0f;

	bool reverseDirection = false;
};
