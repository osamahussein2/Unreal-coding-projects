// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCOREJAM37_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPauseMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// Pause Menu functionality
	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void QuitGame();

	// Pause Menu Hover functionality
	UFUNCTION()
	void HoverOnResumeButton();

	UFUNCTION()
	void HoverOnQuitButton();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PauseMenuCanvas;

	// Resume area
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftResumeImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightResumeImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ResumeText;

	// Quit area
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftQuitImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightQuitImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* QuitText;
};
