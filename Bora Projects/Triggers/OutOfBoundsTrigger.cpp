// Fill out your copyright notice in the Description page of Project Settings.

#include "Triggers/OutOfBoundsTrigger.h"

// Sets default values
AOutOfBoundsTrigger::AOutOfBoundsTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup the out of bounds trigger
	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	trigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AOutOfBoundsTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}