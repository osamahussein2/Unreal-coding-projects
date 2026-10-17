// Fill out your copyright notice in the Description page of Project Settings.

#include "Metasound/CameraShutterMetasound.h"
#include "MetasoundSource.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
ACameraShutterMetasound::ACameraShutterMetasound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACameraShutterMetasound::BeginPlay()
{
	Super::BeginPlay();

	if (cameraShutterSettings.IsEmpty() || !GetRootComponent() || !GetWorld())
	{
		DestroyActor();
		return;
	}
	
	const int& randomIndex = FMath::RandRange(0, cameraShutterSettings.Num() - 1);

	if (!cameraShutterSettings[randomIndex].metasoundSource)
	{
		DestroyActor();
		return;
	}

	UGameplayStatics::SpawnSoundAttached(cameraShutterSettings[randomIndex].metasoundSource, GetRootComponent());

	FTimerHandle destroyActorHandle{};

	GetWorld()->GetTimerManager().SetTimer(destroyActorHandle, this, &ACameraShutterMetasound::DestroyActor,
		cameraShutterSettings[randomIndex].endTime, false);

	for (int i = 0; i < cameraShutterSettings.Num(); ++i)
	{
		cameraShutterSettings[i].metasoundSource = nullptr;
	}

	cameraShutterSettings.Empty();
}

// Called every frame
void ACameraShutterMetasound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACameraShutterMetasound::DestroyActor()
{
	Destroy();
}