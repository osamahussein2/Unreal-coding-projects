// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Engine/Level.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class GMTKJAM2025_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	// Main Menu functionality
	UFUNCTION()
	void PlayLevel();

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void GoToCreditsMenu();

	UFUNCTION()
	void GoToControlsMenu();

	UFUNCTION()
	void GoToConceptArtMenu();

	// Go Back to Main
	UFUNCTION()
	void GoToBackToMainMenu();

	// Controls functionality
	UFUNCTION()
	void ResetControls();

	// Concept art functionality
	UFUNCTION()
	void LookAtPreviousConceptArt();

	UFUNCTION()
	void LookAtNextConceptArt();

	// Modify controls functionality
	UFUNCTION()
	void ChangeForwardKey();

	UFUNCTION()
	void ChangeBackKey();

	UFUNCTION()
	void ChangeLeftKey();

	UFUNCTION()
	void ChangeRightKey();

	UFUNCTION()
	void ChangePauseKeyLogic();

	UFUNCTION()
	void ChangeAttackKeyLogic();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MainMenuCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* CreditsMenuCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* ControlsMenuCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* ConceptArtMenuCanvas;

	// Main Menu buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreditsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ControlsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConceptArtButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

	// Credits buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreditsBackButton;

	// Controls buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ControlsResetButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ControlsBackButton;

	// Concept art buttons

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConceptArtLeftButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConceptArtRightButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ConceptArtBackButton;

	// Modify controls buttons (movement keys)
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChangeMoveForwardButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChangeMoveBackButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChangeMoveLeftButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChangeMoveRightButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChangePauseButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ChangeAttackButton;

	// Movement key texts
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* forwardKeyText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* backKeyText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* leftKeyText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* rightKeyText;

	// Action key texts
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* pauseKeyText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* attackKeyText;

	// Concept art current image number text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* currentImageText;

	// Menu background
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	// Concept art image
	UImage* ConceptArtImage;

	// Update controls texts
	void SetMoveForwardControlsText();
	void SetMoveBackControlsText();
	void SetMoveLeftControlsText();
	void SetMoveRightControlsText();

	void SetPauseControlsText();
	void SetAttackControlsText();

private:
	float currentImageIndex;

	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> conceptArtImages;

	float imageNumber, maxImages;
};
