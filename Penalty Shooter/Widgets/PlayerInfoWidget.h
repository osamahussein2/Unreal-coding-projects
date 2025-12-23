// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/Border.h"
#include "Components/EditableTextBox.h"
#include "Animation/WidgetAnimation.h"
#include "Slate/SlateBrushAsset.h"
#include "PlayerInfoWidget.generated.h"

class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class SCOREJAM37_API UPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerInfoWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	void SetPlayerName(FText playerName_);
	void SetPlayerScore(int playerScore);

	void SetLeaderboardButtons(UButton* backButton_, UButton* resetButton_, UImage* leftBackImage_, UImage* rightBackImage_,
		UImage* leftResetImage_, UImage* rightResetImage_);

	void ShowLeaderboardButtons();

	void ShowEditableText();
	void ShowActualLeaderboard();

	UFUNCTION()
	void OnTextChanged(const FText& Text);

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	USizeBox* SizeBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UHorizontalBox* HorizontalBox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerScore;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UOverlay* OverlayC;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UOverlay* OverlayD;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UBorder* BorderC;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UEditableTextBox* nameTextBox;

	UPROPERTY(meta = (BindWidgetAnim, allowPrivateAccess = true), Transient)
	UWidgetAnimation* InputFailedText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	int maxTypedNameStringLength = 10;

	FText typedName;

	UButton* backButton;
	UButton* resetButton;

	UImage* leftBackImage;
	UImage* rightBackImage;
	UImage* leftResetImage;
	UImage* rightResetImage;
};
