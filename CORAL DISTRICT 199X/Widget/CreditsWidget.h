// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsWidget.generated.h"

class UCanvasPanel;
class UImage;
class UOverlay;
class UButton;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCreditsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void GoBackToMainMenu();

	void RemoveBindedButtonDelegates();

	UFUNCTION()
	void PlayCreditsScrollAnimation();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* CreditsMenuCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* CreditsOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient, meta = (AllowPrivateAccess = true))
	UWidgetAnimation* ScrollCredits;

	UPROPERTY(EditAnywhere, Category = "Wait Times")
	float timeToScrollCreditsAgain = 20.0f;
};
