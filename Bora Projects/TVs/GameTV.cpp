// Fill out your copyright notice in the Description page of Project Settings.

#include "TVs/GameTV.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameTV::AGameTV()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cubeMesh"));
	cubeMesh->SetupAttachment(RootComponent);

	planeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("planeMesh"));
	planeMesh->SetupAttachment(cubeMesh);
}

// Called when the game starts or when spawned
void AGameTV::BeginPlay()
{
	Super::BeginPlay();
}