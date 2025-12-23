// Fill out your copyright notice in the Description page of Project Settings.


#include "Goalie/Goalkeeper.h"

// Sets default values
AGoalkeeper::AGoalkeeper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	spriteComponent->SetupAttachment(RootComponent);

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(spriteComponent);

	directionChanged = false;
}

// Called when the game starts or when spawned
void AGoalkeeper::BeginPlay()
{
	Super::BeginPlay();

	initialLocation = GetActorLocation();
}

// Called every frame
void AGoalkeeper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!directionChanged)
	{
		// Move to right of goalpost
		spriteComponent->AddRelativeLocation(rightPostLocation * goalkeeperSpeed * DeltaTime);

		// Change direction once the goalkeeper has reached the right post location (x-axis)
		if (GetActorLocation().X >= rightPostLocation.X) directionChanged = true;
	}

	else
	{
		// Move to left of goalpost
		spriteComponent->AddRelativeLocation(leftPostLocation * goalkeeperSpeed * DeltaTime);

		// Change direction once the goalkeeper has reached the left post location (x-axis)
		if (GetActorLocation().X <= leftPostLocation.X) directionChanged = false;
	}
}

void AGoalkeeper::ResetGoalkeeper()
{
	if (GetActorLocation() != initialLocation) SetActorLocation(initialLocation);
}

void AGoalkeeper::DecreaseGoalkeeperSpeed(float decreasedValue_)
{
	goalkeeperSpeed -= decreasedValue_;
}