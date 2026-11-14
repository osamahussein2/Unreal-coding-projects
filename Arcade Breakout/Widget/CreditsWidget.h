// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsWidget.generated.h"

class UCanvasPanel;
class UButton;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class SHROOMJAM2026_API UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCreditsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	virtual void NativeDestruct() override;

private:
	// Button press functions
	UFUNCTION()
	void BackToMainMenu();

	// Button press animation functions
	UFUNCTION()
	void OnBackAnimationFinished();

	void RemoveBindedDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CreditsMenuCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> BackButton = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ShowButtons = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> BackPressedAnimation = nullptr;

	FWidgetAnimationDynamicEvent EndDelegate;

	float buttonShowTimer = 0.f;
};
