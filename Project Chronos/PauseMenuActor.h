// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PauseMenuWidget.h"
#include "Components/AudioComponent.h"
#include "PlayerHUDWidget.h"
#include "PauseMenuActor.generated.h"

class ABossCharacter;

UCLASS()
class GMTKJAM2025_API APauseMenuActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APauseMenuActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static bool pauseStateChanged;
	bool musicChanged;

private:
	// Pause menu
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPauseMenuWidget> pauseMenuClass;

	UPROPERTY()
	UPauseMenuWidget* pauseMenuUI;

	// Player HUD
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUDWidget> playerHUDClass;

	UPROPERTY()
	UPlayerHUDWidget* playerHUD_UI;

	// Import sound base from editor inspector
	UPROPERTY(EditAnywhere)
	USoundBase* battleMusic;

	UPROPERTY(EditAnywhere)
	USoundBase* battleMusic2;

	UAudioComponent* battleAudioComponent;

	UPROPERTY(EditAnywhere)
	ABossCharacter* bossCharacter;

	UPROPERTY(EditAnywhere)
	AActor* spawnPoint;

private:
	void UpdateBattleMusic();
	void PhaseChangeLogic();

	void ResetGame();
};
