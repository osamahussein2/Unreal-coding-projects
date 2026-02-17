// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/PickNerveWidget.h"
#include "PickFiveNervesWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UPickFiveNervesWidget : public UPickNerveWidget
{
	GENERATED_BODY()
	
public:
	UPickFiveNervesWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void RemoveBindedButtonDelegates() override;

private:
	UFUNCTION()
	void NerveOneSelected();

	UFUNCTION()
	void NerveTwoSelected();

	UFUNCTION()
	void NerveThreeSelected();

	UFUNCTION()
	void NerveFourSelected();

	UFUNCTION()
	void NerveFiveSelected();

	UFUNCTION()
	void RestartNerveSequence();

	// Setup nerve button images functions
	void SetupNerveOneButtonImageDependingOnNerve();
	void SetupNerveTwoButtonImageDependingOnNerve();
	void SetupNerveThreeButtonImageDependingOnNerve();
	void SetupNerveFourButtonImageDependingOnNerve();
	void SetupNerveFiveButtonImageDependingOnNerve();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* nerveOneButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* nerveTwoButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* nerveThreeButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* nerveFourButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* nerveFiveButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* sequencesText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* restartSequenceButton = nullptr;

protected:
	UPROPERTY(EditAnywhere, Category = "Nerve Parameters")
	FDeprecateSlateVector2D nerveImageSize = FDeprecateSlateVector2D(200.0f, 200.0f);

private:
	int buttonOneTextureNumber = 0, buttonTwoTextureNumber = 0, buttonThreeTextureNumber = 0, buttonFourTextureNumber = 0,
		buttonFiveTextureNumber = 0;

	/* 0 for first sequence press sound, 1 for second sequence press sound, 2 for third sequence press sound, 3 for fourth
	sequence press sound, and 4 for fifth sequence press sound */
	UPROPERTY(EditAnywhere, Category = "Music")
	TArray<USoundBase*> sequencePressedSounds;
};
