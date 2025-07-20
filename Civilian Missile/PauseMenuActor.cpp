// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuActor.h"
#include "PlayerCharacter.h"

bool APauseMenuActor::pauseStateChanged = false;

// Sets default values
APauseMenuActor::APauseMenuActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	battleMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("BattleMusic"));
	battleMusic->SetupAttachment(RootComponent);

	pauseMenuUI = nullptr;
	playerHUD_UI = nullptr;
}

UPlayerHUDWidget* APauseMenuActor::GetPlayerHUD_UI()
{
	return playerHUD_UI;
}

// Called when the game starts or when spawned
void APauseMenuActor::BeginPlay()
{
	Super::BeginPlay();

	// Hide the pause menu upon initialization
	pauseMenuUI = CreateWidget<UPauseMenuWidget>(GetWorld()->GetFirstPlayerController(), pauseMenuClass);
	pauseMenuUI->RemoveFromParent();

	// Initialize the player HUD
	playerHUD_UI = CreateWidget<UPlayerHUDWidget>(GetWorld()->GetFirstPlayerController(), playerHUDClass);

	if (GetWorld()->GetFirstPlayerController()->bShowMouseCursor != true)
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	// Set the main camera
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(cineCameraActor);

	// Set the battle music to the drumline sound once the object points to something
	if (battleMusic) battleMusic->SetSound(LoadObject<USoundBase>(nullptr, TEXT("/Game/Music/drumline__1_")));

	pauseStateChanged = false;

	if (APlayerCharacter::inMenu != false) APlayerCharacter::inMenu = false;
}

// Called every frame
void APauseMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Show the pause menu widget and mouse cursor when game is paused
	if (APlayerCharacter::IsGamePaused())
	{
		if (pauseStateChanged != true)
		{
			if (playerHUD_UI->IsInViewport()) playerHUD_UI->RemoveFromParent();

			pauseMenuUI = CreateWidget<UPauseMenuWidget>(GetWorld()->GetFirstPlayerController(), pauseMenuClass);
			pauseMenuUI->AddToViewport();

			pauseStateChanged = true;
		}
	}

	// Hide the pause menu widget and mouse cursor when game is paused
	else if (!APlayerCharacter::IsGamePaused())
	{
		if (pauseStateChanged != true)
		{
			if (pauseMenuUI->IsInViewport()) pauseMenuUI->RemoveFromParent();

			playerHUD_UI = CreateWidget<UPlayerHUDWidget>(GetWorld()->GetFirstPlayerController(), playerHUDClass);
			playerHUD_UI->AddToViewport();

			pauseStateChanged = true;
		}

		// Start updating the player's health bar, power bar and current time
		else if (pauseStateChanged == true)
		{
			playerHUD_UI->SetHealthProgressBar();
			playerHUD_UI->SetPowerProgressBar();
			playerHUD_UI->UpdateTimerText();
		}
	}

	// Play battle music on loop
	if (!battleMusic->IsPlaying()) battleMusic->Play();
}