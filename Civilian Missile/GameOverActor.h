// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameOverWidget.h"
#include "Components/AudioComponent.h"
#include "GameOverActor.generated.h"

UCLASS()
class KENNEYJAM2025_API AGameOverActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGameOverActor();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameOverWidget> gameOverClass;

	UPROPERTY()
	UGameOverWidget* gameOverUI;

	UPROPERTY(EditAnywhere)
	UAudioComponent* loseMusic;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
