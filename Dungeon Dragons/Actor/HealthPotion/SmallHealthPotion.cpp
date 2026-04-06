// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/HealthPotion/SmallHealthPotion.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/DungeonCrawlerGM.h"

// Sets default values
ASmallHealthPotion::ASmallHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASmallHealthPotion::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!player) return;

	if (player->GetCollectedSmallHealthPotions() < 9)
	{
		player->IncrementCollectedSmallHealthPotion();

		if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->SpawnInventoryWidgetOnHUD();
		}
	}
	
	FTimerHandle takePotionHandle{};
	GetWorld()->GetTimerManager().SetTimer(takePotionHandle, this, &ASmallHealthPotion::TakeHealthPotion,
		lifeTime, false);
}

void ASmallHealthPotion::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AHealthPotion::EndPlay(EndPlayReason);
}

void ASmallHealthPotion::OnHealthPotionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

// Called every frame
void ASmallHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASmallHealthPotion::TakeHealthPotion()
{
	Destroy();
}

