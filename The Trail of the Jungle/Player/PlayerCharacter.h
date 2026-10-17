// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enum/CurrentState.h"
#include "Structure/PhotoGalleryInfo.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCameraSnapshot;
class ASign;
class AWaypointActor;
class ACameraShutterMetasound;
class AThreeLaneSign;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFilmCollected, int, filmCollected_);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraMoved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSnapshotCorrectlyTaken, UTextureRenderTarget2D*, currentSnapshotTarget_);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbleToTakePicture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnableToTakePicture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTutorialPopup);

UCLASS()
class COMFYAUTUMNJAM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CollectFilm();

	int GetCurrentSnapshotIndex() const { return currentSnapshotIndex; }

	bool SignInViewOfPlayerCamera(ASign* referencedSign_) const;
	bool SignInViewOfPlayerCamera(AThreeLaneSign* referencedSign_) const;

	void ShowLevelCompleted();

	void DestroyWaypoint();

	void SetTakingPhoto(ECurrentState value_);

	void OpenTutorialIntroductionPopup();

	void ShowHUD(bool triggerCameraTutorialPopup_);
	void HideHUD();

	ECurrentState GetCurrentState() const { return currentState; }

private:
	void Move(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);
	void UseCamera();
	void CheckPhotoGallery();
	void PauseGame();

	void OpenPhotoGallery();

	// Pause Menu functions
	void ShowPauseMenu();

	UFUNCTION(BlueprintCallable)
	void DestroyPauseMenu(bool captureMouse_, bool showInGameWidgets_);

	UFUNCTION(BlueprintCallable)
	void DestroyLevelCompleted();

	UFUNCTION(BlueprintPure)
	bool SignInViewOfPicture() const;

	UFUNCTION(BlueprintPure)
	bool GetCanTakePhoto() const { return currentState == ECurrentState::CanTakePhoto; }

	void UpdateSignPictureTakenStatus();
	void CancelTakingPhoto();

	UFUNCTION(BlueprintPure)
	UTextureRenderTarget2D* GetCurrentSnapshotRenderTarget() const;

	UFUNCTION(BlueprintPure)
	UTextureRenderTarget2D* GetSnapshotRenderTargetAtIndex(int index_) const;

	UFUNCTION()
	void UpdateCanTakePhotoLogic();

	void OpenPhotoGalleryTutorialPopup();
	void OpenSignChangedTutorialPopup();

	void SpawnCameraShutterMetasound();

private:
	// Component
	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraSnapshot> cameraSnapshotComponent = nullptr;

	// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> inputMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> moveAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> lookAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> useCameraAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> photoGalleryAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> pauseGameAction = nullptr;

	// Camera sensivity
	UPROPERTY(EditAnywhere, Category = "Camera Settings", meta = (ClampMin = "0.01"))
	float cameraSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Camera class")
	TSubclassOf<UCameraSnapshot> cameraSnapshotClass = nullptr;

	// HUD
	UPROPERTY(EditAnywhere, Category = "Widget classes")
	TSubclassOf<UUserWidget> hudWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> hudWidget = nullptr;

	// Take snapshot widget
	UPROPERTY(EditAnywhere, Category = "Widget classes")
	TSubclassOf<UUserWidget> takeSnapshotWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> takeSnapshotWidget = nullptr;

	// Photo Gallery widget
	UPROPERTY()
	TObjectPtr<UUserWidget> photoGalleryWidget = nullptr;

	// Level Completed widget
	UPROPERTY(EditAnywhere, Category = "Widget classes")
	TSubclassOf<UUserWidget> levelCompletedWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> levelCompletedWidget = nullptr;

	// Pause Menu
	UPROPERTY(EditAnywhere, Category = "Widget classes")
	TSubclassOf<UUserWidget> pauseWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> pauseWidget = nullptr;

	// Films collected
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int filmsCollected = 0;

	// Snapshot categories
	UPROPERTY(VisibleAnywhere, Category = "Snapshot")
	TArray<TObjectPtr<UTextureRenderTarget2D>> snapshotRenderTargets = TArray<TObjectPtr<UTextureRenderTarget2D>>();

	UPROPERTY(EditAnywhere, Category = "Snapshot")
	TArray<FPhotoGalleryInfo> photoGalleryInfos = TArray<FPhotoGalleryInfo>();

	UPROPERTY(VisibleAnywhere, Category = "Snapshot")
	int currentSnapshotIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "Snapshot")
	int previousSnapshotIndex = 0;

	// Delegates that will be assigned widget blueprints
	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FOnFilmCollected OnFilmCollected{};

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FOnCameraMoved OnCameraMoved{};

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FOnSnapshotCorrectlyTaken OnSnapshotCorrectlyTaken{};

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FOnAbleToTakePicture OnAbleToTakePicture{};

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FOnUnableToTakePicture OnUnableToTakePicture{};

	UPROPERTY(BlueprintAssignable, meta = (AllowPrivateAccess = true))
	FOnTutorialPopup OnTutorialPopup{};

	// Actors to ignore
	TArray<AActor*> actorsToIgnore = TArray<AActor*>();

	// Camera Detection Settings
	UPROPERTY(EditAnywhere, Category = "Camera Detection Settings")
	float cameraDetectionRadius = 40.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Detection Settings")
	float cameraDetectionTraceLength = 2000.0f;

	UPROPERTY()
	mutable ASign* hitSign = nullptr;

	UPROPERTY()
	mutable AThreeLaneSign* hitThreeLaneSign = nullptr;

	UPROPERTY()
	AWaypointActor* waypointActor = nullptr;

	// Camera shutter metasound
	UPROPERTY(EditAnywhere, Category = "Camera Shutter Class")
	TSubclassOf<ACameraShutterMetasound> cameraShutterMetasoundClass = nullptr;

	// Current state
	UPROPERTY(VisibleAnywhere, Category = "Current State")
	ECurrentState currentState = ECurrentState::Exploration;

	UPROPERTY(VisibleAnywhere, Category = "Current State")
	ECurrentState previousState = ECurrentState::Exploration;
};
