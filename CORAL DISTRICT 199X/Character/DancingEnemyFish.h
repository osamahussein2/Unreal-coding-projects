// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DancingEnemyFish.generated.h"

UCLASS()
class BRACKEYSGAMEJAM_API ADancingEnemyFish : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADancingEnemyFish();

	void StartDancing();
	void StopDancing();

	void BreakDanceOnPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "Animations")
	TArray<UAnimSequence*> danceAnimations;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* breakDanceAnimation;

	FTimerHandle randomDanceHandle;
};
