// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameWinWidget.h"
#include "Components/AudioComponent.h"
#include "GameWinActor.generated.h"

UCLASS()
class KENNEYJAM2025_API AGameWinActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGameWinActor();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameWinWidget> gameWinClass;

	UPROPERTY()
	UGameWinWidget* gameWinUI;

	UPROPERTY(EditAnywhere)
	UAudioComponent* winMusic;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
