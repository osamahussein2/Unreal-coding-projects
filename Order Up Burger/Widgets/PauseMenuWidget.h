// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;
class UWidgetAnimation;

UENUM()
enum class PauseMenuTransitionState : uint8
{
	ENull,
	EResume,
	EQuit
};

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPauseMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void QuitGame();

private:
	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PauseMenuCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PauseTitleText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ResumeButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* ResumeBurgerFlipAnimation = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* QuitBurgerFlipAnimation = nullptr;
	PauseMenuTransitionState menuTransitionState = PauseMenuTransitionState::ENull;
};
