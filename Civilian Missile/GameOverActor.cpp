// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverActor.h"
#include "PlayerCharacter.h"

// Sets default values
AGameOverActor::AGameOverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	loseMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("WinMusic"));
	loseMusic->SetupAttachment(RootComponent);

	gameOverUI = nullptr;
}

// Called when the game starts or when spawned
void AGameOverActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Create the game over widget and add it to viewport
	gameOverUI = CreateWidget<UGameOverWidget>(GetWorld()->GetFirstPlayerController(), gameOverClass);
	gameOverUI->AddToViewport();

	// Make sure the mouse cursor is turned on for clicking on game over buttons
	if (GetWorld()->GetFirstPlayerController()->bShowMouseCursor != true)
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	// Set the lose music once the object points to something
	if (loseMusic) loseMusic->SetSound(LoadObject<USoundBase>(nullptr, TEXT("/Game/Music/end__1_")));

	if (APlayerCharacter::inMenu != true) APlayerCharacter::inMenu = true;
}

void AGameOverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Play lose music on loop
	if (!loseMusic->IsPlaying()) loseMusic->Play();
}