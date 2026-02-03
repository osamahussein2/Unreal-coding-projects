// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/Bun.h"

// Sets default values
ABun::ABun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BunMesh"));
	bunMesh->SetupAttachment(RootComponent);

	bunMesh->SetCollisionResponseToAllChannels(ECR_Block);
}

// Called when the game starts or when spawned
void ABun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

