// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/ThreeLaneSign.h"
#include "Object/WaypointActor.h"
#include "Object/Path.h"

// Sets default values
AThreeLaneSign::AThreeLaneSign()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(staticMeshComponent);
}

// Called when the game starts or when spawned
void AThreeLaneSign::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThreeLaneSign::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThreeLaneSign::SetTakenFirstPicture()
{
	takenPictureOfSignStatus = EThreeLaneSignStatus::ETakenFirstPicture;
}

void AThreeLaneSign::SetSignChangedOnce()
{
	takenPictureOfSignStatus = EThreeLaneSignStatus::ESignChangedOnce;

	SetActorRotation(signUpdatedFirstRotation);
}

void AThreeLaneSign::SetTakenSecondPicture()
{
	takenPictureOfSignStatus = EThreeLaneSignStatus::ETakenSecondPicture;
}

void AThreeLaneSign::SetSignChangedTwice()
{
	takenPictureOfSignStatus = EThreeLaneSignStatus::ESignChangedTwice;

	SetActorRotation(signUpdatedSecondRotation);
}

void AThreeLaneSign::SetTakenThirdPicture()
{
	takenPictureOfSignStatus = EThreeLaneSignStatus::ETakenThirdPicture;

	if (!pathToOpen) return;

	pathToOpen->OpenPath();
	pathToOpen = nullptr;
}
