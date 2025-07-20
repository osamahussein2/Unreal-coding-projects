// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuActor.h"
#include "PlayerCharacter.h"

// Sets default values
AMainMenuActor::AMainMenuActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	menuMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("MenuMusic"));
	menuMusic->SetupAttachment(RootComponent);

	// Initialize any variables
	mainMenuUI = nullptr;
}

// Called when the game starts or when spawned
void AMainMenuActor::BeginPlay()
{
	Super::BeginPlay();

	// Create the main menu widget and add it to viewport
	mainMenuUI = CreateWidget<UMainMenuWidget>(GetWorld()->GetFirstPlayerController(), mainMenuClass);
	mainMenuUI->AddToViewport();

	// Make sure the mouse cursor is turned on for clicking on main menu buttons
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	// Set the menu music to the menu music once the object points to something
	if (menuMusic) menuMusic->SetSound(LoadObject<USoundBase>(nullptr, TEXT("/Game/Music/Menu1NGE_1")));

	if (APlayerCharacter::inMenu != true) APlayerCharacter::inMenu = true;
}

void AMainMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Play menu music on loop
	if (!menuMusic->IsPlaying()) menuMusic->Play();
}
