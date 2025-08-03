// Fill out your copyright notice in the Description page of Project Settings.

#include "KillBox.h"
#include "PlayerCharacter.h"

// Sets default values
AKillBox::AKillBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Setup the kill box trigger
	killboxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("KillboxTrigger"));
	killboxTrigger->SetupAttachment(RootComponent);

	// Set the kill box trigger to use the signature of OnComponentBeginOverlap on the OnComponentBeginOverlap function
	killboxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AKillBox::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void AKillBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKillBox::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If player overlaps with the killbox trigger, reset the game
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		APlayerCharacter::SetPlayerHealth(0.0f);
	}
}

// Called every frame
/*void AKillBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/