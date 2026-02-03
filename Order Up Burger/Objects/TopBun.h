// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Bun.h"
#include "TopBun.generated.h"

UENUM()
enum class BunState
{
	EMovingSideways UMETA(DisplayName = "Moving Sideways"),
	EDroppingBun UMETA(DisplayName = "Dropping Bun"),
	EBunDropped UMETA(DisplayName = "Bun Dropped")
};

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API ATopBun : public ABun
{
	GENERATED_BODY()
	
public:
	ATopBun();

	virtual void DropTopBun();

	bool IfTopBunHasNotDroppedYet() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnTopBunBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Food")
	BunState bunState = BunState::EMovingSideways;

	UPROPERTY(EditAnywhere, Category = "Food")
	FString foodName = "";

	UPROPERTY(EditAnywhere, Category = "Food")
	float maxRightPos = 1.025f;

	UPROPERTY(EditAnywhere, Category = "Food", meta = (ClampMin = "0.0"))
	float dropSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Food")
	float maxDistanceToBottomBun = 30.0f;

	float movePos = 0.0f;

	bool reverseDirection = false;
};
