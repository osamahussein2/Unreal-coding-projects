// Fill out your copyright notice in the Description page of Project Settings.

#include "Trigger/TakePhotoTrigger.h"
#include "Player/PlayerCharacter.h"

// Sets default values
ATakePhotoTrigger::ATakePhotoTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(staticMeshComponent);

	staticMeshComponent->SetGenerateOverlapEvents(true);

	staticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	staticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	staticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	staticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
}

// Called when the game starts or when spawned
void ATakePhotoTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (staticMeshComponent)
	{
		staticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ATakePhotoTrigger::AllowTakingPhotos);
		staticMeshComponent->OnComponentEndOverlap.AddDynamic(this, &ATakePhotoTrigger::DisableTakingPhotos);
	}
}

void ATakePhotoTrigger::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (staticMeshComponent)
	{
		staticMeshComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ATakePhotoTrigger::AllowTakingPhotos);
		staticMeshComponent->OnComponentEndOverlap.RemoveDynamic(this, &ATakePhotoTrigger::DisableTakingPhotos);

		staticMeshComponent->DestroyComponent();
		staticMeshComponent = nullptr;
	}
}

void ATakePhotoTrigger::AllowTakingPhotos(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		playerCharacter->SetTakingPhoto(ECurrentState::CanTakePhoto);
	}
}

void ATakePhotoTrigger::DisableTakingPhotos(UPrimitiveComponent * OverlappedComp, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this) return;

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		playerCharacter->SetTakingPhoto(ECurrentState::Exploration);
	}
}
