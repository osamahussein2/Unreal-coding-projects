// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanel.h"
#include "GameOverWidget.generated.h"

UENUM()
enum class NextGameOverState : uint8
{
	ENull,
	EResume,
	EQuit
};

/**
 * 
 */
UCLASS()
class SHROOMJAM2025_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGameOverWidget(const FObjectInitializer& ObjectInitializer);

	//virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	void UpdateGameOverButtonState();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void HoverOnPlayButton();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void HoverOnQuitButton();

	// Main Menu functionality
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void QuitToMain();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* GameOverCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RestartButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowButtons;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HideButtons;

	UPROPERTY(BlueprintReadOnly)
	int selectedButton;

	FSlateColor normalColor, hoveredColor, pressedColor;

private:
	void PauseGameMusic(bool paused_);

private:
	NextGameOverState nextState;
};
