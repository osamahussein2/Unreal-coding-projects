// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/InstructionWidget.h"
#include "Components/TextBlock.h"

UInstructionWidget::UInstructionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInstructionWidget::NativeConstruct()
{
	if (instructionText && !instructionText->IsVisible()) instructionText->SetVisibility(ESlateVisibility::Visible);
}

void UInstructionWidget::SetInstructionText(FString& text_)
{
	if (instructionText) instructionText->SetText(FText::FromString(text_));
}
