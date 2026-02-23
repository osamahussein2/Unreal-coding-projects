// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/DialogueWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	currentCharIndex = 0;
	typeTimer = 0.0f;
	typeSpeed = 0.0f;
	currentDialogueText = "";
	typedText = "";
}

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDialogueWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	if (IsTyping())
	{
		typeTimer += DeltaTime;

		// Check if type timer exceeds the character speed interval to type the next character
		if (typeTimer >= 1.f / typeSpeed)
		{
			currentDialogueText.AppendChar(typedText[currentCharIndex]);
			currentCharIndex++;

			DialogueText->SetText(FText::FromString(currentDialogueText));

			typeTimer = 0.0f; // Reset type timer to wait until typing the next character again
		}
	}
}

void UDialogueWidget::GoToNextDialogue(FString name_, FString dialogueText_, float typeSpeed_)
{
	// And update any dialogue and name texts
	DialogueNameText->SetText(FText::FromString(name_));

	currentDialogueText = "";
	typedText = dialogueText_;
	typeSpeed = typeSpeed_;
	currentCharIndex = 0;

	if (typeTimer != 0.0f) typeTimer = 0.0f;

	// Set actual dialogue text to the current dialogue text being typed
	DialogueText->SetText(FText::FromString(currentDialogueText));
}

void UDialogueWidget::SkipTyping()
{
	DialogueText->SetText(FText::FromString(typedText));
	currentCharIndex = typedText.Len();

	if (typeTimer != 0.0f) typeTimer = 0.0f;
}

bool UDialogueWidget::IsTyping() const
{
	return currentCharIndex < typedText.Len();
}