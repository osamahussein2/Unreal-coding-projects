// Fill out your copyright notice in the Description page of Project Settings.

#include "GameWinActor.h"
#include "PlayerCharacter.h"

// Sets default values
AGameWinActor::AGameWinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	winMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("WinMusic"));
	winMusic->SetupAttachment(RootComponent);

	gameWinUI = nullptr;
}

// Called when the game starts or when spawned
void AGameWinActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Create the game win widget and add it to viewport
	gameWinUI = CreateWidget<UGameWinWidget>(GetWorld()->GetFirstPlayerController(), gameWinClass);
	gameWinUI->AddToViewport();

	// Make sure the mouse cursor is turned on for clicking on game win buttons
	if (GetWorld()->GetFirstPlayerController()->bShowMouseCursor != true)
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	// Set the win music once the object points to something
	if (winMusic) winMusic->SetSound(LoadObject<USoundBase>(nullptr, TEXT("/Game/Music/Afro_Centric")));

	if (APlayerCharacter::inMenu != true) APlayerCharacter::inMenu = true;
}

void AGameWinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Play win music on loop
	if (!winMusic->IsPlaying()) winMusic->Play();
}