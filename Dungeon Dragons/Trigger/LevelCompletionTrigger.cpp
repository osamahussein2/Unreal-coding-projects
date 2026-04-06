// Fill out your copyright notice in the Description page of Project Settings.

#include "Trigger/LevelCompletionTrigger.h"
#include "Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "DungeonCrawler2026/DungeonCrawler2026.h"
#include "GameMode/DungeonCrawlerGM.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelCompletionTrigger::ALevelCompletionTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	levelCompletionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LevelCompletionBox"));
	SetRootComponent(levelCompletionBox);

	levelCompletionBox->SetMobility(EComponentMobility::Static);

	levelCompletionBox->bHiddenInSceneCapture = true;

	levelCompletionSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LevelCompletionSprite"));
	levelCompletionSprite->SetupAttachment(levelCompletionBox);

	levelCompletionSprite->SetMobility(EComponentMobility::Static);

	levelCompletionSprite->bVisibleInSceneCaptureOnly = true;

	levelCompletionSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	levelCompletionSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	levelCompletionSprite->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Ignore);
	levelCompletionSprite->SetCollisionResponseToChannel(COLLISION_DOOR, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ALevelCompletionTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (levelCompletionBox)
	{
		levelCompletionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelCompletionTrigger::OnLevelCompletionOverlap);
	}
}

// Called every frame
void ALevelCompletionTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelCompletionTrigger::OnLevelCompletionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (player->GetDoorsUnlocked() >= doorsUnlockedAmount)
		{
			if (ADungeonCrawlerGM* gameMode = Cast<ADungeonCrawlerGM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				UGameplayStatics::SetGamePaused(GetWorld(), true);

				gameMode->EnableMouseCursor();

				gameMode->PopWidget(gameMode->GetMapClass());
				gameMode->PopWidget(gameMode->GetInventoryClass());
				gameMode->PopWidget(gameMode->GetPlayerHUDClass());
				gameMode->PushWidget(gameMode->GetLevelCompletionClass());

				gameMode->UnloadPlayingLevel();
			}
		}
	}
}

