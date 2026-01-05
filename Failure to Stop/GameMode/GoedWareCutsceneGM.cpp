// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/GoedWareCutsceneGM.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Player/CutscenePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Billboard/CreditsBillboard.h"

// Widget include header files
#include "Widgets/MainMenuWidget.h"
#include "Widgets/CutsceneFinishedWidget.h"

// For getting the cutscene/movie include header files
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"

// Niagara include header files
#include "NiagaraActor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

AGoedWareCutsceneGM::AGoedWareCutsceneGM()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
}

void AGoedWareCutsceneGM::BeginPlay()
{
	if (!playerPawnClass) return;

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

	APawn* CachedPlayerCharacter = GetWorld()->SpawnActor<APawn>(playerPawnClass, SpawnLocation,
		SpawnRotation, SpawnParams);

	if (!CachedPlayerCharacter)	return;

	PlayerController->Possess(CachedPlayerCharacter);
	CachedPlayerCharacter->EnableInput(PlayerController);

	cutscenePlayer = Cast<ACutscenePlayer>(CachedPlayerCharacter);

	if (cutscenePlayer.IsValid())
	{
		// Set the cutscene camera inside the player class and set its view target to be the cutscene camera actor
		cutscenePlayer->SetCutsceneCamera();
		PlayerController->SetViewTargetWithBlend(cutscenePlayer->GetCutsceneCamera());

		// Reset camera location and rotation to starting values
		cutscenePlayer->ResetCutsceneCameraTransform(true);
	}

	EnableMouseCursor();
	ShowLoadingWidget(LoadToNextState::BackToMain); // Show the loading widget at start to load main menu

	if (PlayerStart)
	{
		// Destroy player start after player has spawned in the level
		PlayerStart->Destroy();
		PlayerStart = nullptr;
	}

	if (!creditsBillboard)
	{
		TArray<AActor*> foundCreditsBillboard;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACreditsBillboard::StaticClass(), foundCreditsBillboard);

		creditsBillboard = Cast<ACreditsBillboard>(foundCreditsBillboard[0]);
		foundCreditsBillboard[0] = nullptr;
	}
}

void AGoedWareCutsceneGM::Tick(float DeltaTime)
{
	if (snowEffectActor && cutscenePlayer.IsValid() && snowEffectActor->GetActorLocation() != 
		cutscenePlayer->GetCutsceneCamera()->GetActorLocation())
	{
		Super::Tick(DeltaTime);

		snowEffectActor->SetActorLocation(cutscenePlayer->GetCutsceneCamera()->GetActorLocation());
	}
}

void AGoedWareCutsceneGM::PlayCutscene()
{
	if (LevelSequences.IsEmpty() || !GetWorld()) return;

	ALevelSequenceActor* SequenceActor = nullptr;

	// Create a level sequence player
	ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
		LevelSequences[levelSequenceIndex], FMovieSceneSequencePlaybackSettings(), SequenceActor);

	if (LevelSequencePlayer)
	{
		LevelSequencePlayer->Play(); // Play the level sequencer

		// If the level sequence index isn't at the last index, use the level sequence end time
		if (levelSequenceIndex < LevelSequences.Num() - 1)
		{
			FTimerHandle timerHandle;
			GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AGoedWareCutsceneGM::CheckForMoreCutscenes,
				static_cast<const float&>(LevelSequencePlayer->GetEndTime().AsSeconds()), false);
		}

		// If the last level sequence has been reached, change its time to some value less than the level sequence end time
		else
		{
			FTimerHandle timerHandle;
			GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AGoedWareCutsceneGM::CheckForMoreCutscenes,
				static_cast<const float&>(LevelSequencePlayer->GetEndTime().AsSeconds() - 0.1f), false);
		}

		LevelSequencePlayer = nullptr;
	}
}

void AGoedWareCutsceneGM::CheckForMoreCutscenes()
{
	// Find a spawned level sequence
	TArray<AActor*> spawnedSequences;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), spawnedSequences);

	// Destroy it before proceeding to the next cutscene or cutscene finished screen
	if (spawnedSequences[0])
	{
		spawnedSequences[0]->Destroy();
		spawnedSequences[0] = nullptr;
	}

	// If the level sequence index is less than the last index of the level sequences array
	if (levelSequenceIndex < LevelSequences.Num() - 1)
	{
		++levelSequenceIndex; // Increment the index
		watchCutscene.ExecuteIfBound(); // Play the next cutscene

		return; // Don't go past this code to avoid unbinding watch cutscene delegate
	}

	// Set is cutscene playing to false to prevent the player from rotating the cutscene camera around unti cutscenes play again
	SetIsCutscenePlaying(false);

	// Remove the cutscene action tag and unbind watch cutscene delegate once all cutscenes have finished playing
	watchCutscene.Unbind();

	// Show cutscene finished screen
	ShowCutsceneFinished();
}

// Mouse cursor functions
void AGoedWareCutsceneGM::EnableMouseCursor()
{
	// Exclusive to PC
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false); // Don't hide the cursor during capture
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* playerController = Cast<APlayerController>(cutscenePlayer->GetController());
	if (!playerController) return;
	
	playerController->SetInputMode(InputMode);
	
	if (playerController->bShowMouseCursor != true) playerController->bShowMouseCursor = true;
}

void AGoedWareCutsceneGM::DisableMouseCursor()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

	APlayerController* playerController = Cast<APlayerController>(cutscenePlayer->GetController());
	if (!playerController) return;

	playerController->SetInputMode(GameInputMode);

	// Make sure the mouse cursor is turned off
	if (playerController->bShowMouseCursor != false) playerController->bShowMouseCursor = false;
}

// Main Menu widget
void AGoedWareCutsceneGM::ShowMainMenu()
{
	EnableMouseCursor();

	// Bind watch cutscene delegate with the PlayCutscene function if it isn't bound already
	if (!watchCutscene.IsBound()) watchCutscene.BindUFunction(this, "PlayCutscene");

	// Create main menu widget and add it to viewport
	if (mainMenuWidgetClass)
	{
		if (!mainMenuWidget)
		{
			mainMenuWidget = CreateWidget<UMainMenuWidget>(Cast<APlayerController>(cutscenePlayer->GetController()), 
				mainMenuWidgetClass);
		}

		if (!mainMenuWidget) return;

		mainMenuWidget->AddToViewport();
	}
}

void AGoedWareCutsceneGM::HideMainMenu()
{
	// Hide main menu
	if (mainMenuWidget)
	{
		mainMenuWidget->RemoveFromParent();
		mainMenuWidget = nullptr;
	}
}

// Cutscene finished widget
void AGoedWareCutsceneGM::ShowCutsceneFinished()
{
	EnableMouseCursor();

	// Create cutscene finished widget and add it to viewport
	if (cutsceneFinishedWidgetClass)
	{
		if (!cutsceneFinishedWidget)
		{
			cutsceneFinishedWidget = CreateWidget<UCutsceneFinishedWidget>(Cast<APlayerController>(
				cutscenePlayer->GetController()), cutsceneFinishedWidgetClass);
		}

		if (!cutsceneFinishedWidget) return;

		cutsceneFinishedWidget->AddToViewport();
	}
}

void AGoedWareCutsceneGM::HideCutsceneFinished()
{
	// Hide cutscene finished widget and show the main menu
	if (cutsceneFinishedWidget)
	{
		cutsceneFinishedWidget->RemoveFromParent();
		cutsceneFinishedWidget = nullptr;
	}
}

void AGoedWareCutsceneGM::ShowLoadingWidget(LoadToNextState value_)
{
	// Create loading widget and add it to viewport
	if (loadingWidgetClass)
	{
		if (!cutsceneFinishedWidget)
		{
			loadingWidget = CreateWidget<ULoadingWidget>(Cast<APlayerController>(cutscenePlayer->GetController()), 
				loadingWidgetClass);
		}

		if (!loadingWidget) return;

		loadingWidget->SetLoadingToGameState(value_); // Also set its load to next state value to transition to next game state

		if (value_ == LoadToNextState::BackToMain && cutscenePlayer.IsValid())
		{
			// Reset camera location and rotation to starting values
			cutscenePlayer->ResetCutsceneCameraTransform(true);
		}

		loadingWidget->AddToViewport();
	}
}

void AGoedWareCutsceneGM::HideLoadingWidget()
{
	// Hide loading widget and transition to the next game state
	if (loadingWidget)
	{
		loadingWidget->RemoveFromParent();
		TransitionToNextGameState();

		loadingWidget = nullptr;
	}
}

void AGoedWareCutsceneGM::ShowGoBackWidget()
{
	// Create loading widget and add it to viewport
	if (goBackWidgetClass)
	{
		if (!goBackWidget)
		{
			goBackWidget = CreateWidget<UGoBackWidget>(Cast<APlayerController>(cutscenePlayer->GetController()), 
				goBackWidgetClass);
		}

		if (!goBackWidget) return;

		goBackWidget->AddToViewport();
	}
}

void AGoedWareCutsceneGM::HideGoBackWidget()
{
	// Hide go back widget
	if (goBackWidget)
	{
		goBackWidget->RemoveFromParent();
		if (creditsBillboard) creditsBillboard->DestroyCreditsWidget();

		goBackWidget = nullptr;
	}
}

void AGoedWareCutsceneGM::CreateCreditsWidgetComponent()
{
	if (creditsBillboard) creditsBillboard->ConstructCreditsWidget();
}

void AGoedWareCutsceneGM::TransitionToNextGameState()
{
	switch (loadingWidget->GetLoadingToGameState())
	{
	case LoadToNextState::Play: // If the next state is Play

		DisableMouseCursor();

		DestroySnowEffect(); // Destroy snow effect

		SetIsCutscenePlaying(true); // Allow the player to rotate cutscene camera around with input

		cutscenePlayer->SetBlackAreaCameraTransform();

		// Reset back to 1st cutscene if necessary and execute watch cutscene delegate if it's bounded
		if (levelSequenceIndex != 0) levelSequenceIndex = 0;
		watchCutscene.ExecuteIfBound();
		break;

	case LoadToNextState::BackToMain: // If the next state is Main Menu

		HideCutsceneFinished(); // Hide cutscene finished widget if necessary
		ShowMainMenu(); // Show the main menu widget
		SpawnSnowEffect(); // Spawn snow as well
		break;

	default:
		break;
	}
}

void AGoedWareCutsceneGM::SetIsCutscenePlaying(bool value_)
{
	if (cutscenePlayer.IsValid())
	{
		// Set is cutscene playing by passing in a boolean inside the argument
		cutscenePlayer->isCutscenePlaying = value_;

		// If the value passed in is false, reset cutscene camera parameters and disable constrain aspect ratio
		if (!value_)
		{
			cutscenePlayer->GetCutsceneCamera()->SetConstrainAspectRatio(false);
			cutscenePlayer->GetCutsceneCamera()->ResetCutsceneCameraParameters();
		}

		// Otherwise if value is true, constrain the camera's aspect ratio to reveal a cutscene is playing
		else
		{
			cutscenePlayer->GetCutsceneCamera()->SetConstrainAspectRatio(true);
		}
	}
}

void AGoedWareCutsceneGM::SpawnSnowEffect()
{
	if (!snowEffectActor)
	{
		FActorSpawnParameters SpawnParams;

		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the snow effect actor at a set location
		snowEffectActor = GetWorld()->SpawnActor<ANiagaraActor>(ANiagaraActor::StaticClass(), snowSpawnLocation,
			FRotator::ZeroRotator, SpawnParams);

		// Also set the snow effect actor's asset to use the snow effect niagara system if it's valid
		if (snowEffectNS) snowEffectActor->GetNiagaraComponent()->SetAsset(snowEffectNS);
	}
}

void AGoedWareCutsceneGM::DestroySnowEffect()
{
	if (snowEffectActor)
	{
		// Only when the snow effect actor is valid should the actor be destroyed
		snowEffectActor->Destroy();
		snowEffectActor = nullptr;
	}
}