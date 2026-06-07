// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlsWidget.generated.h"

class UCanvasPanel;
class UButton;
class UTextBlock;
class UImage;
class UWidgetAnimation;
class ADefaultPlayerController;

UENUM()
enum class MappingControlsState : uint8
{
	ENull,
	EModifyingMoveLeftBinding,
	EModifyingMoveRightBinding,
	EModifyingMoveDownBinding,
	EModifyingMoveUpBinding,
	EModifyingJumpBinding,
	EModifyingInteractBinding,
	EModifyingSprintBinding,
	EModifyingFireBinding,
	EModifyingPauseBinding
};

/**
 * 
 */
UCLASS()
class GAMEDEVTVGAMEJAM2026_API UControlsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UControlsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	virtual void NativeDestruct() override;

private:
	// Update input button
	UFUNCTION()
	void ChangeMoveLeftInput();

	UFUNCTION()
	void ChangeMoveRightInput();

	UFUNCTION()
	void ChangeMoveDownInput();

	UFUNCTION()
	void ChangeMoveUpInput();

	UFUNCTION()
	void ChangeJumpInput();

	UFUNCTION()
	void ChangeInteractInput();

	UFUNCTION()
	void ChangeSprintInput();

	UFUNCTION()
	void ChangeFireInput();

	UFUNCTION()
	void ChangePauseInput();

	// Back to main menu button
	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void TransitionToMainMenu();

	// Change move left input function
	void SetInputForMoveLeft(const FKey& newKey_);

	// Change move right input function
	void SetInputForMoveRight(const FKey& newKey_);

	// Change move down input function
	void SetInputForMoveDown(const FKey& newKey_);

	// Change move up input function
	void SetInputForMoveUp(const FKey& newKey_);

	// Change jump input function
	void SetInputForJump(const FKey& newKey_);

	// Change interact input function
	void SetInputForInteract(const FKey& newKey_);

	// Change sprint input function
	void SetInputForSprint(const FKey& newKey_);

	// Change fire input function
	void SetInputForFire(const FKey& newKey_);

	// Change pause input function
	void SetInputForPause(const FKey& newKey_);

	// Update Input Text function
	void UpdateInputText(UTextBlock* textBlock_, const FKey& assignedKey_);

	void RemoveBindedDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ControlsCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> HookImage = nullptr;

	// Move Left Button and Text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveLeftText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveLeftInputText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> MoveLeftInputButton = nullptr;

	// Move Right Button and Text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveRightText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveRightInputText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> MoveRightInputButton = nullptr;

	// Move Down Button and Text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveDownText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveDownInputText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> MoveDownInputButton = nullptr;

	// Move Up Button and Text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveUpText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveUpInputText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> MoveUpInputButton = nullptr;

	// Jump Input Button and Text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> JumpText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> JumpInputText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> JumpInputButton = nullptr;

	// Interact Input Button and Text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractInputText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> InteractInputButton = nullptr;

	// Sprint Input Button and Text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> SprintText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> SprintInputText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> SprintInputButton = nullptr;

	// Fire Input Button and Text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> FireText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> FireInputText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> FireInputButton = nullptr;

	// Pause Input Button and Text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> PauseText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> PauseInputText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> PauseInputButton = nullptr;

	// Back button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> BackButton = nullptr;

	// Changing input enum state
	UPROPERTY(VisibleAnywhere, Category = "Mapping Controls State")
	MappingControlsState mappingControlsState = MappingControlsState::ENull;

	// Button colors when the player is trying to remap an input action
	UPROPERTY(VisibleAnywhere, Category = "Button Color")
	FLinearColor normalButtonColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Button Color")
	FLinearColor inputToPressButtonColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Default Player controller
	UPROPERTY()
	ADefaultPlayerController* defaultPlayerController = nullptr;

	// Button animation
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> PressBackAnimation = nullptr;
};
