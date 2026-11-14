// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelCompleteWidget.generated.h"

class UCanvasPanel;
class UImage;
class UButton;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class SHROOMJAM2026_API ULevelCompleteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULevelCompleteWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	virtual void NativeDestruct() override;

private:
	// Button press functions
	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void QuitToMainMenu();

	// Button press animation functions
	UFUNCTION()
	void OnRestartAnimationFinished();

	UFUNCTION()
	void OnQuitAnimationFinished();

	void RemoveBindedDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> LevelCompleteCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> TransparentImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> RestartButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> QuitButton = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ShowButtons = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> RestartPressedAnimation = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> QuitPressedAnimation = nullptr;

	FWidgetAnimationDynamicEvent EndDelegate;

	float buttonShowTimer = 0.f;
};
