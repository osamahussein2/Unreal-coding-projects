// Fill out your copyright notice in the Description page of Project Settings.

#include "Triggers/GameWonTrigger.h"
#include "Characters/Player/PlayerCharacter.h"

AGameWonTrigger::AGameWonTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup the game won trigger
	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	trigger->SetupAttachment(RootComponent);

	// Set the kill box trigger to use the signature of OnComponentBeginOverlap on the OnComponentBeginOverlap function
	trigger->OnComponentBeginOverlap.AddDynamic(this, &AGameWonTrigger::OnComponentBeginOverlap);
}

void AGameWonTrigger::BeginPlay()
{
}

void AGameWonTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		// Find a player in the level
		TArray<AActor*> FoundPlayer;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundPlayer);
		
		APlayerCharacter* player = Cast<APlayerCharacter>(FoundPlayer[0]);
		
		if (player)
		{
			player->ShowGameWonScreen();
			player = nullptr;
		}
	}
}
