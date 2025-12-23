// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UpgradeMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCOREJAM37_API UUpgradeMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUpgradeMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void ContinueToLeaderboard();

	// Upgrade button clicked functions
	UFUNCTION()
	void UpgradeBallSpeed();

	UFUNCTION()
	void UpgradeGoalkeeperSpeed();

	UFUNCTION()
	void UpgradeEXPMultiplier();

	// Upgrade button hovered functions
	UFUNCTION()
	void HoverOnBallSpeedUpgrade();

	UFUNCTION()
	void HoverOnGoalkeeperSpeedUpgrade();

	UFUNCTION()
	void HoverOnEXPMultiplierUpgrade();

	UFUNCTION()
	void HoverOnContinueButton();

private:
	void InitializeBallSpeedTexts(TWeakObjectPtr<class APlayerCharacter> playerCharacter_);
	void InitializeGoalkeeperSpeedTexts(TWeakObjectPtr<class APlayerCharacter> playerCharacter_);
	void InitializeEXPMultiplierTexts(TWeakObjectPtr<class APlayerCharacter> playerCharacter_);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* UpgradeMenuCanvas;

	// Money text
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerMoneyText;

	// Speed ball upgrade UI
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SpeedBallUpgradeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* BallSpeedUpgradeTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* BallSpeedUpgradeInformation;

	// Goalkeeper speed upgrade UI
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GoalkeeperSpeedUpgradeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* GoalkeeperSpeedUpgradeTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* GoalkeeperSpeedUpgradeInformation;

	// EXP multiplier upgrade UI
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* EXPMultiplierUpgradeButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* EXPMultiplierUpgradeTitle;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* EXPMultiplierUpgradeInformation;

	// Continue button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ContinueText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftContinueImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightContinueImage;
};
