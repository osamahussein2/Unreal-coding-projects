// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "CreditsMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCOREJAM37_API UCreditsMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCreditsMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Clicked button functions
	UFUNCTION()
	void GoBackToMainMenu();

	// Hover button functions
	UFUNCTION()
	void HoverOnBackButton();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* CreditsMenuCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* CreditsOverlay;

	// Scrolling credits animation
	UPROPERTY(EditAnywhere, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ScrollCredits;

	// Back button
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* backButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LeftBackImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightBackImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* BackText;
};
