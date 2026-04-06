// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Door.h"
#include "PaperSpriteComponent.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DungeonCrawler2026/DungeonCrawler2026.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	doorScene = CreateDefaultSubobject<USceneComponent>(TEXT("DoorScene"));
	SetRootComponent(doorScene);

	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	doorMesh->SetupAttachment(doorScene);

	doorMesh->bHiddenInSceneCapture = true;

	doorMesh->SetCollisionObjectType(COLLISION_DOOR);
	doorMesh->SetCollisionResponseToAllChannels(ECR_Block);
	doorMesh->SetCollisionResponseToChannel(COLLISION_DOOR, ECR_Ignore);

	doorSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DoorSprite"));
	doorSprite->SetupAttachment(doorMesh);

	doorSprite->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	doorSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	doorSprite->SetCollisionResponseToAllChannels(ECR_Ignore);

	doorSprite->bVisibleInSceneCaptureOnly = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (doorMesh)
	{
		if (doorStateMesh[0] && doorMesh->GetStaticMesh() != doorStateMesh[0]) doorMesh->SetStaticMesh(doorStateMesh[0]);

		if (doorScene)
		{
			const FRotator& doorSceneRotation = FRotator(doorScene->GetComponentRotation().Pitch,
				doorScene->GetComponentRotation().Yaw,
				doorScene->GetComponentRotation().Roll);

			doorMesh->SetWorldRotation(doorSceneRotation);
		}
	}
}

void ADoor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld()->GetTimerManager().IsTimerActive(openDoorHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(openDoorHandle);
	}
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::StartOpeningDoor()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(openDoorHandle))
	{
		doorOpening = true;

		if (doorStateMesh.Num() > 1 && doorStateMesh[1]) doorMesh->SetStaticMesh(doorStateMesh[1]);

		if (doorScene)
		{
			// Start with the door scene rotation (plus 90 degrees in yaw) to open the door mesh appropriately
			const FRotator& doorSceneRotation = FRotator(doorScene->GetComponentRotation().Pitch,
				doorScene->GetComponentRotation().Yaw + 90.0f,
				doorScene->GetComponentRotation().Roll);

			doorMesh->SetWorldRotation(doorSceneRotation);
		}

		GetWorld()->GetTimerManager().SetTimer(openDoorHandle, this, &ADoor::OpenDoor,
			GetWorld()->GetDeltaSeconds(), true);
	}
}

void ADoor::OpenDoor()
{
	if (doorScene->GetComponentRotation().Yaw <= rotationValueToLetPlayerProceed)
	{
		if (doorSprite)
		{
			if (GetWorld()->GetTimerManager().IsTimerActive(openDoorHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(openDoorHandle);
			}

			if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				player->IncrementDoorsUnlocked();

				if (doorMesh) doorMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

				doorSprite->DestroyComponent();
				doorSprite = nullptr;
			}
		}
	}

	else
	{
		doorRotation += doorOpenSpeed * GetWorld()->GetDeltaSeconds();

		if (doorScene)
		{
			const FRotator& openDoorRotation = FRotator(doorScene->GetComponentRotation().Pitch,
				doorScene->GetComponentRotation().Yaw + doorRotation,
				doorScene->GetComponentRotation().Roll);

			doorScene->SetWorldRotation(openDoorRotation);
		}
	}
}