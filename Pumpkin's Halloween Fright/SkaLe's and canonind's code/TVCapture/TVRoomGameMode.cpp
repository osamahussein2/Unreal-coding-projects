// Fill out your copyright notice in the Description page of Project Settings.


#include "TVCapture/TVRoomGameMode.h"
#include "UI/UIManager.h"
#include "TVCapture/TVUIActor.h"
#include "LightAnchorsSystem.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Characters/HJPlayerCharacter.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraComponent.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

ATVRoomGameMode::ATVRoomGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATVRoomGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CachedPlayerCharacter && DynamicPostProcessMaterial && LightAnchorSystemCached)
	{
		float LightRadius = 0;
		FVector LightPosition = LightAnchorSystemCached->CaclulateLightPosition(CachedPlayerCharacter->GetActorLocation(), LightRadius);

		DynamicPostProcessMaterial->SetVectorParameterValue(TEXT("LightPosition"), LightPosition);
		DynamicPostProcessMaterial->SetScalarParameterValue(TEXT("LightOuterRadius"), LightRadius);
	}

}

void ATVRoomGameMode::RestartGame()
{
	CachedUIManager->ClearWidgetStack();
	TVActor->AddStaticImage();
	DestroyCharacter();
	ResetLevels(true);
	SetViewTarget();
	FTimerHandle Restart_TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(Restart_TimerHandle, [this]() {

		StartGame();
		}, 1.f, false);


}

void ATVRoomGameMode::StartGame()
{
	LoadNextLevel();
}

void ATVRoomGameMode::EndGame()
{
	// When pressed Quit from main menu
	OnCassettePulledOut();
}

void ATVRoomGameMode::GoToMainMenu()
{
	bIsPaused = false;
	DestroyCharacter();
	ResetLevels(false);
	SetViewTarget();

	CachedUIManager->ClearWidgetStack();
	CachedUIManager->PushWidget(MainMenuWidgetClass);
	TVActor->AddStaticImage();

}

bool ATVRoomGameMode::LoadNextLevel()
{
	if (LevelsList.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No level names provided for Level List"));
		return false;
	}
	if (LevelsList.Num() <= NextLevelIndex)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not load next level. Level List has no more levels"));
		return false;
	}
	FName NextLevelName = LevelsList[NextLevelIndex++];

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("");
	LatentInfo.UUID = 123;
	LatentInfo.Linkage = 0;

	UGameplayStatics::LoadStreamLevel(this, NextLevelName, true, false, LatentInfo);
	ULevelStreaming* StreamedLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), NextLevelName);
	if (StreamedLevel)
	{
		StreamedLevel->OnLevelShown.AddUniqueDynamic(this, &ATVRoomGameMode::OnTVWorldLoaded);
	}

	return true;
}

void ATVRoomGameMode::ResetLevels(bool bBlock)
{
	for (int32 i = 0; i < LevelsList.Num(); i++)
	{
		UGameplayStatics::UnloadStreamLevel(
			GetWorld(),
			LevelsList[i],
			FLatentActionInfo(),
			bBlock
		);
	}
	NextLevelIndex = 0;
}


void ATVRoomGameMode::PlayEndSequence()
{
	StartEndGameSequence();
}


void ATVRoomGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();



	UE_LOG(LogTemp, Warning, TEXT("ATVRoomGameMode: Game Mode is valid: %s"), *this->GetName());


	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		AHUD* GenericHUD = PC->GetHUD();
		if (!GenericHUD)
		{
			UE_LOG(LogTemp, Warning, TEXT("ATVRoomGameMode: AHUD is invalid"));
			return;
		}
		AUIManager* UIManager = Cast<AUIManager>(GenericHUD);
		if (!UIManager)
		{
			UE_LOG(LogTemp, Warning, TEXT("ATVRoomGameMode: UIManager is invalid"));
			return;
		}
		if (UIManager->bIsInitialized)
		{
			UE_LOG(LogTemp, Warning, TEXT("ATVRoomGameMode::BeginPlay: UIManager is ready. Running OnUIManagerReady..."));

		}
		FTimerHandle UIManagerReady_TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(UIManagerReady_TimerHandle, [this, UIManager]() {OnUIManagerReady(UIManager); }, 0.1f, false);
		CachedUIManager = UIManager;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ATVRoomGameMode: Player controller is invalid"));
	}

	CacheTVActor();

}

void ATVRoomGameMode::OnTVWorldLoaded()
{
	FName LastLoadedLevelName = LevelsList[NextLevelIndex - 1];

	ULevelStreaming* StreamedLevel = UGameplayStatics::GetStreamingLevel(GetWorld(), LastLoadedLevelName);

	if (StreamedLevel)
	{
		StreamedLevel->OnLevelShown.RemoveDynamic(this, &ATVRoomGameMode::OnTVWorldLoaded);
		UE_LOG(LogTemp, Log, TEXT("Successfully unbound OnTVWorldLoaded delegate."));
	}

	if (NextLevelIndex > 1) return; // Next steps are only for the first level

	if (!SpawnPlayer())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn player"));
		return;
	}

	CachedUIManager->ClearWidgetStack();
	SetViewTarget();
	SetCameraReferences();
	CopyCameraSettingsToSceneCapture();
	AttachCaptureActorToCamera();

	FTimerHandle StartLevel_TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StartLevel_TimerHandle, [this]() {TVActor->RemoveStaticImage(); 	InitializeCharacter(); }, StartFirstLevelDelay, false);

}

void ATVRoomGameMode::SetCameraReferences()
{
	UWorld* World = GetWorld();
	if (!World) return;
	TArray<AActor*> FoundActors;


	AHJPlayerCharacter* Player = Cast<AHJPlayerCharacter>(CachedPlayerCharacter);

	if (Player)
	{
		PlayerCameraComponent = Player->GetPlayerCameraComponent();
	}
	FoundActors.Empty();

	// Get scene capture
	UGameplayStatics::GetAllActorsOfClass(World, ASceneCapture2D::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		SceneCaptureActor = FoundActors[0];
	}
	
	if (SceneCaptureActor)
	{
		USceneCaptureComponent2D* SceneCaptureComponent = Cast<USceneCaptureComponent2D>(SceneCaptureActor->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));
		if (SceneCaptureComponent && BasePostProcessMaterial)
		{
			DynamicPostProcessMaterial = UMaterialInstanceDynamic::Create(BasePostProcessMaterial, this);
			if (DynamicPostProcessMaterial)
			{
				FPostProcessSettings Settings = SceneCaptureComponent->PostProcessSettings;

				FWeightedBlendable BlendableItem;
				BlendableItem.Weight = 1.0f;
				BlendableItem.Object = DynamicPostProcessMaterial;

				Settings.WeightedBlendables.Array.Empty();
				Settings.WeightedBlendables.Array.Add(BlendableItem);

				SceneCaptureComponent->PostProcessSettings = Settings;
			}
		}
	}
}

void ATVRoomGameMode::SetViewTarget()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
		{
			if (It->ActorHasTag("TVView"))
			{
				PC->SetViewTargetWithBlend(*It, 0.0f); // instant switch
				break;
			}
		}
	}
}

void ATVRoomGameMode::CopyCameraSettingsToSceneCapture()
{
	if (SceneCaptureActor)
	{
		USceneCaptureComponent2D* SceneCaptureComponent = Cast<USceneCaptureComponent2D>(SceneCaptureActor->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));
		if (SceneCaptureComponent && PlayerCameraComponent)
		{
			SceneCaptureComponentCached = SceneCaptureComponent;
			// Projection
			SceneCaptureComponent->FOVAngle = PlayerCameraComponent->FieldOfView;

			SceneCaptureComponent->ShowFlags.SetTemporalAA(false);
			SceneCaptureComponent->ShowFlags.SetMotionBlur(true);
			SceneCaptureComponent->bAlwaysPersistRenderingState = true;

			AActor* PlayerActor = PlayerCameraComponent->GetOwner();
			if (PlayerActor)
			{
				AHJPlayerCharacter* PlayerCharacter = Cast<AHJPlayerCharacter>(PlayerActor);
				if (PlayerCharacter)
				{
					PlayerCharacter->OnDOFUpdate.AddDynamic(this, &ATVRoomGameMode::UpdateCaptureDOF);
				}
			}
		}
	}


}

void ATVRoomGameMode::AttachCaptureActorToCamera()
{
	USceneCaptureComponent2D* SceneCaptureComponent = Cast<USceneCaptureComponent2D>(SceneCaptureActor->GetComponentByClass(USceneCaptureComponent2D::StaticClass()));

	if (PlayerCameraComponent && SceneCaptureComponent)
	{
		SceneCaptureComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		SceneCaptureComponent->AttachToComponent(
			PlayerCameraComponent,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale
		);
	}
}

void ATVRoomGameMode::CacheTVActor()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATVUIActor::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ATVUIActor* TargetActor = Cast<ATVUIActor>(FoundActors[0]);
		if (TargetActor)
		{
			TVActor = TargetActor;
		}
	}
}

void ATVRoomGameMode::DisplayMainMenu()
{
	if (!MainMenuWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATVRoomGameMode: Main Menu Class is not set!"));
		return;
	}
	UBaseUIWidget* MainMenuWidget = CachedUIManager->PushWidget(MainMenuWidgetClass);
	if (!MainMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATVRoomGameMode: Failed to create Main Menu widget!"));
		return;
	}
}

void ATVRoomGameMode::InitializeCharacter()
{
	if (!HUDWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATVRoomGameMode: HUD Class is not set!"));
		return;
	}

	UBaseUIWidget* HUDWidget = CachedUIManager->PushWidget(HUDWidgetClass);
	if (!HUDWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATVRoomGameMode: Failed to create HUD widget!"));
		return;
	}

	SetPlayerHUD(HUDWidget);
	
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALightAnchorsSystem::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ALightAnchorsSystem* TargetActor = Cast<ALightAnchorsSystem>(FoundActors[0]);
		if (TargetActor)
		{
			LightAnchorSystemCached = TargetActor;
		}
	}
	ActivateInitialAnchors();

	FTimerHandle StartNarration_TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StartNarration_TimerHandle, this, &AHJGameModeBase::OpenDialogueSystem, StartNarrationDelay, false);
}

void ATVRoomGameMode::DestroyCharacter()
{
	if (CachedPlayerCharacter)
	{
		CachedPlayerCharacter->Destroy();
		CachedPlayerCharacter = nullptr;
	}
	if (LightAnchorSystemCached)
	{
		LightAnchorSystemCached->ResetLightAnchorsSections();
		LightAnchorSystemCached = nullptr;
	}
}

bool ATVRoomGameMode::SpawnPlayer()
{
	if (!PlayerCharacterClass) return false;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return false;


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
		return false;
	}

	FVector SpawnLocation = PlayerStart ? PlayerStart->GetActorLocation() : FVector::ZeroVector;
	FRotator SpawnRotation = PlayerStart ? PlayerStart->GetActorRotation() : FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	CachedPlayerCharacter = GetWorld()->SpawnActor<ACharacter>(
		PlayerCharacterClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	if (!CachedPlayerCharacter)	return false;
	PlayerController->Possess(CachedPlayerCharacter);
	CachedPlayerCharacter->EnableInput(PlayerController);

	return true;
}


void ATVRoomGameMode::UpdateCaptureDOF(float TargetLength)
{

	if (!SceneCaptureComponentCached) return;

	FPostProcessSettings& Settings = SceneCaptureComponentCached->PostProcessSettings;

	Settings.bOverride_DepthOfFieldFocalDistance = true;
	Settings.DepthOfFieldFocalDistance = TargetLength;

	// 	Settings.bOverride_DepthOfFieldFstop = true;
	// 	Settings.DepthOfFieldFstop = 0.5f;
	// 
	// 	Settings.bOverride_DepthOfFieldSensorWidth = true;
	// 	Settings.DepthOfFieldSensorWidth = 150.f;
}

void ATVRoomGameMode::OnTVTurnedOn()
{
	TVActor->TurnOn(); // Set image material
	TVActor->AddStaticImage();
	TVActor->AddStaticWhiteSound();

	// Sequence will call OnCassetteInserted
	FTimerHandle InsertCassette_TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(InsertCassette_TimerHandle, this, &ATVRoomGameMode::StartInsertCassetteSequence, CassetteInsertDelay, false);
}

void ATVRoomGameMode::OnTVTurnedOff()
{
	TVActor->TurnOff();
	TVActor->RemoveStaticSound();

	FTimerHandle QuitGame_TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(QuitGame_TimerHandle, this, &ATVRoomGameMode::ExecuteQuitGame, QuitGameDelay, false);
}

void ATVRoomGameMode::ExecuteQuitGame()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		PlayerController,
		EQuitPreference::Quit,
		true
	);
}

void ATVRoomGameMode::OnCassetteInserted()
{
	DisplayMainMenu();
	TVActor->AddStaticVHSSound();
}

void ATVRoomGameMode::OnTVCrashed()
{
	ShowCredits();
}

void ATVRoomGameMode::OnCassettePulledOut()
{
	StartPullOutCassetteSequence();
	CachedUIManager->ClearWidgetStack();
	TVActor->RemoveStaticSound();
	TVActor->AddStaticWhiteSound();
	TVActor->AddStaticImage();

	FTimerHandle TurnOffTV_TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TurnOffTV_TimerHandle, this, &ATVRoomGameMode::OnTVTurnedOff, TVTurnOffDelay, false);
}

void ATVRoomGameMode::OnUIManagerReady(AUIManager* UIManager)
{
	Super::OnUIManagerReady(UIManager);

	UE_LOG(LogTemp, Warning, TEXT("ATVRoomGameMode: Executing OnUIManagerReady callback"));
	FTimerHandle UIManagerReady_Timer;
	GetWorld()->GetTimerManager().SetTimer(UIManagerReady_Timer, [this, UIManager]()
		{
			TVActor->TurnOff(); // Set Off Material
			SetViewTarget();
			FTimerHandle TurnoOnTV_TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TurnoOnTV_TimerHandle, this, &ATVRoomGameMode::OnTVTurnedOn, TVTurnOnDelay, false);
		}, 0.1f, false);



}


