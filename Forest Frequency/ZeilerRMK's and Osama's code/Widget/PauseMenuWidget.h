// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UCanvasPanel;
class UButton;
class UImage;
class UWidgetAnimation;

UENUM()
enum class PressedButtonAnimation : uint8
{
	Null,
	ResumeAnimation,
	QuitToMainMenu
};

/**
 * 
 */
UCLASS()
class GAMEDEVTVGAMEJAM2026_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPauseMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeDestruct() override;

	void PlayResumeAnimation();

private:
	// Button hover functions
	UFUNCTION()
	void HoverOnResume();

	UFUNCTION()
	void HoverOnMainMenu();

	// Button press functions
	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void QuitToMainMenu();

	void RemoveBindedDelegates();

	// State transition functions
	UFUNCTION()
	void TransitionToResume();

	UFUNCTION()
	void TransitionToMainMenu();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> PauseMenuCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> OverlayedImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> HookImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> QuitToMainMenuButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Levels", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> menuLevel = nullptr;

	// Button animations
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> PressResumeAnimation = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> PressMainMenuAnimation = nullptr;

	PressedButtonAnimation pressedButtonAnimation = PressedButtonAnimation::Null;

	float animationTimer = 0.0f;
};
