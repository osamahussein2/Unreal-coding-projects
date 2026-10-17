// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/ComfyAutumnGM.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"
#include "Player/PlayerCharacter.h"
#include "CineCameraActor.h"

AComfyAutumnGM::AComfyAutumnGM()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void AComfyAutumnGM::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	ShowMainMenu();
	LoadMainMenuBackgroundLevel();
}

void AComfyAutumnGM::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (mainMenuBackgroundCamera)
	{
		mainMenuBackgroundCamera->Destroy();
		mainMenuBackgroundCamera = nullptr;
	}

	if (playerCharacter)
	{
		playerCharacter->Destroy();
		playerCharacter = nullptr;
	}
}

void AComfyAutumnGM::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	reloadLevelTimer += DeltaTime;

	if (reloadLevelTimer >= timeToReloadLevel)
	{
		LoadPlayingLevel();
		SetActorTickEnabled(false);

		reloadLevelTimer = 0.0f;
	}
}

void AComfyAutumnGM::ShowMainMenu()
{
	if (!IsValid(mainMenuClass) || mainMenu) return;

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		mainMenu = CreateWidget<UUserWidget>(playerController, mainMenuClass);
		if (!mainMenu) return;

		mainMenu->AddToViewport();
	}
}

void AComfyAutumnGM::DestroyMainMenu()
{
	if (!mainMenu) return;

	mainMenu->RemoveFromParent();
	mainMenu = nullptr;
}

void AComfyAutumnGM::LoadPlayingLevel()
{
	if (gameLevel.IsNull()) return;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	// Stream the game level
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnGameLevelLoaded");

	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, gameLevel, true, true, LatentInfo);
}

void AComfyAutumnGM::UnloadPlayingLevel(bool loadLevelAfterwards_)
{
	if (gameLevel.IsNull()) return;

	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, gameLevel, FLatentActionInfo(), false);

	if (playerCharacter)
	{
		playerCharacter->Destroy();
		playerCharacter = nullptr;
	}

	if (loadLevelAfterwards_)
	{
		SetActorTickEnabled(true);
	}
}

void AComfyAutumnGM::OnGameLevelLoaded()
{
	SetActorTickEnabled(false);

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

	if (playerCharacter)
	{
		PlayerController->Possess(playerCharacter);
		
		if (APlayerCharacter* player = Cast<APlayerCharacter>(playerCharacter))
		{
			player->OpenTutorialIntroductionPopup();
		}
	}

	PlayerStart->Destroy();
}

void AComfyAutumnGM::LoadMainMenuBackgroundLevel()
{
	if (mainMenuBackgroundLevel.IsNull()) return;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	// Stream the game level
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnMainMenuBackgroundLevelLoaded");

	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, mainMenuBackgroundLevel, true, false, LatentInfo);
}

void AComfyAutumnGM::UnloadMainMenuBackgroundLevel()
{
	if (mainMenuBackgroundLevel.IsNull()) return;

	UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, mainMenuBackgroundLevel, FLatentActionInfo(), false);

	if (mainMenuBackgroundCamera)
	{
		mainMenuBackgroundCamera->Destroy();
		mainMenuBackgroundCamera = nullptr;
	}
}

void AComfyAutumnGM::OnMainMenuBackgroundLevelLoaded()
{
	if (!IsValid(mainMenuBackgroundCameraClass) || mainMenuBackgroundCamera) return;

	mainMenuBackgroundCamera = GetWorld()->SpawnActor<ACineCameraActor>(mainMenuBackgroundCameraClass,
		mainMenuBackgroundCameraSpawn, FActorSpawnParameters());

	if (!mainMenuBackgroundCamera) return;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetViewTarget(mainMenuBackgroundCamera);
}
