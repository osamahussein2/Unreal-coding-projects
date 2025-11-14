// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Input/PlayerInputDataAsset.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "CineCameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Widgets/PlayerHUDWidget.h"
#include "Widgets/PauseMenuWidget.h"
#include "Widgets/GameOverWidget.h"
#include "Widgets/GameWinWidget.h"
#include "Components/GameplayTagComponent.h"
#include "Components/HealthRegenerationComponent.h"
#include "Components/AudioComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SHROOMJAM2025_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void AssignCineCameraToPlayer();
	void ShouldPauseMusic(bool paused_);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveCharacter(const FInputActionValue& value_);
	void MoveCamera(const FInputActionValue& value_);

	UHealthRegenerationComponent* GetHealthRegenerationComponent() const;

	void ShowPauseMenu();
	void HidePauseMenu();

	void StartCrouch();
	void EndCrouch();

	UFUNCTION()
	void ChangeGameplayTag(FName AddedTag, FName RemoveTag);

	UFUNCTION()
	void AddToGameplayTag(FName AddedTag);

	UFUNCTION()
	void RemoveGameplayTag(FName RemoveTag);

	void ShowPlayerHUD();
	void HidePlayerHUD();

	void SetGameOverScreen();
	void ShowGameOverScreen();

	void EnableMouseCursor();
	void DisableMouseCursor();

	UFUNCTION()
	void SetPlayerDamaged(float newValue);

	UFUNCTION()
	void HealPlayer();

	void ShowGameWonScreen();

private:
	void RegeneratePlayerHealth(float& DeltaTime);
	void UpdatePlayerHealthInformation(float& DeltaTime);

private:
	APlayerController* playerController;
	class ASceneCapture2D* sceneCapture;

	// Input objects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* inputMapping;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UPlayerInputDataAsset* PlayerDataAsset;

	UPROPERTY(VisibleAnywhere)
	ACineCameraActor* cineCamera;

	UPROPERTY(EditAnywhere)
	UCameraComponent* cameraComponent;

	UPROPERTY(EditAnywhere)
	UGameplayTagComponent* gameplayTagComponent;

	// Minimap for the player icon
	UPROPERTY(EditAnywhere)
	USpringArmComponent* minimapSpringArm;

	UPROPERTY(EditAnywhere)
	USceneCaptureComponent2D* sceneCaptureComponent;

	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* spriteComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerHUDWidget> playerHUDClass;

	UPlayerHUDWidget* playerHUD;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPauseMenuWidget> pauseMenuClass;

	UPauseMenuWidget* pauseMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameOverWidget> gameOverClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameWinWidget> gameWinClass;

	UPROPERTY(EditAnywhere)
	UHealthRegenerationComponent* healthRegenerationComponent;

	UPROPERTY(EditAnywhere)
	UAudioComponent* musicComponent;

	// Camera crouch
	UPROPERTY(EditAnywhere, Category = "Camera Position")
	float maxCrouchPosition = -40.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	float maxStandingPosition = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Position")
	float cameraMoveSpeed = 10.0f;

	float startingWalkSpeed;

	float healTimer;

	//float damageTime;

public:
	// Mobile touch input
	void StartTouchInput(const FInputActionValue& Value);

	// Mobile related functions
	void MobileControlCamera(const FInputActionValue& Value);

	void InitializeMobileInterface();

private:
	// Mobile related variables
	FVector2D startTouchLocation;

	UPROPERTY(EditAnywhere)
	UTouchInterface* gameTouchInterface;

	// Mobile booleans
	bool isTouchInterfaceEnabled;
	bool canDragCamera;
};
