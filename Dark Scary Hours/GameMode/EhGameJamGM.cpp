// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/EhGameJamGM.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Character/PlayerCharacter.h"

AEhGameJamGM::AEhGameJamGM()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
}

void AEhGameJamGM::BeginPlay()
{
	if (!playerMenuClass) return;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	menuPlayer = GetWorld()->SpawnActor<APawn>(playerMenuClass, FVector::ZeroVector,
		FRotator::ZeroRotator, FActorSpawnParameters());

	if (!menuPlayer) return;

	playerController->Possess(menuPlayer);
	menuPlayer->EnableInput(playerController);

	EnableMouseCursor();
	PushWidget(mainMenuClass);
}

void AEhGameJamGM::PushWidget(TSubclassOf<UUserWidget> widgetClass_)
{
	// Only if the widget class if valid
	if (widgetClass_)
	{
		// Add whatever widget subclass is passed in into the cached widgets array
		cachedWidgets.Add(CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), widgetClass_));

		// Set the last index to be the last added element in the cached widgets array
		const int& lastIndex = cachedWidgets.Num() - 1;

		// If the cached widget at the last pushed index is valid, add it to viewport
		if (cachedWidgets[lastIndex]) cachedWidgets[lastIndex]->AddToViewport();

		// Otherwise, just remove it from the array as we don't want the array to hold an invalid widget
		else cachedWidgets.RemoveAt(lastIndex);
	}
}

void AEhGameJamGM::PopWidget(TSubclassOf<UUserWidget> widgetClass_)
{
	for (int i = 0; i < cachedWidgets.Num(); ++i)
	{
		if (widgetClass_ && cachedWidgets[i]->GetClass() == widgetClass_)
		{
			cachedWidgets[i]->RemoveFromParent();
			cachedWidgets[i] = nullptr;

			cachedWidgets.RemoveAt(i);
		}

		else continue;
	}
}

void AEhGameJamGM::EnableMouseCursor()
{
	// Exclusive to PC
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false); // Don't hide the cursor during capture
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetInputMode(InputMode);

	if (playerController->bShowMouseCursor != true) playerController->bShowMouseCursor = true;
}

void AEhGameJamGM::DisableMouseCursor()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetInputMode(GameInputMode);

	// Make sure the mouse cursor is turned off
	if (playerController->bShowMouseCursor != false) playerController->bShowMouseCursor = false;
}

TSubclassOf<UMainMenuWidget> AEhGameJamGM::GetMainMenuClass() const
{
	return mainMenuClass;
}

TSubclassOf<UPauseGameWidget> AEhGameJamGM::GetPauseMenuClass() const
{
	return pauseMenuClass;
}

void AEhGameJamGM::LoadPlayingLevel()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	if (menuPlayer)
	{
		menuPlayer->DisableInput(playerController);
		playerController->Possess(nullptr);
		
		menuPlayer->Destroy();
		menuPlayer = nullptr;
	}

	// Stream the game level
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnGameLevelLoaded");

	UGameplayStatics::LoadStreamLevel(GetWorld(), FName("GameLevel"), true, false, LatentInfo);
}

void AEhGameJamGM::OnGameLevelLoaded()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	AActor* PlayerStart = nullptr;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() && It->GetLevel()->bIsVisible)
		{
			PlayerStart = *It;
			break;
		}
	}

	if (!PlayerStart) // Don't try to spawn player character
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnPlayer: No valid PlayerStart found in visible levels."));
		return;
	}

	if (!playerGameClass) return;

	FVector SpawnLocation = PlayerStart->GetActorLocation();
	FRotator SpawnRotation = PlayerStart->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	gamePlayer = GetWorld()->SpawnActor<APawn>(playerGameClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (!gamePlayer) return;

	playerController->Possess(gamePlayer);
	gamePlayer->EnableInput(playerController);
}

void AEhGameJamGM::UnloadPlayingLevel()
{
	UGameplayStatics::UnloadStreamLevel(GetWorld(), FName("GameLevel"), FLatentActionInfo(), false);

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	if (gamePlayer)
	{
		gamePlayer->DisableInput(playerController);
		playerController->Possess(nullptr);

		gamePlayer->Destroy();
		gamePlayer = nullptr;
	}

	if (!playerMenuClass) return;

	menuPlayer = GetWorld()->SpawnActor<APawn>(playerMenuClass, FVector::ZeroVector,
		FRotator::ZeroRotator, FActorSpawnParameters());

	if (!menuPlayer) return;

	playerController->Possess(menuPlayer);
	menuPlayer->EnableInput(playerController);
}

void AEhGameJamGM::ShowPlayerHUD()
{
	if (gamePlayer)
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(gamePlayer))
		{
			player->InitializePlayerHUD();
		}
	}
}
