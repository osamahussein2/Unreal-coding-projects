// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/ShroomJamGM.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"
#include "Player/PlayerCharacter.h"
#include "Manager/WidgetManager.h"
#include "Widget/PlayerHUDWidget.h"

AShroomJamGM::AShroomJamGM()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;
}

void AShroomJamGM::BeginPlay()
{
	Super::BeginPlay();

	if (gameMusic)
	{
		UGameplayStatics::PlaySound2D(this, gameMusic);
	}
}

void AShroomJamGM::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (playerCharacter)
	{
		playerCharacter->Destroy();
		playerCharacter = nullptr;
	}
}

void AShroomJamGM::LoadPlayingLevel()
{
	if (gameLevel.IsNull()) return;

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		// Stream the game level
		FLatentActionInfo LatentInfo;
		LatentInfo.Linkage = 0;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("OnGameLevelLoaded");

		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, gameLevel, true, true, LatentInfo);
	}
}

void AShroomJamGM::UnloadPlayingLevel()
{
	if (gameLevel.IsNull()) return;

	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, gameLevel, FLatentActionInfo(), false);

	if (playerCharacter)
	{
		playerCharacter->Destroy();
		playerCharacter = nullptr;
	}
}

void AShroomJamGM::SetPlayerCharacterToNull()
{
	if (playerCharacter && playerCharacter->IsValidLowLevel()) playerCharacter->Destroy();
	playerCharacter = nullptr;
}

void AShroomJamGM::OnGameLevelLoaded()
{
	if (!IsValid(playerCharacterClass)) return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
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

	if (!PlayerStart) return;

	FVector SpawnLocation = PlayerStart->GetActorLocation();
	FRotator SpawnRotation = PlayerStart->GetActorRotation();

	FActorSpawnParameters SpawnParams{};
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	playerCharacter = GetWorld()->SpawnActor<ACharacter>(playerCharacterClass, PlayerStart->GetActorLocation(),
		PlayerStart->GetActorRotation(), SpawnParams);

	if (playerCharacter) PlayerController->Possess(playerCharacter);

	PlayerStart->Destroy();
	
	if (AWidgetManager* widgetManager = Cast<AWidgetManager>(PlayerController->GetHUD()))
	{
		widgetManager->PushWidget(widgetManager->GetPlayerHUDClass());
	}
}
