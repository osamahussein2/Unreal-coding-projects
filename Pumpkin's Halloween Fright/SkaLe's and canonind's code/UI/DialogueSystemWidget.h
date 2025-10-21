// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "UI/DialogueRow.h"
#include "DialogueSystemWidget.generated.h"


class UTextBlock;
class UButton;
class UImage;


/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UDialogueSystemWidget : public UBaseUIWidget
{
	GENERATED_BODY()

public:
	UDialogueSystemWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogueByID(FName DialogueID);

protected:
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void OnContinueClicked();
	void DisplayCurrentLine();
	void EndDialogue();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	UDataTable* DialogueTable;
protected:

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SpeakerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DialogueText;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* PortraitImage;

	UPROPERTY(meta = (BindWidget))
	class UButton* ContinueButton;


private:
	TArray<FDialogueRow> CurrentDialogueLines;

	int32 CurrentLineIndex = 0;
	
	FString TypedText;
	FString CurrentTypedText;
	int32 CurrentCharIndex = 0;
	bool bIsTyping = false;
	float TypingTimer = 0.f;
	float TypingSpeed = 30.f;


};
