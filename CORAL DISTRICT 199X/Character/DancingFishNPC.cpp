// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/DancingFishNPC.h"

// Sets default values
ADancingFishNPC::ADancingFishNPC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	skeletalMesh->SetupAttachment(GetRootComponent());

	skeletalMesh->SetAnimationMode(EAnimationMode::Type::AnimationSingleNode);
}

// Called when the game starts or when spawned
void ADancingFishNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADancingFishNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADancingFishNPC::MakeFishNPCDance()
{
	if (skeletalMesh && dancingAnimation)
	{
		skeletalMesh->SetAnimation(dancingAnimation);
		skeletalMesh->Play(true);
	}
}