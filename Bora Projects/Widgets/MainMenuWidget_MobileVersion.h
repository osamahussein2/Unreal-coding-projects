// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "MainMenuWidget_MobileVersion.generated.h"

/**
 * 
 */
UCLASS()
class SHROOMJAM2025_API UMainMenuWidget_MobileVersion : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget_MobileVersion(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	// Main Menu functionality (will wait until to transition to next state)
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void TransitionToPlayGame();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void TransitionToCreditsMenu();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void TransitionToQuitGame();

	UFUNCTION(BlueprintCallable, Category = "Credits Menu")
	void TransitionBackToMainMenu(); // Credits Menu functionality

	// Actual main menu transition functionality
	void PlayLevel();
	void CreditsMenu();
	void QuitGame();
	void GoToMain();

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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* TVStaticImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* TVCreditsStaticImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ScrollCredits;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* WelcomeToTheGame;

private:
	UPROPERTY(EditDefaultsOnly)
	float scrollCreditsAnimationSpeed = 0.3f;

	UPROPERTY(EditDefaultsOnly)
	float goToNextStateTime = 0.6f;

	UPROPERTY(EditDefaultsOnly)
	float brightStaticValue = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float darkStaticValue = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UI Materials")
	UMaterialInterface* TVStaticMaterial;

	UMaterialInstanceDynamic* materialInst;
};
