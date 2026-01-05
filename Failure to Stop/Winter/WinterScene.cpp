// Fill out your copyright notice in the Description page of Project Settings.

#include "Winter/WinterScene.h"
#include "Components/BoxComponent.h"

// Sets default values
AWinterScene::AWinterScene()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	winterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WinterMesh"));
	winterMesh->SetupAttachment(GetRootComponent());

	winterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	winterMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Create the floor box component and attach it as a child of the winter mesh
	floorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorBox"));
	floorBox->SetupAttachment(winterMesh);

	floorBox->SetGenerateOverlapEvents(false);

	floorBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	floorBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AWinterScene::BeginPlay()
{
	Super::BeginPlay();
}
