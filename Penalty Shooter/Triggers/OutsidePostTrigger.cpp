// Fill out your copyright notice in the Description page of Project Settings.

#include "Triggers/OutsidePostTrigger.h"
#include "Components/BoxComponent.h"

// Sets default values
AOutsidePostTrigger::AOutsidePostTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AOutsidePostTrigger::BeginPlay()
{
	Super::BeginPlay();
}