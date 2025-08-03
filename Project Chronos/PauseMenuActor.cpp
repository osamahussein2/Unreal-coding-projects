// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuActor.h"
#include "PlayerCharacter.h"
#include "BossCharacter.h"

bool APauseMenuActor::pauseStateChanged = false;

// Sets default values
APauseMenuActor::APauseMenuActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	battleAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BattleAudioComponent"));
	battleAudioComponent->SetupAttachment(RootComponent);

	pauseMenuUI = nullptr;
	playerHUD_UI = nullptr;
}

// Called when the game starts or when spawned
void APauseMenuActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Create both pause menu and player HUD widgets
	pauseMenuUI = CreateWidget<UPauseMenuWidget>(GetWorld()->GetFirstPlayerController(), pauseMenuClass);
	playerHUD_UI = CreateWidget<UPlayerHUDWidget>(GetWorld()->GetFirstPlayerController(), playerHUDClass);

	if (APlayerCharacter::isInMenu != false) APlayerCharacter::isInMenu = false;

	pauseStateChanged = false;

	// Play the battle music
	if (battleAudioComponent && battleMusic)
	{
		battleAudioComponent->SetSound(battleMusic);
	}

	musicChanged = false;

	APlayerCharacter::phaseNumber = 1;

	APlayerCharacter::SetPlayerHealth(APlayerCharacter::GetPlayerMaxHealth());

	// Just in case if the phase number is over a 100 at the start of level, keep it to a 100
	if (APlayerCharacter::phaseNumber >= 100) APlayerCharacter::phaseNumber = 100;
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

			FInputModeGameAndUI InputMode;
			InputMode.SetHideCursorDuringCapture(false); // Don't hide the cursor during capture
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Don't worry about mouse lock
			GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);

			if (GetWorld()->GetFirstPlayerController()->bShowMouseCursor != true)
				GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

			// Pause battle music
			if (battleAudioComponent && battleMusic) battleAudioComponent->SetPaused(true);

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

			FInputModeGameOnly InputMode;
			InputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away
			GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);

			if (GetWorld()->GetFirstPlayerController()->bShowMouseCursor != false)
				GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

			// Resume battle music
			if (battleAudioComponent && battleMusic) battleAudioComponent->SetPaused(false);

			pauseStateChanged = true;
		}

		else
		{
			UpdateBattleMusic();
			PhaseChangeLogic();
			ResetGame();

			if (playerHUD_UI)
			{
				playerHUD_UI->SetPlayerHealthProgressBar();

				if (bossCharacter)
				{
					playerHUD_UI->SetEnemyHealthProgressBar(bossCharacter->GetHealth(), bossCharacter->GetMaxHealth());
				}

				playerHUD_UI->SetPhaseNumberText();

				switch (APlayerCharacter::phaseNumber)
				{
				case 1:
					playerHUD_UI->UpdateBossFillHealth(0);
					break;

				case 2:
					playerHUD_UI->UpdateBossFillHealth(1);
					break;

				default:
					playerHUD_UI->UpdateBossFillHealth(1);
					break;
				}
			}

		}
	}
}

void APauseMenuActor::UpdateBattleMusic()
{
	if (musicChanged && APlayerCharacter::phaseNumber == 1)
	{
		if (battleAudioComponent && battleMusic)
		{
			battleAudioComponent->Stop();
			battleAudioComponent->SetSound(battleMusic);
		}

		musicChanged = false;
	}

	else if (musicChanged && APlayerCharacter::phaseNumber >= 2)
	{
		if (battleAudioComponent && battleMusic)
		{
			battleAudioComponent->Stop();
			battleAudioComponent->SetSound(battleMusic2);
		}

		musicChanged = false;
	}

	// Otherwise if music hasn't changed play the corresponding battle music depending on phase number
	if (!musicChanged && APlayerCharacter::phaseNumber == 1)
	{
		if (battleAudioComponent && battleMusic)
		{
			// Play battle music if it's not playing already and it is valid along with its audio component
			if (battleAudioComponent && battleMusic && !battleAudioComponent->IsPlaying())
				battleAudioComponent->Play();
		}
	}

	else if (!musicChanged && APlayerCharacter::phaseNumber >= 2)
	{
		if (battleAudioComponent && battleMusic2)
		{
			// Play battle music if it's not playing already and it is valid along with its audio component
			if (battleAudioComponent && battleMusic2 && !battleAudioComponent->IsPlaying())
				battleAudioComponent->Play();
		}
	}
}

void APauseMenuActor::PhaseChangeLogic()
{
	if (bossCharacter && bossCharacter->GetHealth() <= 0.0f && bossCharacter->GetIsDead() && 
		bossCharacter->GetDeathTime() >= 1.0f)
	{
		if (APlayerCharacter::phaseNumber <= 1) musicChanged = true;

		if (APlayerCharacter::phaseNumber < 100) APlayerCharacter::phaseNumber += 1;
		else  if (APlayerCharacter::phaseNumber >= 100) APlayerCharacter::phaseNumber = 100;

		APlayerCharacter::SetPlayerHealth(APlayerCharacter::GetPlayerMaxHealth());

		bossCharacter->SetActorLocation(bossCharacter->GetStartingPosition());
		bossCharacter->ResetBoss();

		if (spawnPoint)
		{
			// Respawn the player back into the level
			GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(spawnPoint->GetActorLocation());
		}
	}
}

void APauseMenuActor::ResetGame()
{
	if (APlayerCharacter::GetPlayerHealth() <= 0.0f && APlayerCharacter::GetIsDead() &&
		APlayerCharacter::GetDeathTime() >= 1.0f)
	{
		APlayerCharacter::SetPlayerHealth(APlayerCharacter::GetPlayerMaxHealth());

		UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/death_Sound")));

		if (APlayerCharacter::phaseNumber > 1)
		{
			APlayerCharacter::phaseNumber -= 1;
		}

		if (APlayerCharacter::phaseNumber <= 1) musicChanged = true;

		if (bossCharacter)
		{
			bossCharacter->SetActorLocation(bossCharacter->GetStartingPosition());
			bossCharacter->ResetBoss();
		}

		if (spawnPoint)
		{
			// Respawn the player back into the level
			GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(spawnPoint->GetActorLocation());
		}
	}
}