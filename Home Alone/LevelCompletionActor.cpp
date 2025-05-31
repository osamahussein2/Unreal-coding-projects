// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCompletionActor.h"
#include "PlayerCharacter.h"

// Sets default values
ALevelCompletionActor::ALevelCompletionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	levelCompletionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LevelCompletionMesh"));
	levelCompletionMesh->SetupAttachment(RootComponent);

	levelCompletionCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("LevelCompletionCollider"));
	levelCompletionCollider->SetupAttachment(levelCompletionMesh);

	levelCompletionLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LevelCompletionPointLight"));
	levelCompletionLight->SetupAttachment(levelCompletionMesh);

	levelCompletionCollider->OnComponentBeginOverlap.AddDynamic(this, &ALevelCompletionActor::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void ALevelCompletionActor::BeginPlay()
{
	Super::BeginPlay();

	if (levelCompletionLight)
	{
		levelCompletionLight->SetIntensity(currentLightIntensity);
	}
}

void ALevelCompletionActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only if the player character has interacted with the toy and key that this collider is interactable
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && APlayerCharacter::toyDestroyed &&
		APlayerCharacter::keyPickedUp)
	{
		// Go the level completion screen
		UGameplayStatics::OpenLevel(this, levelName);
	}
}