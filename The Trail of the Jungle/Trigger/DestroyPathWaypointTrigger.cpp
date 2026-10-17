// Fill out your copyright notice in the Description page of Project Settings.

#include "Trigger/DestroyPathWaypointTrigger.h"
#include "Components/BoxComponent.h"
#include "Player/PlayerCharacter.h"

// Sets default values
ADestroyPathWaypointTrigger::ADestroyPathWaypointTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(boxComponent);

	boxComponent->SetGenerateOverlapEvents(true);

	boxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	boxComponent->SetCollisionObjectType(ECC_WorldStatic);
}

// Called when the game starts or when spawned
void ADestroyPathWaypointTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (boxComponent) boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestroyPathWaypointTrigger::DestroyPathWaypointTriggered);
}

void ADestroyPathWaypointTrigger::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (boxComponent)
	{
		boxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ADestroyPathWaypointTrigger::DestroyPathWaypointTriggered);

		boxComponent->DestroyComponent();
		boxComponent = nullptr;
	}
}

void ADestroyPathWaypointTrigger::DestroyPathWaypointTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		if (playerCharacter->GetCurrentSnapshotIndex() <= snapshotIndexForArea) return;

		playerCharacter->DestroyWaypoint();

		if (shouldLevelBeCompletedHere) playerCharacter->ShowLevelCompleted();
		Destroy();
	}
}
