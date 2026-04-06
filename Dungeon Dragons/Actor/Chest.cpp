// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Chest.h"
#include "PaperSpriteComponent.h"
#include "Actor/HealthPotion/HealthPotion.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthComponent.h"
#include "Engine/EngineTypes.h"

// Sets default values
AChest::AChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	chestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	SetRootComponent(chestMesh);

	chestMesh->bHiddenInSceneCapture = true;

	chestMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	chestMesh->SetGenerateOverlapEvents(true);

	chestMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	chestMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	chestSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ChestSprite"));
	chestSprite->SetupAttachment(chestMesh);

	chestSprite->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	chestSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	chestSprite->SetCollisionResponseToAllChannels(ECR_Ignore);

	chestSprite->bVisibleInSceneCaptureOnly = true;
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChest::Interact()
{
	// 0 for health potions, 1 for combat potions (scrapped idea)
	//const int& randomizedObjectTypeIndex = FMath::RandRange(0, 1);

	/*if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (player->GetHealthComponent() && player->GetHealthComponent()->IsHealthFull()) return;
	}*/

	if (healthPotions.IsEmpty()) return;

	const int& randomizedIndex = FMath::RandRange(0, healthPotions.Num() - 1);

	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		// Player has grabbed at least 9 small potions, check if player has not collected other potions 9 times to re-execute this function
		if (randomizedIndex == 0 && player->GetCollectedSmallHealthPotions() >= 9)
		{
			if (player->GetCollectedMediumHealthPotions() < 9 || player->GetCollectedLargeHealthPotions() < 9) Interact();
			return;
		}

		// Player has grabbed at least 9 medium potions, check if player has not collected other potions 9 times to re-execute this function
		else if (randomizedIndex == 1 && player->GetCollectedMediumHealthPotions() >= 9)
		{
			if (player->GetCollectedSmallHealthPotions() < 9 || player->GetCollectedLargeHealthPotions() < 9) Interact();
			return;
		}

		// Player has grabbed at least 9 large potions, check if player has not collected other potions 9 times to re-execute this function
		else if (randomizedIndex == 2 && player->GetCollectedLargeHealthPotions() >= 9)
		{
			if (player->GetCollectedSmallHealthPotions() < 9 || player->GetCollectedMediumHealthPotions() < 9) Interact();
			return;
		}
	}

	FTransform healthPotionTransform{};
	healthPotionTransform.SetTranslationAndScale3D(GetActorLocation() + potionSpawnOffset, potionSpawnScale);
	healthPotionTransform.SetRotation(FQuat(GetActorRotation()));

	AHealthPotion* healthPotion = GetWorld()->SpawnActorDeferred<AHealthPotion>(healthPotions[randomizedIndex],
		healthPotionTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (healthPotion) UGameplayStatics::FinishSpawningActor(healthPotion, healthPotionTransform);
}