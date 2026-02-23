// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/AmmoCrate.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmoCrate::AAmmoCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ammoCrateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoCrateMesh"));
	ammoCrateMesh->SetupAttachment(GetRootComponent());

	ammoCrateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ammoCrateMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(ammoCrateMesh);

	glowingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlowingMesh"));
	glowingMesh->SetupAttachment(boxComponent);

	glowingMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	glowingMesh->SetHiddenInSceneCapture(true);

	boxComponent->SetGenerateOverlapEvents(true);

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoCrate::OnComponentBeginOverlap);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AAmmoCrate::OnComponentEndOverlap);

	SetCanBeDamaged(false);
}

// Called when the game starts or when spawned
void AAmmoCrate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAmmoCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoCrate::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor))
		{
			player->PlayerIsInAmmoCrateTrigger();
		}
	}
}

void AAmmoCrate::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor))
		{
			player->PlayerIsOutOfAmmoCrateTrigger();
		}
	}
}