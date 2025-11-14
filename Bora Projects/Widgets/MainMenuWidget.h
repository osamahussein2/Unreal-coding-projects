// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanel.h"
#include "MainMenuWidget.generated.h"

UENUM()
enum class NextMainMenuState : uint8
{
	ENull,
	EPlay,
	ECredits,
	EGoBackToMainMenu,
	EQuit
};

/**
 * 
 */
UCLASS()
class SHROOMJAM2025_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	//virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	void UpdateMainMenuButtonState();
	void UpdateCreditsMenuButtonState();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void HoverOnPlayButton();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void HoverOnCreditsButton();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void HoverOnQuitButton();

	//void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	// Main Menu functionality
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void PlayLevel();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void GoToCredits();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void QuitGame();

	// Credits Menu functionality
	UFUNCTION(BlueprintCallable, Category = "Credits Menu")
	void GoBackToMainMenu();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MainMenuCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* CreditsMenuCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreditsButton;

	// Credits Back button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreditsBackButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowButtons;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HideButtons;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ScrollCredits;

	UPROPERTY(BlueprintReadOnly)
	int selectedButton;

	FSlateColor normalColor, hoveredColor, pressedColor;

private:
	NextMainMenuState nextState;
};
