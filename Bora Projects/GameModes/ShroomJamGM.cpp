// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModes/ShroomJamGM.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Characters/Player/PlayerCharacter.h"

AShroomJamGM::AShroomJamGM()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultPawnClass = nullptr;
	HUDClass = nullptr;

	defaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	defaultSceneComponent->SetupAttachment(RootComponent);
}

void AShroomJamGM::BeginPlay()
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

	// Check if the player character cast works to assign found cine camera to player's view target
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CachedPlayerCharacter))
	{
		//PlayerCharacter->DisableMouseCursor();
		PlayerCharacter->AssignCineCameraToPlayer();
		PlayerCharacter->ShowPlayerHUD(); // Also show the HUD as well

		// For mobile builds only the mobile touch interface will pop up for the player
		PlayerCharacter->InitializeMobileInterface();
	}

	// Destroy player start after player has spawned into the level
	if (PlayerStart)
	{
		PlayerStart->Destroy();
		PlayerStart = nullptr;
	}

	// Set all pointers to nullptr after everything has loaded correctly
	CachedPlayerCharacter = nullptr;
	PlayerController = nullptr;
}