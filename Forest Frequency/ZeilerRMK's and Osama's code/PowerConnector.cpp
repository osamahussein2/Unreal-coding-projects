// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerConnector.h"
#include "PowerCable.h"

// Sets default values
APowerConnector::APowerConnector()
{

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneRoot);

	ConnectionPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ConnectionPoint"));
	ConnectionPoint->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void APowerConnector::BeginPlay()
{
	Super::BeginPlay();
	
}

bool APowerConnector::CanConnectCable_Implementation(APowerCable* Cable) const
{
	if (bIsOccupied) return false;

	if (!Cable) return false;

	if (!ConnectionPoint) return false;

	return true;
}

void APowerConnector::ConnectCable_Implementation(APowerCable* Cable)
{
	if (!Cable) return;

	ConnectedCable = Cable;
	bIsOccupied = true;

	Cable->ConnectTo(this);
	bIsPowered = true;
}

void APowerConnector::DisconnectCable_Implementation(APowerCable* Cable)
{
	if (!Cable) return;

	ConnectedCable = nullptr;
	bIsOccupied = false;
	bIsPowered = false;
}

