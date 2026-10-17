// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/Path.h"
#include "Components/BoxComponent.h"

// Sets default values
APath::APath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(sceneComponent);

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(sceneComponent);

	boxComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	boxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	boxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	boxComponent->SetGenerateOverlapEvents(false);

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	staticMeshComponent->SetupAttachment(boxComponent);

	staticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMeshComponent->SetCollisionObjectType(ECC_WorldDynamic);
	staticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	staticMeshComponent->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void APath::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
}

void APath::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (boxComponent)
	{
		boxComponent->UnregisterComponent();
		boxComponent->DestroyComponent();
		boxComponent = nullptr;
	}
}

// Called every frame
void APath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (pathOpening)
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), openPathRotation, DeltaTime, openPathSpeed));

		// If the current actor location Z exceeds the up location to stop opening, just destroy this actor
		if (GetActorRotation().Equals(openPathRotation, 0.1f))
		{
			Destroy();
		}
	}
}

void APath::OpenPath()
{
	if (pathOpening) return;

	pathOpening = true;
	SetActorTickEnabled(true);
}