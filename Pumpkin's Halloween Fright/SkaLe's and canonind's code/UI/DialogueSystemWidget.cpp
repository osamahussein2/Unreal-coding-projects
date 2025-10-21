// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueSystemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UI/UIManager.h"
#include "Engine/AssetManager.h"

UDialogueSystemWidget::UDialogueSystemWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bShouldHideLowerWidget = false;
	bAllowsPause = false;
	bHideOnPauseToggle = false;
	bPauseGameWhenShown = false;
	bDisablePlayerInput = true;
}

void UDialogueSystemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UDialogueSystemWidget::OnContinueClicked);
	}
}

void UDialogueSystemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsTyping)
	{
		TypingTimer += InDeltaTime;
		float CharInterval = 1.f / TypingSpeed;

		while (TypingTimer >= CharInterval && CurrentCharIndex < TypedText.Len())
		{
			TypingTimer -= CharInterval;
			CurrentTypedText.AppendChar(TypedText[CurrentCharIndex]);
			CurrentCharIndex++;

			DialogueText->SetText(FText::FromString(CurrentTypedText));

			if (CurrentCharIndex >= TypedText.Len())
			{
				bIsTyping = false;
				break;
			}
		}
	}
}

void UDialogueSystemWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UDialogueSystemWidget::StartDialogueByID(FName DialogueID)
{
	if (!DialogueTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("No DialogueTable assigned"));
		if (OwningManager)
		{
			OwningManager->PopWidget();
		}
		return;
	}

	TArray<FDialogueRow*> AllRows;
	DialogueTable->GetAllRows(TEXT("Dialogue"), AllRows);

	CurrentDialogueLines.Empty();
	for (auto Row : AllRows)
	{
		if (Row->DialogueID == DialogueID)
		{
			CurrentDialogueLines.Add(*Row);
		}
	}

	CurrentDialogueLines.Sort([](const FDialogueRow& A, const FDialogueRow& B)
		{
			return A.LineIndex < B.LineIndex;
		});

	if (CurrentDialogueLines.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dialogue ID '%s' not found"), *DialogueID.ToString());
		return;
	}
	CurrentLineIndex = 0;
	DisplayCurrentLine();
}

void UDialogueSystemWidget::OnContinueClicked()
{
	if (bIsTyping)
	{
		// Skip to full text
		DialogueText->SetText(FText::FromString(TypedText));
		bIsTyping = false;
		return;
	}

	CurrentLineIndex++;
	if (CurrentLineIndex < CurrentDialogueLines.Num())
	{
		DisplayCurrentLine();
	}
	else
	{
		EndDialogue();
	}
}

void UDialogueSystemWidget::DisplayCurrentLine()
{
	if (!DialogueText || CurrentDialogueLines.Num() == 0)
		return;

	const FDialogueRow& Line = CurrentDialogueLines[CurrentLineIndex];

	if (SpeakerName)
	{
		SpeakerName->SetText(Line.Speaker);
	}
	if (PortraitImage)
	{
		if (Line.Portrait.IsValid())
		{
			PortraitImage->SetBrushFromTexture(Line.Portrait.Get());
		}
		else if (!Line.Portrait.IsNull())
		{
			FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
			Streamable.RequestAsyncLoad(Line.Portrait.ToSoftObjectPath(),
				[this, Line]() {
					if (IsValid(this))
					{
						if (UTexture2D* Loaded = Line.Portrait.Get())
						{
							PortraitImage->SetBrushFromTexture(Loaded);
						}
					}
				});
		}
		else
		{
			PortraitImage->SetBrushFromTexture(nullptr);
		}
	}


	TypedText = Line.Text.ToString();
	CurrentTypedText.Empty();
	CurrentCharIndex = 0;
	bIsTyping = true;
	TypingTimer = 0.f;
	TypingSpeed = Line.DisplaySpeed;

	DialogueText->SetText(FText::GetEmpty());
}

void UDialogueSystemWidget::EndDialogue()
{
	TriggerHide();
}


