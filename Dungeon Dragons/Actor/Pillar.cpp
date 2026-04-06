// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Pillar.h"

// Sets default values
APillar::APillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	pillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	SetRootComponent(pillarMesh);

	pillarMesh->bHiddenInSceneCapture = true;
}

// Called when the game starts or when spawned
void APillar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

