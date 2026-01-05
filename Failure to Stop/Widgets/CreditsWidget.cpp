// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/CreditsWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

UCreditsWidget::UCreditsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RoleText)
	{
		RoleText->SetOpacity(textOpacity);
		RoleText->SetText(FText::FromString(TEXT("C++ Programmer")));
	}

	if (TeamMemberText)
	{
		TeamMemberText->SetOpacity(textOpacity);
		TeamMemberText->SetText(FText::FromString(TEXT("Osama Hussein")));
	}
}

void UCreditsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (textHidden)
	{
		hideTextTimer += InDeltaTime;

		if (hideTextTimer >= maxHideTextTime) // Hide text timer exceeds max hide text time
		{
			textOpacity += InDeltaTime; // Increase text opacity

			// If the text opacity is less than 1, update the credits texts opacity
			if (textOpacity < 1.0f)
			{
				if (RoleText) RoleText->SetOpacity(textOpacity);
				if (TeamMemberText) TeamMemberText->SetOpacity(textOpacity);
			}

			// Text opacity exceeds max hide text time times 2, try to hide the text
			else if (textOpacity >= maxHideTextTime * 2.0f)
			{
				textOpacity = 1.0f;

				hideTextTimer = 0.0f;
				textHidden = false;
			}
		}
	}

	else
	{
		showTextTimer += InDeltaTime;

		if (showTextTimer >= maxShowTextTime)
		{
			textOpacity -= InDeltaTime; // Decrease text opacity

			// If the text opacity is greater than 0, update the credits texts opacity
			if (textOpacity > 0.0f)
			{
				if (RoleText) RoleText->SetOpacity(textOpacity);
				if (TeamMemberText) TeamMemberText->SetOpacity(textOpacity);
			}

			// Text opacity exceeds the negative value of max show text time, try to show the text and update credits text
			else if (textOpacity <= -maxShowTextTime)
			{
				showTextTimer = 0.0f;
				textOpacity = 0.0f;

				UpdateCreditsText();
				textHidden = true;
			}
		}
	}
}

void UCreditsWidget::UpdateCreditsText()
{
	++creditsIndex; // Increment the credits index

	// Update role and team member texts based on updated credits index
	switch (creditsIndex)
	{
	case 1:
		if (RoleText) RoleText->SetText(FText::FromString(TEXT("Voice Actor")));
		if (TeamMemberText) TeamMemberText->SetText(FText::FromString(TEXT("Jond")));
		break;

	case 2:
		if (RoleText) RoleText->SetText(FText::FromString(TEXT("Writer")));
		if (TeamMemberText) TeamMemberText->SetText(FText::FromString(TEXT("Jond")));
		break;

	case 3:
		creditsIndex = 0;

		if (RoleText) RoleText->SetText(FText::FromString(TEXT("C++ Programmer")));
		if (TeamMemberText) TeamMemberText->SetText(FText::FromString(TEXT("Osama Hussein")));
		break;

	default:
		creditsIndex = 0;

		if (RoleText) RoleText->SetText(FText::FromString(TEXT("C++ Programmer")));
		if (TeamMemberText) TeamMemberText->SetText(FText::FromString(TEXT("Osama Hussein")));
		break;
	}
}