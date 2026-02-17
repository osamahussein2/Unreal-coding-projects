// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsWidget.generated.h"

class UCanvasPanel;
class UOverlay;
class UTextBlock;
class UButton;


/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCreditsWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

private:
	void UpdateCreditsText();

	void RemoveBindedButtonDelegates();

	UFUNCTION()
	void AnimateCreditsText();

	UFUNCTION()
	void GoBackToMainMenu();

private:
	// Widget components
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* CreditsMenuCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CreditsTitleText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* RoleText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* TeamMemberText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton;

	// Modifiable string arrays in engine
	UPROPERTY(EditAnywhere, Category = "Text Strings")
	TArray<FString> RoleTextStrings;

	UPROPERTY(EditAnywhere, Category = "Text Strings")
	TArray<FString> TeamMemberTextStrings;

	float textOpacity = 0.0f;

	float hideTextTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Text Time", meta = (ClampMin = "0.0"))
	float maxHideTextTime = 2.0f;

	float showTextTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Text Time", meta = (ClampMin = "0.0"))
	float maxShowTextTime = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = "Text Time")
	bool textHidden = true;

	UPROPERTY()
	int creditsIndex = 0;
};
