// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Wall.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	wallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	SetRootComponent(wallMesh);

	wallMesh->bHiddenInSceneCapture = true;
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

