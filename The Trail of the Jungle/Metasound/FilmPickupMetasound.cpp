// Fill out your copyright notice in the Description page of Project Settings.

#include "Metasound/FilmPickupMetasound.h"
#include "MetasoundSource.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AFilmPickupMetasound::AFilmPickupMetasound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AFilmPickupMetasound::BeginPlay()
{
	Super::BeginPlay();
	
	if (filmPickupSettings.IsEmpty() || !GetRootComponent() || !GetWorld())
	{
		DestroyActor();
		return;
	}

	const int& randomIndex = FMath::RandRange(0, filmPickupSettings.Num() - 1);

	if (!filmPickupSettings[randomIndex].metasoundSource)
	{
		DestroyActor();
		return;
	}

	UGameplayStatics::SpawnSoundAttached(filmPickupSettings[randomIndex].metasoundSource, GetRootComponent());

	FTimerHandle destroyActorHandle{};

	GetWorld()->GetTimerManager().SetTimer(destroyActorHandle, this, &AFilmPickupMetasound::DestroyActor,
		filmPickupSettings[randomIndex].endTime, false);

	for (int i = 0; i < filmPickupSettings.Num(); ++i)
	{
		filmPickupSettings[i].metasoundSource = nullptr;
	}

	filmPickupSettings.Empty();
}

// Called every frame
void AFilmPickupMetasound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFilmPickupMetasound::DestroyActor()
{
	Destroy();
}