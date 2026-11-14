// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataTable/DialogueDataTable.h"
#include "DialogueWidget.generated.h"

class UCanvasPanel;
class UOverlay;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class SHROOMJAM2026_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDialogueWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> DialogueCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UOverlay> DialogueOverlay;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> DialoguePanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueText;

	// Dialogue text
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true, RowType = "DialogueDataTable"))
	FDataTableRowHandle DialogueRow{};

	TArray<FDialogueDataTable> dialogues;

	// Dialogue text
	FString currentDialogueText;
	FString typedText;

	// Typing text
	int currentCharIndex;

	float typeTimer;
	float typeSpeed;

	bool isTyping = false;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> playerController = nullptr;

	int currentDialogueIndex = 0;
};
