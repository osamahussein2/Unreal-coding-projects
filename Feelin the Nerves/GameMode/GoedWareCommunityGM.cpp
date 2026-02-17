// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GoedWareCommunityGM.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/DefaultPawn.h"
#include "Kismet/GameplayStatics.h"
#include "CineCameraActor.h"
#include "CineCameraComponent.h"

AGoedWareCommunityGM::AGoedWareCommunityGM()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultPawnClass = APawn::StaticClass();
	HUDClass = nullptr;
}

void AGoedWareCommunityGM::BeginPlay()
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (!playerController) return;

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() && It->GetLevel()->bIsVisible)
		{
			AActor* PlayerStart = *It;

			if (PlayerStart)
			{
				PlayerStart->Destroy();
				PlayerStart = nullptr;
			}

			break;
		}
	}

	// Find the cine camera actor somewhere in level
	TArray<AActor*> cameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACineCameraActor::StaticClass(), cameraActors);

	// If a camera actor is found in the level
	if (cameraActors[0])
	{
		// Disable constraint aspect ratio and set the player's view target to the camera actor
		Cast<ACineCameraActor>(cameraActors[0])->GetCineCameraComponent()->SetConstraintAspectRatio(false);
		playerController->SetViewTargetWithBlend(cameraActors[0]);

		cameraActors[0] = nullptr;
	}

	// Show mouse cursor for point and click
	EnableMouseCursor();

	// Push the main menu widget onto the cached widgets array upon booting the game
	PushWidget(mainMenuClass);

	if (gameMusic) UGameplayStatics::PlaySound2D(GetWorld(), gameMusic);
}

void AGoedWareCommunityGM::PushWidget(TSubclassOf<UUserWidget> widgetClass_)
{
	// Only if the widget class if valid
	if (widgetClass_)
	{
		// Add whatever widget subclass is passed in into the cached widgets array
		cachedWidgets.Add(CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), widgetClass_));

		// Set the last index to be the last added element in the cached widgets array
		int lastIndex = cachedWidgets.Num() - 1;

		// If the cached widget at the last pushed index is valid, add it to viewport
		if (cachedWidgets[lastIndex]) cachedWidgets[lastIndex]->AddToViewport();

		// Otherwise, just remove it from the array as we don't want the array to hold an invalid widget
		else cachedWidgets.RemoveAt(lastIndex);
	}
}

void AGoedWareCommunityGM::PopWidget(TSubclassOf<UUserWidget> widgetClass_)
{
	for (int i = 0; i < cachedWidgets.Num(); ++i)
	{
		if (widgetClass_ && cachedWidgets[i]->GetClass() == widgetClass_)
		{
			cachedWidgets[i]->RemoveFromParent();
			cachedWidgets[i] = nullptr;
			
			cachedWidgets.RemoveAt(i);
		}
	}
}

void AGoedWareCommunityGM::EnableMouseCursor()
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

void AGoedWareCommunityGM::DisableMouseCursor()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetInputMode(GameInputMode);

	// Make sure the mouse cursor is turned off
	if (playerController->bShowMouseCursor != false) playerController->bShowMouseCursor = false;
}

int AGoedWareCommunityGM::GetCurrentPlayedSequences() const
{
	return currentPlayedSequences;
}

int AGoedWareCommunityGM::GetMaxPlayedSequences() const
{
	return maxPlayedSequences;
}

int AGoedWareCommunityGM::GetCurrentPickedSequences() const
{
	return currentPickedSequences;
}

TSubclassOf<UMainMenuWidget> AGoedWareCommunityGM::GetMainMenuClass() const
{
	return mainMenuClass;
}

TSubclassOf<UPauseMenuWidget> AGoedWareCommunityGM::GetPauseMenuClass() const
{
	return pauseMenuClass;
}

TSubclassOf<UMM_QuitConfirmationWidget> AGoedWareCommunityGM::GetMM_QuitConfirmationClass() const
{
	return mm_QuitConfirmationClass;
}

TSubclassOf<UPM_QuitConfirmationWidget> AGoedWareCommunityGM::GetPM_QuitConfirmationClass() const
{
	return pm_QuitConfirmationClass;
}

TSubclassOf<UCM_QuitConfirmationWidget> AGoedWareCommunityGM::GetCM_QuitConfirmationClass() const
{
	return cm_QuitConfirmationClass;
}

TSubclassOf<UPlayerHUDWidget> AGoedWareCommunityGM::GetPlayerHUDClass() const
{
	return playerHUDClass;
}

TSubclassOf<UNerveSequenceWidget> AGoedWareCommunityGM::GetNerveSequenceClass() const
{
	return nerveSequenceClass;
}

TSubclassOf<UPickThreeNervesWidget> AGoedWareCommunityGM::GetPickThreeNervesClass() const
{
	return pickThreeNervesClass;
}

TSubclassOf<UPickFourNervesWidget> AGoedWareCommunityGM::GetPickFourNervesClass() const
{
	return pickFourNervesClass;
}

TSubclassOf<UPickFiveNervesWidget> AGoedWareCommunityGM::GetPickFiveNervesClass() const
{
	return pickFiveNervesClass;
}

TSubclassOf<USequenceStatsWidget> AGoedWareCommunityGM::GetSequenceStatsClass() const
{
	return sequenceStatsClass;
}

TSubclassOf<UCompletionWidget> AGoedWareCommunityGM::GetCompletionClass() const
{
	return completionClass;
}

TSubclassOf<UCreditsWidget> AGoedWareCommunityGM::GetCreditsClass() const
{
	return creditsClass;
}

TSubclassOf<USaveGameWidget> AGoedWareCommunityGM::GetSaveGameClass() const
{
	return saveGameClass;
}

TSubclassOf<UPM_LoadGameWidget> AGoedWareCommunityGM::GetPM_LoadGameClass() const
{
	return pm_LoadGameClass;
}

TSubclassOf<UMM_LoadGameWidget> AGoedWareCommunityGM::GetMM_LoadGameClass() const
{
	return mm_LoadGameClass;
}

void AGoedWareCommunityGM::SpawnBodyActor()
{
	if (!bodyActor)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		bodyActor = GetWorld()->SpawnActor<ABodyActor>(bodyActorClass, bodyActorSpawnLocation, bodyActorSpawnRotation, spawnParams);
	}
}

void AGoedWareCommunityGM::DestroyBodyActor()
{
	//DestroyNerveActors();

	if (bodyActor)
	{
		bodyActor->Destroy();
		bodyActor = nullptr;
	}
}

ABodyActor* AGoedWareCommunityGM::GetBodyActor() const
{
	return bodyActor;
}

/*void AGoedWareCommunityGM::SpawnNerveActors()
{
	const int& totalCorrectSequences = maxCorrectSequences * currentCompletedSequences;

	while (nerveIndex < totalCorrectSequences)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn a nerve actor
		nerveActors.Add(GetWorld()->SpawnActor<ANerveActor>(nerveActorClass,
			bodyActorSpawnLocation + FVector(0.0f, 0.0f, 1.0f), bodyActorSpawnRotation, spawnParams));

		// If any of the nerve actors are invalid, just destroy all the nerve actors to prevent errors
		if (!nerveActors[nerveIndex])
		{
			DestroyNerveActors();
			break;
		}
		
		// Set the nerve sprite for nerve actors to be the random nerves index
		if (nerveActors[nerveIndex]->OnSequenceSuccessful.IsBound())
		{
			nerveActors[nerveIndex]->OnSequenceSuccessful.Broadcast(currentCompletedSequences,
				randomNerves[nerveIndex % randomNerves.Num()]);
		}

		// Increment the nerve index once its nerve actor is valid and its sprite is set correctly
		++nerveIndex;

		//UE_LOG(LogTemp, Warning, TEXT("%i, %i"), nerveIndex, nerveIndex % randomNerves.Num());
	}
}

void AGoedWareCommunityGM::DestroyNerveActors()
{
	for (ANerveActor* nerveActor : nerveActors)
	{
		nerveActor->Destroy();
		nerveActor = nullptr;
	}

	if (!nerveActors.IsEmpty()) nerveActors.Empty();
	if (nerveIndex != 0) nerveIndex = 0;
}*/

void AGoedWareCommunityGM::UpdateBodySprite()
{
	if (bodyActor && !bodyActor->OnSequenceSuccessful.IsBound())
	{
		bodyActor->BindOnSequenceSuccessful();
		bodyActor->OnSequenceSuccessful.Broadcast(currentCompletedSequences);
	}
}

int AGoedWareCommunityGM::GetCurrentCorrectSequences() const
{
	return currentCorrectSequences;
}

int AGoedWareCommunityGM::GetMaxCorrectSequences() const
{
	return maxCorrectSequences;
}

int AGoedWareCommunityGM::GetCurrentCompletedSequences() const
{
	return currentCompletedSequences;
}

int AGoedWareCommunityGM::GetMaxCompletedSequences() const
{
	return maxCompletedSequences;
}

TArray<int> AGoedWareCommunityGM::GetRandomNerves() const
{
	return randomNerves;
}

int AGoedWareCommunityGM::GetRandomNervesSize() const
{
	return randomNerves.Num();
}

int AGoedWareCommunityGM::GetRandomNerveIndex(int value_) const
{
	return randomNerves[value_];
}

int AGoedWareCommunityGM::GetReferencedRandomNerveIndex(int& value_) const
{
	return randomNerves[value_];
}

int AGoedWareCommunityGM::FindRandomNerveAtReferencedIndex(int& value_) const
{
	if (randomNerves.IsValidIndex(value_))
	{
		return value_;
	}

	return -1;
}

int AGoedWareCommunityGM::GetBlueNervesTextureNumber(int value_) const
{
	if (blueNerves.IsValidIndex(value_))
	{
		return value_;
	}

	return -1;
}

int AGoedWareCommunityGM::GetOrangeNervesTextureNumber(int value_) const
{
	if (orangeNerves.IsValidIndex(value_))
	{
		return value_;
	}

	return -1;
}

int AGoedWareCommunityGM::GetPurpleNervesTextureNumber(int value_) const
{
	if (purpleNerves.IsValidIndex(value_))
	{
		return value_;
	}

	return -1;
}

int AGoedWareCommunityGM::GetYellowNervesTextureNumber(int value_) const
{
	if (yellowNerves.IsValidIndex(value_))
	{
		return value_;
	}

	return -1;
}

void AGoedWareCommunityGM::AddRandomNerves()
{
	if (!randomNerves.IsEmpty()) randomNerves.Empty();

	int randomizeNerveImage = 0;

	for (int i = randomNerves.Num(); i < maxPlayedSequences; i++)
	{
		switch (currentCompletedSequences)
		{
		case 0:
			randomizeNerveImage = FMath::RandRange(0, blueNerves.Num() - 1);
			break;

		case 1:
			randomizeNerveImage = FMath::RandRange(0, orangeNerves.Num() - 1);
			break;

		case 2:
			randomizeNerveImage = FMath::RandRange(0, purpleNerves.Num() - 1);
			break;

		case 3:
			randomizeNerveImage = FMath::RandRange(0, yellowNerves.Num() - 1);
			break;

		default:
			break;
		}

		// If the random nerves already contains a randomized nerve image index, restart this function
		if (randomNerves.Contains(randomizeNerveImage))
		{
			AddRandomNerves();
			return;
		}

		randomNerves.Add(randomizeNerveImage);
	}
}

void AGoedWareCommunityGM::IncrementPickedSequences()
{
	++currentPickedSequences;
}

void AGoedWareCommunityGM::IncrementPlayedSequences()
{
	++currentPlayedSequences;
}

void AGoedWareCommunityGM::IncrementCorrectSequences()
{
	++currentCorrectSequences;
}

void AGoedWareCommunityGM::IncrementCompletedSequences()
{
	++currentCompletedSequences;
}

void AGoedWareCommunityGM::UpdateValuesBasedOnCompletedSequences()
{
	// Increase max played sequence and correct sequence values
	switch (currentCompletedSequences)
	{
	case 1:
		++maxPlayedSequences;
		++maxCorrectSequences;
		break;

	case 3:
		++maxPlayedSequences;
		++maxCorrectSequences;
		break;

	default:
		break;
	}
}

void AGoedWareCommunityGM::ResetCurrentSequenceStats(bool resetArray_)
{
	currentPlayedSequences = 0;
	currentPickedSequences = 0;
	currentCorrectSequences = 0;
	
	if (resetArray_) AddRandomNerves();
}

void AGoedWareCommunityGM::ResetGame()
{
	if (currentPlayedSequences != 0) currentPlayedSequences = 0;
	if (maxPlayedSequences != 3) maxPlayedSequences = 3;

	if (currentPickedSequences != 0) currentPickedSequences = 0;

	if (currentCorrectSequences != 0) currentCorrectSequences = 0;
	if (maxCorrectSequences != 3) maxCorrectSequences = 3;
	
	if (currentCompletedSequences != 0) currentCompletedSequences = 0;
	if (maxCompletedSequences != 4) maxCompletedSequences = 4;

	if (!randomNerves.IsEmpty()) randomNerves.Empty();
}

int AGoedWareCommunityGM::GetBlueNerveTexturesSize() const
{
	return blueNerves.Num();
}

int AGoedWareCommunityGM::GetOrangeNerveTexturesSize() const
{
	return orangeNerves.Num();
}

int AGoedWareCommunityGM::GetPurpleNerveTexturesSize() const
{
	return purpleNerves.Num();
}

int AGoedWareCommunityGM::GetYellowNerveTexturesSize() const
{
	return yellowNerves.Num();
}

UTexture2D* AGoedWareCommunityGM::GetBlueNerveTextureByIndex(int value_) const
{
	return blueNerves[value_];
}

UTexture2D* AGoedWareCommunityGM::GetOrangeNerveTextureByIndex(int value_) const
{
	return orangeNerves[value_];
}

UTexture2D* AGoedWareCommunityGM::GetPurpleNerveTextureByIndex(int value_) const
{
	return purpleNerves[value_];
}

UTexture2D* AGoedWareCommunityGM::GetYellowNerveTextureByIndex(int value_) const
{
	return yellowNerves[value_];
}

EPickNerveState AGoedWareCommunityGM::GetPickNerveState() const
{
	return pickNerveState;
}

void AGoedWareCommunityGM::SetPickNerveState(EPickNerveState value_)
{
	if (pickNerveState != value_) pickNerveState = value_;
}

void AGoedWareCommunityGM::LoadGame(UPlayerSaveGame* playerSaveGame_)
{
	if (playerSaveGame_)
	{
		currentPlayedSequences = 0;
		currentPickedSequences = 0;
		currentCorrectSequences = 0;

		maxPlayedSequences = playerSaveGame_->GetPlayerMaxPlayedSequences();
		maxCorrectSequences = playerSaveGame_->GetPlayerMaxCorrectSequences();
		currentCompletedSequences = playerSaveGame_->GetPlayerCurrentCompletedSequences();

		randomNerves.Empty();

		for (int i = 0; i < playerSaveGame_->GetSavedRandomNerves().Num(); ++i)
		{
			randomNerves.Add(playerSaveGame_->GetSavedRandomNerveAtReferenceIndex(i));
		}

		//UE_LOG(LogTemp, Warning, TEXT("Max Played Sequences: %i"), playerSaveGame_->GetPlayerMaxPlayedSequences());
		//UE_LOG(LogTemp, Warning, TEXT("Completed Sequences: %i"), playerSaveGame_->GetPlayerCurrentCompletedSequences());

		//UE_LOG(LogTemp, Warning, TEXT("Random Nerve size: %i"), randomNerves.Num());

		PopWidget(mm_LoadGameClass);
		PopWidget(pm_LoadGameClass);
		PushWidget(nerveSequenceClass);

		pickNerveState = EPickNerveState::ENone;
	}
}
