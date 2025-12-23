// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "SoccerBall.generated.h"

UENUM()
enum class BallState : uint8
{
	ENormal,
	EReverseDirection,
	EHitInsidePost
};

UCLASS()
class SCOREJAM37_API ASoccerBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoccerBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetPlayerShotBall() const;
	void SetPlayerShotBall(bool value_);

	void ResetSoccerBall();

	void SpawnShotIndicator();
	void DestroyShotIndicator();

	void SetShotIndicatorLocation();

	float GetBallSpeed() const { return ballSpeed; }
	void IncreaseBallSpeed(int value_);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	UPaperSpriteComponent* spriteComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* sphereComponent;

	FVector initialBallPosition;

	bool playerShotBall = false;

	UPROPERTY(EditAnywhere)
	float ballSpeed = 5.0f;

	UPROPERTY(EditAnywhere)
	float reverseBallSpeed = 5.0f;

	UPROPERTY(EditAnywhere)
	float timeToRespawnBall = 1.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShotIndicator> shotIndicatorClass;

	class AShotIndicator* shotIndicator = nullptr;

	FVector shotIndicatorLocation;

	BallState ballState;

	FVector ballLocation;
	FVector shootDirection;
};
