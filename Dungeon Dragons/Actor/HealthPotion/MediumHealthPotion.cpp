// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/HealthPotion/MediumHealthPotion.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/DungeonCrawlerGM.h"

// Sets default values
AMediumHealthPotion::AMediumHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMediumHealthPotion::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;

	if (player->GetCollectedMediumHealthPotions() < 9)
	{
		player->IncrementCollectedMediumHealthPotion();

		if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->SpawnInventoryWidgetOnHUD();
		}
	}
	
	FTimerHandle takePotionHandle{};
	GetWorld()->GetTimerManager().SetTimer(takePotionHandle, this, &AMediumHealthPotion::TakeHealthPotion,
		lifeTime, false);
}

void AMediumHealthPotion::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AHealthPotion::EndPlay(EndPlayReason);
}

void AMediumHealthPotion::OnHealthPotionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

// Called every frame
void AMediumHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMediumHealthPotion::TakeHealthPotion()
{
	Destroy();
}