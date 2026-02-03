// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;
class UWidgetAnimation;

UENUM()
enum class MainMenuTransitionState : uint8
{
	ENull,
	EPlay,
	EQuit
};

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

	UFUNCTION()
	void PlayGame();

	UFUNCTION()
	void QuitGame();

private:
	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MainMenuCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MainMenuTitleText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* PlayBurgerFlipAnimation = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* QuitBurgerFlipAnimation = nullptr;

	MainMenuTransitionState menuTransitionState = MainMenuTransitionState::ENull;
};
