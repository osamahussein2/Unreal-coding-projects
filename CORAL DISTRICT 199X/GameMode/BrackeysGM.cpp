// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/BrackeysGM.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "CineCameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/TouchInterface.h"
#include "Components/AudioComponent.h"
#include "Character/DancingFishNPC.h"

ABrackeysGM::ABrackeysGM()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultPawnClass = nullptr;
	HUDClass = nullptr;

	gameMusic = CreateDefaultSubobject<UAudioComponent>(TEXT("GameMusic"));
	gameMusic->SetupAttachment(GetRootComponent());
}

void ABrackeysGM::BeginPlay()
{
	if (!playerPawnClass) return;

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
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

	FVector SpawnLocation = PlayerStart->GetActorLocation();
	FRotator SpawnRotation = PlayerStart->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* spawnedPlayerCharacter = GetWorld()->SpawnActor<APawn>(playerPawnClass, SpawnLocation, SpawnRotation, SpawnParams);

	startingPosition = SpawnLocation;
	startingRotation = SpawnRotation;

	if (!spawnedPlayerCharacter) return;

	playerController->Possess(spawnedPlayerCharacter);
	spawnedPlayerCharacter->EnableInput(playerController);

	if (gameMusic) gameMusic->SetUISound(true);
	
	if (!isForMobile) EnableMouseCursor();

	PushWidget(mainMenuClass);

	// Destroy player start actor once player character has spawned and can take control of it
	if (PlayerStart)
	{
		PlayerStart->Destroy();
		PlayerStart = nullptr;
	}
	
	SetDrawDistanceForEveryMesh();
}

void ABrackeysGM::SetDrawDistanceForEveryMesh()
{
	TArray<AActor*> staticMeshes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), staticMeshes);

	// Loop through every actor to find its mesh component to set all its draw distance values
	for (AActor* staticMesh : staticMeshes)
	{
		if (staticMesh->FindComponentByClass<UStaticMeshComponent>())
		{
			staticMesh->FindComponentByClass<UStaticMeshComponent>()->MinDrawDistance = 0.0f;
			staticMesh->FindComponentByClass<UStaticMeshComponent>()->LDMaxDrawDistance = maxDrawDistance;
			staticMesh->FindComponentByClass<UStaticMeshComponent>()->CachedMaxDrawDistance = maxDrawDistance;
		}

		if (staticMesh->FindComponentByClass<USkeletalMeshComponent>())
		{
			staticMesh->FindComponentByClass<USkeletalMeshComponent>()->MinDrawDistance = 0.0f;
			staticMesh->FindComponentByClass<USkeletalMeshComponent>()->LDMaxDrawDistance = maxDrawDistance;
			staticMesh->FindComponentByClass<USkeletalMeshComponent>()->CachedMaxDrawDistance = maxDrawDistance;
		}

		staticMesh = nullptr;
	}

	if (!staticMeshes.IsEmpty()) staticMeshes.Empty();
}

void ABrackeysGM::SetDanceBattleDrawDistanceForEveryMesh()
{
	TArray<AActor*> staticMeshes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), staticMeshes);

	// Loop through every actor to find its mesh component to set all its draw distance values
	for (AActor* staticMesh : staticMeshes)
	{
		if (staticMesh->FindComponentByClass<UStaticMeshComponent>())
		{
			staticMesh->FindComponentByClass<UStaticMeshComponent>()->MinDrawDistance = 0.0f;
			staticMesh->FindComponentByClass<UStaticMeshComponent>()->LDMaxDrawDistance = maxDrawDistanceDuringDanceBattle;
			staticMesh->FindComponentByClass<UStaticMeshComponent>()->CachedMaxDrawDistance = maxDrawDistanceDuringDanceBattle;
		}

		if (staticMesh->FindComponentByClass<USkeletalMeshComponent>())
		{
			staticMesh->FindComponentByClass<USkeletalMeshComponent>()->MinDrawDistance = 0.0f;
			staticMesh->FindComponentByClass<USkeletalMeshComponent>()->LDMaxDrawDistance = maxDrawDistanceDuringDanceBattle;
			staticMesh->FindComponentByClass<USkeletalMeshComponent>()->CachedMaxDrawDistance = maxDrawDistanceDuringDanceBattle;
		}

		staticMesh = nullptr;
	}

	if (!staticMeshes.IsEmpty()) staticMeshes.Empty();
}

void ABrackeysGM::EnableMouseCursor()
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

void ABrackeysGM::DisableMouseCursor()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetInputMode(GameInputMode);

	// Make sure the mouse cursor is turned off
	if (playerController->bShowMouseCursor != false) playerController->bShowMouseCursor = false;
}

void ABrackeysGM::PushWidget(TSubclassOf<UUserWidget> widgetClass_)
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

void ABrackeysGM::PopWidget(TSubclassOf<UUserWidget> widgetClass_)
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

TSubclassOf<UMainMenuWidget> ABrackeysGM::GetMainMenuClass() const
{
	return mainMenuClass;
}

TSubclassOf<UPauseMenuWidget> ABrackeysGM::GetPauseMenuClass() const
{
	return pauseMenuClass;
}

TSubclassOf<URhythmMinigameWidget> ABrackeysGM::GetRhythmMinigameClass() const
{
	return rhythmMinigameClass;
}

TSubclassOf<UFinalScoreWidget> ABrackeysGM::GetFinalScoreClass() const
{
	return finalScoreClass;
}

TSubclassOf<URewardWidget> ABrackeysGM::GetRewardClass() const
{
	return rewardClass;
}

TSubclassOf<UCreditsWidget> ABrackeysGM::GetCreditsMenuClass() const
{
	return creditsMenuClass;
}

TSubclassOf<UMainOptionsMenuWidget> ABrackeysGM::GetMainOptionsMenuClass() const
{
	return mainOptionsMenuClass;
}

TSubclassOf<UMobileWidget> ABrackeysGM::GetMobileClass() const
{
	return mobileClass;
}

TSubclassOf<URhythmMinigameMobileWidget> ABrackeysGM::GetRhythmMinigameMobileClass() const
{
	return rhythmMinigameMobileClass;
}

void ABrackeysGM::IncrementPlayerScore(const int& score_)
{
	playerScore += score_;
}

void ABrackeysGM::IncrementEnemyScore(const int& score_)
{
	enemyScore += score_;
}

void ABrackeysGM::ResetGame()
{
	if (playerScore != 0) playerScore = 0;
	if (enemyScore != 0) enemyScore = 0;
	if (rewardNumber != 0) rewardNumber = 0;
	if (lastRewardNumber != 0) lastRewardNumber = 0;

	EndRhythmMinigame();

	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		player->SpawnBoxTriggerForFish();
		player->InvalidateFishActor();
		player->SetActorLocation(startingPosition);
		player->SetActorRotation(startingRotation);
		player->GetController()->SetControlRotation(startingRotation);
		player->ResetPlayerActions();
	}
}

void ABrackeysGM::ResetScores()
{
	if (playerScore != 0) playerScore = 0;
	if (enemyScore != 0) enemyScore = 0;
	if (rewardNumber != lastRewardNumber) rewardNumber = lastRewardNumber;
}

void ABrackeysGM::EarnReward()
{
	if (rewardNumber < rewardActorClass.Num() && rewardActorClass[rewardNumber])
	{
		FVector spawnLocation = FVector(1000.0f, 1000.0f, 1000.0f);
		FRotator spawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ARewardActor* rewardActor = GetWorld()->SpawnActor<ARewardActor>(rewardActorClass[rewardNumber],
			spawnLocation, spawnRotation, spawnParams);

		if (rewardActor)
		{
			++rewardNumber;
			++lastRewardNumber;
		}

		rewardActor = nullptr;
	}
}

void ABrackeysGM::DestroyReward()
{
	for (TActorIterator<ARewardActor> reward(GetWorld()); reward; ++reward)
	{
		reward->Destroy();
	}
}

void ABrackeysGM::StartRhythmMinigame()
{
	SetDanceBattleDrawDistanceForEveryMesh();

	if (!danceCamera)
	{
		FActorSpawnParameters cameraSpawnParams;
		cameraSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		danceCamera = GetWorld()->SpawnActor<ACineCameraActor>(ACineCameraActor::StaticClass(), cameraLocation, 
			cameraRotation, cameraSpawnParams);

		if (!danceCamera) return;

		danceCamera->GetCameraComponent()->SetConstraintAspectRatio(false);

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->SetActorLocation(playerDanceLocation);
			player->SetActorRotation(playerDanceRotation);

			if (APlayerController* playerController = Cast<APlayerController>(player->GetController()))
			{
				//playerController->SetControlRotation(playerDanceRotation);
				playerController->SetViewTargetWithBlend(danceCamera);
			}
		}
	}

	if (!dancingEnemyFish)
	{
		dancingEnemyFish = GetWorld()->SpawnActor<ADancingEnemyFish>(dancingEnemyFishClass, dancingEnemyLocation, 
			dancingEnemyRotation, FActorSpawnParameters());
	}

	if (!dancingNPCFish)
	{
		dancingNPCFish = GetWorld()->SpawnActor<ADancingFishNPC>(dancingNPCFishClass, dancingNPCLocation,
			dancingNPCRotation, FActorSpawnParameters());

		if (!dancingNPCFish) return;

		dancingNPCFish->MakeFishNPCDance();
	}
}

void ABrackeysGM::EndRhythmMinigame()
{
	SetDrawDistanceForEveryMesh();

	if (danceCamera)
	{
		danceCamera->Destroy();
		danceCamera = nullptr;
	}

	if (dancingEnemyFish)
	{
		dancingEnemyFish->Destroy();
		dancingEnemyFish = nullptr;
	}

	if (dancingNPCFish)
	{
		dancingNPCFish->Destroy();
		dancingNPCFish = nullptr;
	}
}

void ABrackeysGM::MakeEnemyDance()
{
	if (dancingEnemyFish)
	{
		dancingEnemyFish->StartDancing();
	}
}

void ABrackeysGM::StopEnemyFromDancing()
{
	if (dancingEnemyFish)
	{
		dancingEnemyFish->StopDancing();
	}
}

void ABrackeysGM::MakeEnemyBreakDance()
{
	if (dancingEnemyFish)
	{
		dancingEnemyFish->BreakDanceOnPlayer();
	}
}

void ABrackeysGM::PlayMenuOpenSound()
{
	if (menuOpenSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), menuOpenSound, sfxVolume);
	}
}

void ABrackeysGM::PlayMenuCloseSound()
{
	if (menuCloseSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), menuCloseSound, sfxVolume);
	}
}

void ABrackeysGM::ShowGameTouchInterface()
{
	if (touchInterface)
	{
		if (APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			playerController->ActivateTouchInterface(touchInterface);
		}
	}
}

void ABrackeysGM::InvalidateTouchInterface()
{
	if (touchInterface)
	{
		if (APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			playerController->ActivateTouchInterface(nullptr);
		}
	}
}

void ABrackeysGM::SetMusicVolume(float& value_)
{
	musicVolume = value_;

	if (musicVolume <= 0.0f) PauseMusic();
	else ResumeMusic();

	if (gameMusic) gameMusic->SetVolumeMultiplier(musicVolume);
}

void ABrackeysGM::PlayMainMenuMusic()
{
	if (gameMusic && mainMenuMusic && gameMusic->GetSound() != mainMenuMusic)
	{
		gameMusic->SetVolumeMultiplier(musicVolume);

		gameMusic->SetSound(mainMenuMusic);
		gameMusic->Play();
	}
}

void ABrackeysGM::PlayCreditsMenuMusic()
{
	if (gameMusic && creditsMenuMusic)
	{
		gameMusic->Stop();

		gameMusic->SetSound(creditsMenuMusic);
		gameMusic->Play();
	}
}

void ABrackeysGM::PlayMallAmbienceMusic()
{
	if (gameMusic && mallAmbienceMusic && gameMusic->GetSound() != mallAmbienceMusic)
	{
		gameMusic->Stop();
		gameMusic->SetVolumeMultiplier(musicVolume * mallAmbienceVolumeMultiplier);

		gameMusic->SetSound(mallAmbienceMusic);
		gameMusic->Play();
	}
}

void ABrackeysGM::PlayBattleMusic()
{
	const int& randomizedBattleMusicIndex = FMath::RandRange(0, battleMusics.Num() - 1);

	if (gameMusic && battleMusics[randomizedBattleMusicIndex] && 
		gameMusic->GetSound() != battleMusics[randomizedBattleMusicIndex])
	{
		gameMusic->Stop();
		gameMusic->SetVolumeMultiplier(musicVolume);

		gameMusic->SetSound(battleMusics[randomizedBattleMusicIndex]);
		gameMusic->Play();
	}
}

void ABrackeysGM::ResumeMusic()
{
	if (gameMusic)
	{
		gameMusic->SetPaused(false);
	}
}

void ABrackeysGM::PauseMusic()
{
	if (gameMusic)
	{
		gameMusic->SetPaused(true);
	}
}
