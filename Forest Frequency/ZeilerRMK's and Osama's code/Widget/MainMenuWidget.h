// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UCanvasPanel;
class UButton;
class UImage;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class GAMEDEVTVGAMEJAM2026_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	// Button hover functions
	UFUNCTION()
	void HoverOnPlay();

	UFUNCTION()
	void HoverOnControls();

	UFUNCTION()
	void HoverOnCredits();

	UFUNCTION()
	void HoverOnQuit();

	// Button press functions
	UFUNCTION()
	void PlayGame();

	UFUNCTION()
	void GoToControls();

	UFUNCTION()
	void GoToCredits();

	UFUNCTION()
	void QuitGame();

	void RemoveBindedDelegates();

	// State transition functions
	UFUNCTION()
	void TransitionToPlayGame();

	UFUNCTION()
	void TransitionToControls();

	UFUNCTION()
	void TransitionToCredits();

	UFUNCTION()
	void TransitionToQuitGame();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> MainMenuCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> HookImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> PlayButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> ControlsButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> CreditsButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> QuitButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> gameLevel = nullptr;

	UPROPERTY(EditAnywhere, Category = "Hook Image Settings", meta = (AllowPrivateAccess = "true"))
	float hookImageOffsetY = -100.0f;

	// Button animations
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> PressPlayAnimation = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> PressControlsAnimation = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> PressCreditsAnimation = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> PressQuitAnimation = nullptr;
};
