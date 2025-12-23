// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Components/AudioComponent.h"
#include "ScoreJam37_GM.generated.h"

/**
 * 
 */
UCLASS()
class SCOREJAM37_API AScoreJam37_GM : public AGameMode
{
	GENERATED_BODY()
	
public:
	AScoreJam37_GM();

	float GetMusicVolume() const { return musicVolume; }
	void UpdateSoccerMusicVolume(float& volume_);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> PlayerCharacterClass;

	UPROPERTY(EditDefaultsOnly)
	UAudioComponent* soccerMusic;

	float musicVolume = 1.0f;
};
