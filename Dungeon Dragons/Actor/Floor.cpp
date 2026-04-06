// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Floor.h"
#include "PaperSpriteComponent.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	floorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	SetRootComponent(floorMesh);

	floorMesh->bHiddenInSceneCapture = true;

	floorSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("FloorSprite"));
	floorSprite->SetupAttachment(floorMesh);

	floorSprite->bVisibleInSceneCaptureOnly = true;

	floorSprite->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	floorSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	floorSprite->SetCollisionResponseToAllChannels(ECR_Ignore);
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}