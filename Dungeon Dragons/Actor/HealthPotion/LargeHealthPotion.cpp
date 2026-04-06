// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/HealthPotion/LargeHealthPotion.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/DungeonCrawlerGM.h"

// Sets default values
ALargeHealthPotion::ALargeHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ALargeHealthPotion::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;

	if (player->GetCollectedLargeHealthPotions() < 9)
	{
		player->IncrementCollectedLargeHealthPotion();

		if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->SpawnInventoryWidgetOnHUD();
		}
	}
	
	FTimerHandle takePotionHandle{};
	GetWorld()->GetTimerManager().SetTimer(takePotionHandle, this, &ALargeHealthPotion::TakeHealthPotion,
		lifeTime, false);
}

void ALargeHealthPotion::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AHealthPotion::EndPlay(EndPlayReason);
}

void ALargeHealthPotion::OnHealthPotionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

// Called every frame
void ALargeHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALargeHealthPotion::TakeHealthPotion()
{
	Destroy();
}