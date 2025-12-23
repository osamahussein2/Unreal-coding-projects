// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/ScoreJam37_GM.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Player/PlayerCharacter.h"

AScoreJam37_GM::AScoreJam37_GM()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultPawnClass = nullptr;
	HUDClass = nullptr;

	soccerMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	soccerMusic->bIsUISound = true;
	soccerMusic->SetupAttachment(RootComponent);
}

void AScoreJam37_GM::BeginPlay()
{
	if (!PlayerCharacterClass) return;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	AActor* PlayerStart = nullptr;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() != nullptr && It->GetLevel()->bIsVisible)
		{
			PlayerStart = *It;
			break;
		}
	}

	if (!PlayerStart)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnPlayer: No valid PlayerStart found in visible levels."));
		return;
	}

	FVector SpawnLocation = PlayerStart->GetActorLocation();
	FRotator SpawnRotation = PlayerStart->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACharacter* CachedPlayerCharacter = GetWorld()->SpawnActor<ACharacter>(PlayerCharacterClass, SpawnLocation,
		SpawnRotation, SpawnParams);

	if (!CachedPlayerCharacter)	return;

	PlayerController->Possess(CachedPlayerCharacter);
	CachedPlayerCharacter->EnableInput(PlayerController);

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CachedPlayerCharacter))
	{
		// Set player's view target to camera actor
		PlayerCharacter->SetPlayerViewTarget();

		// Show main menu
		PlayerCharacter->ShowMainMenu(true);

		// Also set variables based on current difficulty at start
		PlayerCharacter->UpdateVariablesBasedOnCurrentDifficulty();
	}

	if (PlayerStart)
	{
		// Destroy player start after player has spawned in the level
		PlayerStart->Destroy();
		PlayerStart = nullptr;
	}

	// Set the soccer music volume to the current music volume and play the soccer music
	if (soccerMusic && soccerMusic->GetSound())
	{
		soccerMusic->SetVolumeMultiplier(static_cast<float>(musicVolume));
		soccerMusic->Play();
	}
}

void AScoreJam37_GM::UpdateSoccerMusicVolume(float& volume_)
{
	// Update soccer music volume by modifying the music volume slider in the options menu
	if (soccerMusic)
	{
		musicVolume = static_cast<float>(volume_);
		soccerMusic->SetVolumeMultiplier(volume_);
	}
}