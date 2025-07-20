// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CinematicCamera/Public/CineCameraActor.h"
#include "PauseMenuWidget.h"
#include "PlayerHUDWidget.h"
#include "Components/AudioComponent.h"
#include "PauseMenuActor.generated.h"

UCLASS()
class KENNEYJAM2025_API APauseMenuActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APauseMenuActor();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPauseMenuWidget> pauseMenuClass;

	UPROPERTY()
	UPauseMenuWidget* pauseMenuUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUDWidget> playerHUDClass;

	UPROPERTY()
	UPlayerHUDWidget* playerHUD_UI;
	
	UFUNCTION()
	UPlayerHUDWidget* GetPlayerHUD_UI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static bool pauseStateChanged;

	UPROPERTY(EditAnywhere)
	ACineCameraActor* cineCameraActor;

	UPROPERTY(EditAnywhere)
	UAudioComponent* battleMusic;
};
