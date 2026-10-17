// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/FilmCollectible.h"
#include "Components/BoxComponent.h"
#include "Player/PlayerCharacter.h"
#include "Object/Sign.h"
#include "Metasound/FilmPickupMetasound.h"

// Sets default values
AFilmCollectible::AFilmCollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(boxComponent);

	boxComponent->SetGenerateOverlapEvents(true);

	boxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	boxComponent->SetCollisionObjectType(ECC_WorldStatic);

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	staticMeshComponent->SetupAttachment(boxComponent);

	staticMeshComponent->SetGenerateOverlapEvents(false);

	staticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	staticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
}

// Called when the game starts or when spawned
void AFilmCollectible::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	
	if (boxComponent)
	{
		boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFilmCollectible::FilmCollected);
		boxComponent->OnComponentEndOverlap.AddDynamic(this, &AFilmCollectible::FilmLeft);
	}
}

void AFilmCollectible::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (boxComponent)
	{
		boxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AFilmCollectible::FilmCollected);
		boxComponent->OnComponentEndOverlap.RemoveDynamic(this, &AFilmCollectible::FilmLeft);

		boxComponent->DestroyComponent();
		boxComponent = nullptr;
	}

	if (overlappedPlayerCharacter) overlappedPlayerCharacter = nullptr;
	if (referencedSign) referencedSign = nullptr;
}

// Called every frame
void AFilmCollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (overlappedPlayerCharacter && overlappedPlayerCharacter->IsValidLowLevel())
	{
		if (referencedSign && referencedSign->IsValidLowLevel())
		{
			if (!overlappedPlayerCharacter->SignInViewOfPlayerCamera(referencedSign))
			{
				if (referencedSign->GetSignStatus() == ETakenPictureOfSignStatus::ETookInitialPictureOfIt)
				{
					overlappedPlayerCharacter->CollectFilm();
					overlappedPlayerCharacter->DestroyWaypoint();
					referencedSign->SetSignChanged();
					PlayFilmPickedUpMetasound();
					Destroy();
				}
			}
		}

		if (threeLaneSignInfo.threeLaneSign && threeLaneSignInfo.threeLaneSign->IsValidLowLevel())
		{
			if (!overlappedPlayerCharacter->SignInViewOfPlayerCamera(threeLaneSignInfo.threeLaneSign))
			{
				if (threeLaneSignInfo.threeLaneSign->GetSignStatus() == threeLaneSignInfo.threeLaneSignStatusForUpdating)
				{
					overlappedPlayerCharacter->CollectFilm();
					overlappedPlayerCharacter->DestroyWaypoint();

					if (threeLaneSignInfo.threeLaneSignStatusForUpdating == EThreeLaneSignStatus::ETakenFirstPicture)
					{
						threeLaneSignInfo.threeLaneSign->SetSignChangedOnce();
					}

					else if (threeLaneSignInfo.threeLaneSignStatusForUpdating == EThreeLaneSignStatus::ETakenSecondPicture)
					{
						threeLaneSignInfo.threeLaneSign->SetSignChangedTwice();
					}

					PlayFilmPickedUpMetasound();
					Destroy();
				}
			}
		}
	}
}

void AFilmCollectible::FilmCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		overlappedPlayerCharacter = playerCharacter;
		SetActorTickEnabled(true);
	}
}

void AFilmCollectible::FilmLeft(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor == this) return;

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		overlappedPlayerCharacter = nullptr;
		SetActorTickEnabled(false);
	}
}

void AFilmCollectible::PlayFilmPickedUpMetasound()
{
	if (!IsValid(filePickupMetaSoundClass) || !GetWorld()) return;

	AFilmPickupMetasound* filmPickupMetasound = GetWorld()->SpawnActor<AFilmPickupMetasound>(filePickupMetaSoundClass,
		FTransform(), FActorSpawnParameters());
}
