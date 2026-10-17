// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Object/CameraSnapshot.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/CanvasPanel.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SceneView.h"
#include "Object/Sign.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Object/WaypointActor.h"
#include "GameInstance/ComfyAutumnGI.h"
#include "Metasound/CameraShutterMetasound.h"
#include "Kismet/GameplayStatics.h"
#include "Object/ThreeLaneSign.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	actorsToIgnore.Add(this);
}

void APlayerCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (hitSign) hitSign = nullptr;

	if (waypointActor)
	{
		waypointActor->Destroy();
		waypointActor = nullptr;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!inputMappingContext) return;

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (!playerController->GetLocalPlayer()) return;

		// Get the local player enhanced input subsystem
		UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			playerController->GetLocalPlayer());

		// Add the input mapping context
		enhancedInputSubsystem->AddMappingContext(inputMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind move action to the enhanced input component
		if (moveAction) EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		// Bind look action to the enhanced input component
		if (lookAction) EnhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookAround);

		// Bind use camera action to the enhanced input component
		if (useCameraAction) EnhancedInputComponent->BindAction(useCameraAction, ETriggerEvent::Started, this, &APlayerCharacter::UseCamera);

		// Bind use photo gallery action to the enhanced input component
		if (photoGalleryAction) EnhancedInputComponent->BindAction(photoGalleryAction, ETriggerEvent::Started, this, &APlayerCharacter::CheckPhotoGallery);

		// Bind use pause menu action to the enhanced input component
		if (pauseGameAction) EnhancedInputComponent->BindAction(pauseGameAction, ETriggerEvent::Started, this, &APlayerCharacter::PauseGame);
	}
}

void APlayerCharacter::CollectFilm()
{
	++filmsCollected;

	if (OnFilmCollected.IsBound()) OnFilmCollected.Broadcast(filmsCollected);

	// First time collecting a film, show sign changed tutorial
	if (filmsCollected == 1) OpenSignChangedTutorialPopup();
}

bool APlayerCharacter::SignInViewOfPlayerCamera(ASign* referencedSign_) const
{
	if (!referencedSign_) return false;

	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (!playerController || !playerController->PlayerCameraManager) return false;

	FVector directionToSign = (referencedSign_->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	float DotProduct = FVector::DotProduct(playerController->PlayerCameraManager->GetActorForwardVector(), directionToSign);

	// Check if the player is looking at the referenced sign, return true here
	if (DotProduct >= 0.0f) return true;

	return false;
}

bool APlayerCharacter::SignInViewOfPlayerCamera(AThreeLaneSign* referencedSign_) const
{
	if (!referencedSign_) return false;

	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (!playerController || !playerController->PlayerCameraManager) return false;

	FVector directionToSign = (referencedSign_->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	float DotProduct = FVector::DotProduct(playerController->PlayerCameraManager->GetActorForwardVector(), directionToSign);

	// Check if the player is looking at the referenced sign, return true here
	if (DotProduct >= 0.0f) return true;

	return false;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (currentState == ECurrentState::TakingPhoto || currentState == ECurrentState::CheckingOutPhotoGallery) return;

	AddMovementInput(GetActorForwardVector(), Value.Get<FVector2D>().Y);
	AddMovementInput(GetActorRightVector(), Value.Get<FVector2D>().X);

	if (waypointActor) waypointActor->UpdateWaypoint(this);
}

void APlayerCharacter::LookAround(const FInputActionValue& Value)
{
	if (currentState == ECurrentState::CheckingOutPhotoGallery) return;

	AddControllerPitchInput(cameraSensitivity * -Value.Get<FVector2D>().Y);
	AddControllerYawInput(cameraSensitivity * Value.Get<FVector2D>().X);

	if (cameraSnapshotComponent)
	{
		cameraSnapshotComponent->SetWorldRotation(GetControlRotation());

		if (OnCameraMoved.IsBound()) OnCameraMoved.Broadcast();
	}
}

void APlayerCharacter::UseCamera()
{
	if (currentState != ECurrentState::CanTakePhoto && currentState != ECurrentState::TakingPhoto) return;

	if (!cameraSnapshotComponent)
	{
		if (!IsValid(cameraSnapshotClass)) return;

		cameraSnapshotComponent = NewObject<UCameraSnapshot>(this, cameraSnapshotClass, TEXT("CameraSnapshotComponent"));
		if (!cameraSnapshotComponent) return;

		if (GetCapsuleComponent()) cameraSnapshotComponent->SetupAttachment(GetCapsuleComponent());

		// Register the created component during runtime
		cameraSnapshotComponent->RegisterComponent();
		cameraSnapshotComponent->SetWorldRotation(GetControlRotation());

		HideHUD();

		if (!IsValid(takeSnapshotWidgetClass) || takeSnapshotWidget) return;

		if (APlayerController* playerController = Cast<APlayerController>(GetController()))
		{
			takeSnapshotWidget = CreateWidget<UUserWidget>(playerController, takeSnapshotWidgetClass);
			if (!takeSnapshotWidget) return;

			takeSnapshotWidget->AddToViewport();
		}

		currentState = ECurrentState::TakingPhoto;
	}

	else
	{
		if (takeSnapshotWidget)
		{
			takeSnapshotWidget->RemoveFromParent();
			takeSnapshotWidget = nullptr;
		}

		// If the player took a picture of the right sign, update sign picture taken status
		if (SignInViewOfPicture()) UpdateSignPictureTakenStatus();

		// Otherwise player didn't take a picture of the right sign, just release render target and show HUD
		else CancelTakingPhoto();

		cameraSnapshotComponent->UnregisterComponent();
		cameraSnapshotComponent->DestroyComponent();
		cameraSnapshotComponent = nullptr;
	}
}

void APlayerCharacter::CheckPhotoGallery()
{
	if (currentState == ECurrentState::TakingPhoto) return;

	if (!photoGalleryWidget)
	{
		if (photoGalleryInfos.Num() <= currentSnapshotIndex || 
			!IsValid(photoGalleryInfos[currentSnapshotIndex].photoGalleryWidgetClass)) return;

		HideHUD();

		if (APlayerController* playerController = Cast<APlayerController>(GetController()))
		{
			photoGalleryWidget = CreateWidget<UUserWidget>(playerController, photoGalleryInfos[currentSnapshotIndex].photoGalleryWidgetClass);
			if (!photoGalleryWidget) return;

			photoGalleryWidget->AddToViewport();

			currentState = ECurrentState::CheckingOutPhotoGallery;
		}
	}

	else
	{
		// If there are the maximum number correct snapshots taken, release the render target 2Ds and empty the array to take more snapshots
		if (photoGalleryInfos.Num() > previousSnapshotIndex)
		{
			if (snapshotRenderTargets.Num() >= photoGalleryInfos[previousSnapshotIndex].maxSnapshots)
			{
				for (int i = 0; i < snapshotRenderTargets.Num(); ++i)
				{
					UKismetRenderingLibrary::ReleaseRenderTarget2D(snapshotRenderTargets[i]);
					snapshotRenderTargets[i] = nullptr;
				}

				snapshotRenderTargets.Empty();

				if (previousSnapshotIndex < photoGalleryInfos.Num()) ++previousSnapshotIndex;
			}
		}

		if (previousState == ECurrentState::CanTakePhoto) currentState = ECurrentState::CanTakePhoto;
		else currentState = ECurrentState::Exploration;

		photoGalleryWidget->RemoveFromParent();
		photoGalleryWidget = nullptr;

		ShowHUD(true);
	}
}

void APlayerCharacter::PauseGame()
{
	if (!UGameplayStatics::IsGamePaused(this)) ShowPauseMenu();
	else DestroyPauseMenu(true, true);
}

void APlayerCharacter::ShowHUD(bool triggerCameraTutorialPopup_)
{
	if (!IsValid(hudWidgetClass) || hudWidget) return;

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		hudWidget = CreateWidget<UUserWidget>(playerController, hudWidgetClass);
		if (!hudWidget) return;

		hudWidget->AddToViewport();

		if (!triggerCameraTutorialPopup_) return;

		if (GetWorld())
		{
			FTimerHandle timerHandle{};
			GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &APlayerCharacter::UpdateCanTakePhotoLogic, 0.1f, false);
		}
	}
}

void APlayerCharacter::HideHUD()
{
	if (!hudWidget) return;

	hudWidget->RemoveFromParent();
	hudWidget = nullptr;
}

void APlayerCharacter::OpenPhotoGallery()
{
	if (photoGalleryInfos.Num() <= currentSnapshotIndex ||
		!IsValid(photoGalleryInfos[currentSnapshotIndex].photoGalleryWidgetClass) ||
		photoGalleryWidget) return;

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		photoGalleryWidget = CreateWidget<UUserWidget>(playerController, photoGalleryInfos[currentSnapshotIndex].photoGalleryWidgetClass);
		if (!photoGalleryWidget) return;

		photoGalleryWidget->AddToViewport();

		currentState = ECurrentState::CheckingOutPhotoGallery;
	}
}

void APlayerCharacter::ShowPauseMenu()
{
	switch (currentState)
	{
	case ECurrentState::Exploration:
		HideHUD();
		break;

	case ECurrentState::CanTakePhoto:
		HideHUD();
		break;

	case ECurrentState::TakingPhoto:
		break;

	case ECurrentState::CheckingOutPhotoGallery:
		if (photoGalleryWidget)
		{
			photoGalleryWidget->RemoveFromParent();
			photoGalleryWidget = nullptr;
		}
		break;

	default:
		break;
	}

	if (!IsValid(pauseWidgetClass)) return;

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		pauseWidget = CreateWidget<UUserWidget>(playerController, pauseWidgetClass);
		if (!pauseWidget) return;

		pauseWidget->AddToViewport();
	}
}

void APlayerCharacter::DestroyPauseMenu(bool captureMouse_, bool showInGameWidgets_)
{
	if (pauseWidget)
	{
		pauseWidget->RemoveFromParent();
		pauseWidget = nullptr;
	}

	if (captureMouse_)
	{
		FInputModeGameOnly GameInputMode;
		GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

		if (APlayerController* playerController = Cast<APlayerController>(GetController()))
		{
			playerController->SetInputMode(GameInputMode);

			// Make sure the mouse cursor is turned off
			if (playerController->bShowMouseCursor != false) playerController->bShowMouseCursor = false;
		}
	}

	UGameplayStatics::SetGamePaused(this, false);

	if (showInGameWidgets_)
	{
		switch (currentState)
		{
		case ECurrentState::Exploration:
			ShowHUD(false);
			break;

		case ECurrentState::CanTakePhoto:
			ShowHUD(true);
			break;

		case ECurrentState::TakingPhoto:
			break;

		case ECurrentState::CheckingOutPhotoGallery:
			if (photoGalleryInfos.Num() <= currentSnapshotIndex ||
				!IsValid(photoGalleryInfos[currentSnapshotIndex].photoGalleryWidgetClass) ||
				photoGalleryWidget) return;

			if (APlayerController* playerController = Cast<APlayerController>(GetController()))
			{
				photoGalleryWidget = CreateWidget<UUserWidget>(playerController, photoGalleryInfos[currentSnapshotIndex].photoGalleryWidgetClass);
				if (!photoGalleryWidget) return;

				photoGalleryWidget->AddToViewport();

				currentState = ECurrentState::CheckingOutPhotoGallery;
			}
			break;

		default:
			break;
		}
	}
}

void APlayerCharacter::ShowLevelCompleted()
{
	HideHUD();

	if (takeSnapshotWidget)
	{
		takeSnapshotWidget->RemoveFromParent();
		takeSnapshotWidget = nullptr;
	}

	if (photoGalleryWidget)
	{
		photoGalleryWidget->RemoveFromParent();
		photoGalleryWidget = nullptr;
	}

	if (!IsValid(levelCompletedWidgetClass) || levelCompletedWidget) return;

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		levelCompletedWidget = CreateWidget<UUserWidget>(playerController, levelCompletedWidgetClass);
		if (!levelCompletedWidget) return;

		levelCompletedWidget->AddToViewport();
	}
}

void APlayerCharacter::DestroyLevelCompleted()
{
	if (!levelCompletedWidget) return;

	levelCompletedWidget->RemoveFromParent();
	levelCompletedWidget = nullptr;
}

bool APlayerCharacter::SignInViewOfPicture() const
{
	if (hitSign) hitSign = nullptr;
	if (hitThreeLaneSign) hitThreeLaneSign = nullptr;

	if (photoGalleryInfos.Num() <= currentSnapshotIndex) return false;

	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (!playerController || !playerController->PlayerCameraManager) return false;

	FHitResult hitResult{};

	EDrawDebugTrace::Type drawDebugTrace = EDrawDebugTrace::None;

	// Shows the sphere trace only in development build or in editor
#if UE_BUILD_DEVELOPMENT
	drawDebugTrace = EDrawDebugTrace::ForOneFrame;
#endif

	switch (photoGalleryInfos[currentSnapshotIndex].maxSnapshots)
	{
	case 2:

		if (UKismetSystemLibrary::SphereTraceSingle(this, playerController->PlayerCameraManager->GetCameraLocation(),
			playerController->PlayerCameraManager->GetCameraLocation() +
			playerController->PlayerCameraManager->GetActorForwardVector() * cameraDetectionTraceLength,
			cameraDetectionRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore,
			drawDebugTrace, hitResult, true))
		{
			for (TActorIterator<ASign> signIt(GetWorld()); signIt; ++signIt)
			{
				// If the sign's snapshot index doesn't match the current snapshot index, continue iterating
				if (signIt->GetSnapshotIndex() != currentSnapshotIndex) continue;

				// If the sign's status is set to took updated picture of it, continue iterating
				if (signIt->GetSignStatus() == ETakenPictureOfSignStatus::ETookUpdatedPictureOfIt) continue;

				// If the current snapshot index of the current sign sign status is set to took initial picture of it, continue iterating
				if (signIt->GetSnapshotIndex() == currentSnapshotIndex &&
					signIt->GetSignStatus() == ETakenPictureOfSignStatus::ETookInitialPictureOfIt) continue;

				// We arrived at the right sign to take a picture of
				if (hitResult.GetActor() == *signIt)
				{
					hitSign = *signIt;
					return true;
				}
			}
		}

		break;

	case 3:

		if (UKismetSystemLibrary::SphereTraceSingle(this, playerController->PlayerCameraManager->GetCameraLocation(),
			playerController->PlayerCameraManager->GetCameraLocation() +
			playerController->PlayerCameraManager->GetActorForwardVector() * cameraDetectionTraceLength,
			cameraDetectionRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, actorsToIgnore,
			drawDebugTrace, hitResult, true))
		{
			for (TActorIterator<AThreeLaneSign> signIt(GetWorld()); signIt; ++signIt)
			{
				// If the sign's snapshot index doesn't match the current snapshot index, continue iterating
				if (signIt->GetSnapshotIndex() != currentSnapshotIndex) continue;

				// If the sign's status is set to took updated picture of it, continue iterating
				if (signIt->GetSignStatus() == EThreeLaneSignStatus::ETakenThirdPicture) continue;

				// If the current snapshot index of the current sign sign status is set to took initial picture of it, continue iterating
				if (signIt->GetSnapshotIndex() == currentSnapshotIndex &&
					signIt->GetSignStatus() == EThreeLaneSignStatus::ETakenFirstPicture) continue;

				if (signIt->GetSnapshotIndex() == currentSnapshotIndex &&
					signIt->GetSignStatus() == EThreeLaneSignStatus::ETakenSecondPicture) continue;

				// We arrived at the right sign to take a picture of
				if (hitResult.GetActor() == *signIt)
				{
					hitThreeLaneSign = *signIt;
					return true;
				}
			}
		}

		break;

	default:
		break;
	}

	return false;
}

void APlayerCharacter::UpdateSignPictureTakenStatus()
{
	if (!cameraSnapshotComponent || !GetWorld())
	{
		CancelTakingPhoto();
		return;
	}

	if (!hitSign && !hitThreeLaneSign)
	{
		CancelTakingPhoto();
		return;
	}

	if (hitSign)
	{
		switch (hitSign->GetSignStatus())
		{
		case ETakenPictureOfSignStatus::ENotTakenPictureOfIt:

			// If the current sign status is set to not taken picture of it, set it to taken initial picture of it
			hitSign->SetInitialPictureTaken();

			OpenPhotoGalleryTutorialPopup();
			SpawnCameraShutterMetasound();

			if (previousState == ECurrentState::CanTakePhoto) currentState = ECurrentState::CanTakePhoto;
			else currentState = ECurrentState::Exploration;

			if (!waypointActor)
			{
				if (!hitSign->GetWaypointClass()) return;

				waypointActor = GetWorld()->SpawnActor<AWaypointActor>(hitSign->GetWaypointClass(), FTransform(), FActorSpawnParameters());
				if (!waypointActor) return;

				waypointActor->InitializeWaypointLocations(this, hitSign->GetInitialWaypointLocation());
			}

			else
			{
				waypointActor->InitializeWaypointLocations(this, hitSign->GetInitialWaypointLocation());
			}

			snapshotRenderTargets.Add(cameraSnapshotComponent->GetTextureRenderTarget2D());

			break;

		case ETakenPictureOfSignStatus::ESignChanged:

			// If the current sign status is set to sign changed, set it to taken updated picture of it
			hitSign->SetUpdatedPictureTaken();
			snapshotRenderTargets.Add(cameraSnapshotComponent->GetTextureRenderTarget2D());

			SpawnCameraShutterMetasound();

			if (!waypointActor)
			{
				if (!hitSign->GetWaypointClass()) return;

				waypointActor = GetWorld()->SpawnActor<AWaypointActor>(hitSign->GetWaypointClass(), FTransform(), FActorSpawnParameters());
				if (!waypointActor) return;

				waypointActor->InitializeWaypointLocations(this, hitSign->GetUpdatedWaypointLocation());
			}

			else
			{
				waypointActor->InitializeWaypointLocations(this, hitSign->GetUpdatedWaypointLocation());
			}

			OpenPhotoGallery();

			++currentSnapshotIndex; // Increment current snapshot index to progress taking picture of the next sign
			break;

		default:
			CancelTakingPhoto();
			break;
		}

		hitSign = nullptr;
	}

	if (hitThreeLaneSign)
	{
		switch (hitThreeLaneSign->GetSignStatus())
		{
		case EThreeLaneSignStatus::ENotTakenPictureOfIt:

			// If the current sign status is set to not taken picture of it, set it to taken initial picture of it
			hitThreeLaneSign->SetTakenFirstPicture();

			SpawnCameraShutterMetasound();

			if (previousState == ECurrentState::CanTakePhoto) currentState = ECurrentState::CanTakePhoto;
			else currentState = ECurrentState::Exploration;

			if (!waypointActor)
			{
				if (!hitThreeLaneSign->GetWaypointClass()) return;

				waypointActor = GetWorld()->SpawnActor<AWaypointActor>(hitThreeLaneSign->GetWaypointClass(), FTransform(), FActorSpawnParameters());
				if (!waypointActor) return;

				waypointActor->InitializeWaypointLocations(this, hitThreeLaneSign->GetInitialWaypointLocation());
			}

			else
			{
				waypointActor->InitializeWaypointLocations(this, hitThreeLaneSign->GetInitialWaypointLocation());
			}

			snapshotRenderTargets.Add(cameraSnapshotComponent->GetTextureRenderTarget2D());
			ShowHUD(true);

			break;

		case EThreeLaneSignStatus::ESignChangedOnce:

			// If the current sign status is set to sign changed once, set it to taken second picture of it
			hitThreeLaneSign->SetTakenSecondPicture();

			SpawnCameraShutterMetasound();

			if (previousState == ECurrentState::CanTakePhoto) currentState = ECurrentState::CanTakePhoto;
			else currentState = ECurrentState::Exploration;

			if (!waypointActor)
			{
				if (!hitThreeLaneSign->GetWaypointClass()) return;

				waypointActor = GetWorld()->SpawnActor<AWaypointActor>(hitThreeLaneSign->GetWaypointClass(), FTransform(), FActorSpawnParameters());
				if (!waypointActor) return;

				waypointActor->InitializeWaypointLocations(this, hitThreeLaneSign->GetSecondWaypointLocation());
			}

			else
			{
				waypointActor->InitializeWaypointLocations(this, hitThreeLaneSign->GetSecondWaypointLocation());
			}

			snapshotRenderTargets.Add(cameraSnapshotComponent->GetTextureRenderTarget2D());
			ShowHUD(true);

			break;

		case EThreeLaneSignStatus::ESignChangedTwice:

			// If the current sign status is set to sign changed, set it to taken updated picture of it
			hitThreeLaneSign->SetTakenThirdPicture();
			snapshotRenderTargets.Add(cameraSnapshotComponent->GetTextureRenderTarget2D());

			SpawnCameraShutterMetasound();

			if (!waypointActor)
			{
				if (!hitThreeLaneSign->GetWaypointClass()) return;

				waypointActor = GetWorld()->SpawnActor<AWaypointActor>(hitThreeLaneSign->GetWaypointClass(), FTransform(), FActorSpawnParameters());
				if (!waypointActor) return;

				waypointActor->InitializeWaypointLocations(this, hitThreeLaneSign->GetThirdWaypointLocation());
			}

			else
			{
				waypointActor->InitializeWaypointLocations(this, hitThreeLaneSign->GetThirdWaypointLocation());
			}

			OpenPhotoGallery();

			++currentSnapshotIndex; // Increment current snapshot index to progress taking picture of the next sign

			break;

		default:
			CancelTakingPhoto();
			break;
		}

		hitThreeLaneSign = nullptr;
	}

	// Show the most recent snapshot on the HUD after taking a picture of the right sign only if the delegate is bound to an event
	if (OnSnapshotCorrectlyTaken.IsBound())
	{
		if (!GetWorld() || !GetCurrentSnapshotRenderTarget()) return;

		// Need to wait for a few seconds before snapshot will appear on the HUD
		FTimerHandle timer{};
		GetWorld()->GetTimerManager().SetTimer(timer, [this]()
			{
				if (OnSnapshotCorrectlyTaken.IsBound() && GetCurrentSnapshotRenderTarget())
				{
					OnSnapshotCorrectlyTaken.Broadcast(GetCurrentSnapshotRenderTarget());
				}
			}, 0.1f, false);
	}
}

void APlayerCharacter::CancelTakingPhoto()
{
	if (previousState == ECurrentState::CanTakePhoto) currentState = ECurrentState::CanTakePhoto;
	else currentState = ECurrentState::Exploration;

	if (cameraSnapshotComponent) cameraSnapshotComponent->ReleaseRenderTarget();
	ShowHUD(true);
}

UTextureRenderTarget2D* APlayerCharacter::GetCurrentSnapshotRenderTarget() const
{
	if (snapshotRenderTargets.IsEmpty()) return nullptr;

	// Get the most recent snapshot render target
	return snapshotRenderTargets[snapshotRenderTargets.Num() - 1];
}

UTextureRenderTarget2D* APlayerCharacter::GetSnapshotRenderTargetAtIndex(int index_) const
{
	if (snapshotRenderTargets.Num() <= index_) return nullptr;

	// Get snapshot render target at index
	return snapshotRenderTargets[index_];
}

void APlayerCharacter::UpdateCanTakePhotoLogic()
{
	if (currentState == ECurrentState::CanTakePhoto)
	{
		if (OnAbleToTakePicture.IsBound()) OnAbleToTakePicture.Broadcast();
	}

	else
	{
		if (OnUnableToTakePicture.IsBound()) OnUnableToTakePicture.Broadcast();
	}
}

void APlayerCharacter::OpenTutorialIntroductionPopup()
{
	if (UComfyAutumnGI* gameInstance = Cast<UComfyAutumnGI>(GetGameInstance()))
	{
		switch (gameInstance->GetTutorialState())
		{
		case ETutorialState::NotGiven:
		{
			// Disable any player input first
			if (APlayerController* playerController = Cast<APlayerController>(GetController()))
			{
				DisableInput(playerController);
			}

			ShowHUD(false);

			// Show the tutorial introduction texts only
			if (GetWorld())
			{
				FTimerHandle timerHandle{};
				GetWorld()->GetTimerManager().SetTimer(timerHandle, [this, gameInstance]()
					{
						if (OnTutorialPopup.IsBound()) OnTutorialPopup.Broadcast();
						if (gameInstance) gameInstance->PopupTutorial();
					}, 0.1f, false);
			}

			break;
		}

		default:
			ShowHUD(true);
			break;
		}
	}
}

void APlayerCharacter::OpenPhotoGalleryTutorialPopup()
{
	if (UComfyAutumnGI* gameInstance = Cast<UComfyAutumnGI>(GetGameInstance()))
	{
		switch (gameInstance->GetTutorialState())
		{
		case ETutorialState::PhotoGallery:
		{
			// Disable any player input first
			if (APlayerController* playerController = Cast<APlayerController>(GetController()))
			{
				DisableInput(playerController);
			}

			ShowHUD(false);

			// Show the photo gallery tutorial texts only
			if (GetWorld())
			{
				FTimerHandle timerHandle{};
				GetWorld()->GetTimerManager().SetTimer(timerHandle, [this, gameInstance]()
					{
						if (OnTutorialPopup.IsBound()) OnTutorialPopup.Broadcast();
					}, 0.1f, false);
			}

			break;
		}

		default:
			ShowHUD(true);
			break;
		}
	}
}

void APlayerCharacter::OpenSignChangedTutorialPopup()
{
	if (UComfyAutumnGI* gameInstance = Cast<UComfyAutumnGI>(GetGameInstance()))
	{
		switch (gameInstance->GetTutorialState())
		{
		case ETutorialState::SignChanged:
		{
			// Disable any player input first
			if (APlayerController* playerController = Cast<APlayerController>(GetController()))
			{
				DisableInput(playerController);
			}

			ShowHUD(false);

			// Show the sign changed tutorial texts only
			if (GetWorld())
			{
				FTimerHandle timerHandle{};
				GetWorld()->GetTimerManager().SetTimer(timerHandle, [this, gameInstance]()
					{
						if (OnTutorialPopup.IsBound()) OnTutorialPopup.Broadcast();
					}, 0.1f, false);
			}

			break;
		}

		default:
			ShowHUD(true);
			break;
		}
	}
}

void APlayerCharacter::SpawnCameraShutterMetasound()
{
	if (!IsValid(cameraShutterMetasoundClass) || !GetWorld()) return;
	
	ACameraShutterMetasound* cameraShutterMetasound = GetWorld()->SpawnActor<ACameraShutterMetasound>(cameraShutterMetasoundClass,
		FTransform(), FActorSpawnParameters());
}

void APlayerCharacter::DestroyWaypoint()
{
	if (!waypointActor) return;

	waypointActor->Destroy();
	waypointActor = nullptr;
}

void APlayerCharacter::SetTakingPhoto(ECurrentState value_)
{
	if (previousState != value_) previousState = value_;
	if (currentState != value_) currentState = value_;

	UpdateCanTakePhotoLogic();
}
