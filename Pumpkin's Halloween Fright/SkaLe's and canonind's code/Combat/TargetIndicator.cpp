// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TargetIndicator.h"

ATargetIndicator::ATargetIndicator()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	IndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Indicator"));
	SetRootComponent(SceneRoot);
	IndicatorMesh->SetupAttachment(GetRootComponent());

	IndicatorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATargetIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATargetIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

