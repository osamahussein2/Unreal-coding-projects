// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "LeaderboardWidget.generated.h"

class UPlayerInfoWidget;

/**
 * 
 */
UCLASS()
class SCOREJAM37_API ULeaderboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULeaderboardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	// Clicked button functions
	UFUNCTION()
	void GoBackToMainMenu();

	UFUNCTION()
	void ResetLeaderboard();

	// Hover button functions
	UFUNCTION()
	void HoverOnBackButton();

	UFUNCTION()
	void HoverOnResetButton();

private:
	// Player info widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerInfoWidget> playerInfoWidgetClass;
	UPlayerInfoWidget* playerInfoWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UCanvasPanel* LeaderboardCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UBorder* BorderC;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UBackgroundBlur* backgroundBlur;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UVerticalBox* verticalBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UVerticalBox* playerContainer;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* leaderboardText;

	// Back button
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* backButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftBackImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightBackImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* BackText;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* resetButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftResetImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightResetImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* ResetText;
};
