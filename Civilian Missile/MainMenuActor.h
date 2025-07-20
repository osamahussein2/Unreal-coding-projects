// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "MainMenuWidget.h"
#include "MainMenuActor.generated.h"

UCLASS()
class KENNEYJAM2025_API AMainMenuActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMainMenuActor();

	// Use the main menu widget set in inspector
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuWidget> mainMenuClass;

	// Set up main menu main menu widget
	UPROPERTY()
	UMainMenuWidget* mainMenuUI;

	UPROPERTY(EditAnywhere)
	UAudioComponent* menuMusic;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};