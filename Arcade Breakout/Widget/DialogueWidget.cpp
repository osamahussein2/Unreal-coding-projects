// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/DialogueWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/WidgetManager.h"
#include "Widget/LevelCompleteWidget.h"

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::SetGamePaused(this, true);

	TArray<FDialogueDataTable*> npcDialoguePointers;
	if (DialogueRow.DataTable) DialogueRow.DataTable->GetAllRows("", npcDialoguePointers);

	for (FDialogueDataTable* npcDialoguePointer : npcDialoguePointers)
	{
		// Add the deferenced pointer (value) into the dialogues array
		dialogues.Add(*npcDialoguePointer);
		npcDialoguePointer = nullptr; // Release the pointer
	}

	// Then empty the entire npc dialogue pointers array if necessary
	if (!npcDialoguePointers.IsEmpty()) npcDialoguePointers.Empty();

	if (!dialogues.IsEmpty())
	{
		isTyping = true;
		typedText = dialogues[0].Line;
		typeSpeed = dialogues[0].typeSpeed;
	}

	playerController = UGameplayStatics::GetPlayerController(this, 0);
}

void UDialogueWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (isTyping)
	{
		typeTimer += InDeltaTime;

		// Check if type timer exceeds the character speed interval to type the next character
		while (typeTimer >= 1.0f / typeSpeed && currentCharIndex < typedText.Len())
		{
			typeTimer -= 1.0f / typeSpeed;
			currentDialogueText.AppendChar(typedText[currentCharIndex]);
			currentCharIndex++;

			DialogueText->SetText(FText::FromString(currentDialogueText));

			typeTimer = 0.0f; // Reset type timer to wait until typing the next character again

			if (currentCharIndex >= typedText.Len())
			{
				isTyping = false;
				break;
			}
		}
	}

	if (playerController.IsValid())
	{
		if (playerController->WasInputKeyJustPressed(EKeys::SpaceBar))
		{
			if (isTyping)
			{
				DialogueText->SetText(FText::FromString(typedText));

				isTyping = false;

				typeTimer = 0.0f;
			}

			else
			{
				if (currentDialogueIndex < dialogues.Num() - 1)
				{
					currentCharIndex = 0;
					++currentDialogueIndex;

					isTyping = true;
					typedText = dialogues[currentDialogueIndex].Line;
					typeSpeed = dialogues[currentDialogueIndex].typeSpeed;
				}

				else
				{
					if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
					{
						widgetManager->PopWidget(GetClass());
						widgetManager->PushWidget(widgetManager->GetLevelCompleteClass());

						widgetManager->EnableMouseCursor();
					}
				}
			}
		}
	}
}

void UDialogueWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (DialogueText)
	{
		DialogueText->RemoveFromParent();
		DialogueText = nullptr;
	}

	if (DialoguePanel)
	{
		DialoguePanel->RemoveFromParent();
		DialoguePanel = nullptr;
	}

	if (DialogueOverlay)
	{
		DialogueOverlay->RemoveFromParent();
		DialogueOverlay = nullptr;
	}

	if (DialogueCanvas)
	{
		DialogueCanvas->RemoveFromParent();
		DialogueCanvas = nullptr;
	}
}