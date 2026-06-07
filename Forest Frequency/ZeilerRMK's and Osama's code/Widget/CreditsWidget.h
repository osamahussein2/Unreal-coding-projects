// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsWidget.generated.h"

class UCanvasPanel;
class UButton;
class UTextBlock;
class UImage;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class GAMEDEVTVGAMEJAM2026_API UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCreditsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	// Credits text update functions
	UFUNCTION()
	void ShowCreditsText();

	UFUNCTION()
	void HideCreditsText();

	// Back to main menu function
	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void TransitionToMainMenu();

	void RemoveBindedDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CreditsCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> HookImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> BackButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> RoleText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> TeamMemberText = nullptr;

	// Credits texts
	UPROPERTY(EditAnywhere, Category = "Credits Texts", meta = (MultiLine = true))
	TArray<FString> RoleStringTexts;

	UPROPERTY(EditAnywhere, Category = "Credits Texts", meta = (MultiLine = true))
	TArray<FString> TeamMemberStringTexts;

	float textOpacity = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Text Time", meta = (ClampMin = "0.0"))
	float maxHideTextTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Text Time", meta = (ClampMin = "0.0"))
	float maxShowTextTime = 2.0f;

	int creditsIndex = 0;

	// Timer handles for showing/hiding credits text
	FTimerHandle showCreditsTextHandle{};
	FTimerHandle hideCreditsTextHandle{};

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> PressBackAnimation = nullptr;
};
