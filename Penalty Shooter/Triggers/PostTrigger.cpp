// Fill out your copyright notice in the Description page of Project Settings.

#include "Triggers/PostTrigger.h"

// Sets default values
APostTrigger::APostTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APostTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}