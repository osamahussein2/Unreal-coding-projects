// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/Topping.h"

// Sets default values
ATopping::ATopping()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATopping::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATopping::OnFoodBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABurger::OnFoodBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

// Called every frame
void ATopping::Tick(float DeltaTime)
{
	ABurger::Tick(DeltaTime);
}

void ATopping::DropFood()
{
	ABurger::DropFood();
}