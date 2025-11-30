// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"
#include "Components/AudioComponent.h"
#include "Blender.generated.h"

UCLASS()
class SHROOMJAM2025_API ABlender : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlender();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void PlayBlenderAlarmedAnimation();

	void PlayBlenderSoundAgain();
	void MakeEnemiesDetectPlayer();

private:
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* skeletalMeshComponent;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<USkeletalMesh>> blenderMeshes;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UAnimSequence>> blenderAnimations;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> blenderSound;

	UPROPERTY(EditAnywhere)
	float enemyDetectPlayerDelay = 1.0f;

	UPROPERTY(EditAnywhere)
	float playSoundDelay = 7.0f;

	bool hasBlenderAnimationChanged;

	TArray<class AEnemy*> enemies;
};
