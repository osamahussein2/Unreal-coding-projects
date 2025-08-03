// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "MainMenuActor.generated.h"

UCLASS()
class GMTKJAM2025_API AMainMenuActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainMenuActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Use the main menu widget set in inspector
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuWidget> mainMenuClass;

	// Set up main menu main menu widget
	UPROPERTY()
	UMainMenuWidget* mainMenuUI;

	// Import sound base from editor inspector
	UPROPERTY(EditAnywhere)
	USoundBase* mainMenuMusic;

	void ChangeForwardKey();
	void ChangeBackKey();
	void ChangeLeftKey();
	void ChangeRightKey();

	void ChangePauseKey();
	void ChangeAttackKey();
};
