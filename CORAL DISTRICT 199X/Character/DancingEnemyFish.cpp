// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/DancingEnemyFish.h"

// Sets default values
ADancingEnemyFish::ADancingEnemyFish()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
}

// Called when the game starts or when spawned
void ADancingEnemyFish::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADancingEnemyFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADancingEnemyFish::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADancingEnemyFish::StartDancing()
{
	if (danceAnimations.IsEmpty()) return;

	const int& randomEnemyDanceIndex = FMath::RandRange(0, danceAnimations.Num() - 1);

	if (danceAnimations[randomEnemyDanceIndex])
	{
		GetMesh()->SetAnimation(danceAnimations[randomEnemyDanceIndex]);
		GetMesh()->Play(true);

		GetWorld()->GetTimerManager().SetTimer(randomDanceHandle, this, &ADancingEnemyFish::StartDancing,
			danceAnimations[randomEnemyDanceIndex]->GetPlayLength(), true);
	}
}

void ADancingEnemyFish::StopDancing()
{
	if (danceAnimations.IsEmpty()) return;

	if (danceAnimations[0] && danceAnimations[1] && breakDanceAnimation)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(randomDanceHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(randomDanceHandle);
		}

		GetMesh()->SetAnimation(nullptr);
		GetMesh()->Stop();
	}
}

void ADancingEnemyFish::BreakDanceOnPlayer()
{
	if (breakDanceAnimation)
	{
		GetMesh()->SetAnimation(breakDanceAnimation);
		GetMesh()->Play(false);
	}
}