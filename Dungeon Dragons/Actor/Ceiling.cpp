// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Ceiling.h"

// Sets default values
ACeiling::ACeiling()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ceilingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CeilingMesh"));
	SetRootComponent(ceilingMesh);

	ceilingMesh->bHiddenInSceneCapture = true;
}

// Called when the game starts or when spawned
void ACeiling::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACeiling::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

