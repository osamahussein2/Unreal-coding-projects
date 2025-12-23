// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCOREJAM37_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	// Main Menu functionality
	UFUNCTION()
	void PlayLevel();

	UFUNCTION()
	void GoToOptionsMenu();

	UFUNCTION()
	void GoToCreditsMenu();

	UFUNCTION()
	void GoToLeaderboard();

	UFUNCTION()
	void QuitGame();

	// Main Menu Hover functionality
	UFUNCTION()
	void HoverOnPlayButton();

	UFUNCTION()
	void HoverOnOptionsButton();

	UFUNCTION()
	void HoverOnCreditsButton();

	UFUNCTION()
	void HoverOnLeaderboardButton();

	UFUNCTION()
	void HoverOnQuitButton();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MainMenuCanvas;

	// Play area
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftPlayImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightPlayImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayText;

	// Options area
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OptionsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftOptionsImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightOptionsImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* OptionsText;

	// Credits area
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreditsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftCreditsImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightCreditsImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CreditsText;

	// Leaderboard area
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* LeaderboardButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftLeaderboardImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightLeaderboardImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* LeaderboardText;

	// Quit area
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftQuitImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightQuitImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* QuitText;
};
