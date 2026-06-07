// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DefaultPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
struct FEnhancedActionKeyMapping;
class UEnhancedInputUserSettings;

/**
 * 
 */
UCLASS()
class GAMEDEVTVGAMEJAM2026_API ADefaultPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ADefaultPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	// Input key getters
	FKey MoveLeftKey() const;
	FKey MoveRightKey() const;
	FKey MoveDownKey() const;
	FKey MoveUpKey() const;
	FKey JumpKey() const;
	FKey InteractKey() const;
	FKey SprintKey() const;
	FKey FireKey() const;
	FKey PauseKey() const;

	bool NewKeyConflictsWithActionMapping(const FKey& newKey_) const;

	// Change controls
	void UpdateMoveLeftInput(const FKey& newKey_);
	void UpdateMoveRightInput(const FKey& newKey_);
	void UpdateMoveDownInput(const FKey& newKey_);
	void UpdateMoveUpInput(const FKey& newKey_);
	void UpdateJumpInput(const FKey& newKey_);
	void UpdateInteractInput(const FKey& newKey_);
	void UpdateSprintInput(const FKey& newKey_);
	void UpdateFireInput(const FKey& newKey_);
	void UpdatePauseInput(const FKey& newKey_);

private:
	UFUNCTION()
	void InitializeKeys();

	void Move(const FInputActionValue& Value);
	void SwingAround();
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void Jump();
	void Pause();

private:
	// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction = nullptr;

	// Current Input Keys
	UPROPERTY(VisibleAnywhere, Category = "Current Input Keys")
	FKey currentMoveLeftKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Current Input Keys")
	FKey currentMoveRightKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Current Input Keys")
	FKey currentMoveDownKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Current Input Keys")
	FKey currentMoveUpKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Current Input Keys")
	FKey currentJumpKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Current Input Keys")
	FKey currentInteractKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Current Input Keys")
	FKey currentSprintKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Current Input Keys")
	FKey currentFireKey = FKey();

	UPROPERTY(VisibleAnywhere, Category = "Current Input Keys")
	FKey currentPauseKey = FKey();

	// Input Mapping Names
	UPROPERTY(EditAnywhere, Category = "Input Mapping Name")
	FName moveLeftMapName = FName();

	UPROPERTY(EditAnywhere, Category = "Input Mapping Name")
	FName moveRightMapName = FName();

	UPROPERTY(EditAnywhere, Category = "Input Mapping Name")
	FName moveDownMapName = FName();

	UPROPERTY(EditAnywhere, Category = "Input Mapping Name")
	FName moveUpMapName = FName();
};
