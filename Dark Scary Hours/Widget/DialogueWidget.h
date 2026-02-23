// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class UCanvasPanel;
class UOverlay;
class UImage;
class UTextBlock;

/**
 *
 */
UCLASS()
class EHGAMEJAM_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UDialogueWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	UFUNCTION()
	void GoToNextDialogue(FString name_, FString dialogueText_, float typeSpeed_);

	void SkipTyping();

	bool IsTyping() const;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* DialogueCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* DialogueOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* DialoguePanel;

	// Dialogue text
	UPROPERTY(EditAnywhere)
	FString currentDialogueText;

	UPROPERTY(EditAnywhere)
	FString typedText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DialogueText;

	// Typing text
	int currentCharIndex;

	float typeTimer;
	float typeSpeed;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* DialogueNameOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* DialogueNamePanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DialogueNameText;
};