// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/Sign.h"
#include "Object/WaypointActor.h"
#include "Object/Path.h"
#include "Trigger/TakePhotoTrigger.h"
#include "EngineUtils.h"

// Sets default values
ASign::ASign()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(staticMeshComponent);
}

// Called when the game starts or when spawned
void ASign::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASign::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASign::SetInitialPictureTaken()
{
	takenPictureOfSignStatus = ETakenPictureOfSignStatus::ETookInitialPictureOfIt;
}

void ASign::SetSignChanged()
{
	takenPictureOfSignStatus = ETakenPictureOfSignStatus::ESignChanged;
	
	SetActorRotation(signUpdatedRotation);
}

void ASign::SetUpdatedPictureTaken()
{
	takenPictureOfSignStatus = ETakenPictureOfSignStatus::ETookUpdatedPictureOfIt;

	if (GetWorld())
	{
		for (TActorIterator<ATakePhotoTrigger> takePhotoTriggerIt(GetWorld()); takePhotoTriggerIt;)
		{
			takePhotoTriggerIt->SetActorLocation(newPhotoMarkerLocation);
			break;
		}
	}

	if (!pathToOpen) return;

	pathToOpen->OpenPath();
	pathToOpen = nullptr;
}
