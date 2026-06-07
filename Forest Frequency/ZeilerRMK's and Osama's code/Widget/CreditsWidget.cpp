// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/CreditsWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager/WidgetManager.h"
#include "Widget/MainMenuWidget.h"
#include "Animation/WidgetAnimation.h"

UCreditsWidget::UCreditsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RoleText && !RoleStringTexts.IsEmpty())
	{
		RoleText->SetOpacity(textOpacity);
		RoleText->SetText(FText::FromString(RoleStringTexts[0]));
	}

	if (TeamMemberText && !TeamMemberStringTexts.IsEmpty())
	{
		TeamMemberText->SetOpacity(textOpacity);
		TeamMemberText->SetText(FText::FromString(TeamMemberStringTexts[0]));
	}
	
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(showCreditsTextHandle, this, &UCreditsWidget::ShowCreditsText,
			GetWorld()->GetDeltaSeconds(), true);
	}

	if (BackButton) BackButton->OnPressed.AddUniqueDynamic(this, &UCreditsWidget::BackToMainMenu);
}

void UCreditsWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (!RoleStringTexts.IsEmpty()) RoleStringTexts.Empty();
	if (!TeamMemberStringTexts.IsEmpty()) TeamMemberStringTexts.Empty();

	if (GetWorld()->GetTimerManager().IsTimerActive(showCreditsTextHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(showCreditsTextHandle);
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(hideCreditsTextHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(hideCreditsTextHandle);
	}

	if (TeamMemberText)
	{
		TeamMemberText->RemoveFromParent();
		TeamMemberText = nullptr;
	}

	if (RoleText)
	{
		RoleText->RemoveFromParent();
		RoleText = nullptr;
	}

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
	}

	if (HookImage)
	{
		HookImage->RemoveFromParent();
		HookImage = nullptr;
	}

	if (CreditsCanvas)
	{
		CreditsCanvas->RemoveFromParent();
		CreditsCanvas = nullptr;
	}
}

void UCreditsWidget::ShowCreditsText()
{
	textOpacity += GetWorld()->GetDeltaSeconds(); // Increase text opacity

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

		GetWorld()->GetTimerManager().ClearTimer(showCreditsTextHandle);

		GetWorld()->GetTimerManager().SetTimer(hideCreditsTextHandle, this, &UCreditsWidget::HideCreditsText, 
			GetWorld()->GetDeltaSeconds(), true);
	}
}

void UCreditsWidget::HideCreditsText()
{
	textOpacity -= GetWorld()->GetDeltaSeconds(); // Decrease text opacity

	// If the text opacity is greater than 0, update the credits texts opacity
	if (textOpacity > 0.0f)
	{
		if (RoleText) RoleText->SetOpacity(textOpacity);
		if (TeamMemberText) TeamMemberText->SetOpacity(textOpacity);
	}

	// Text opacity exceeds the negative value of max show text time, try to show the text and update credits text
	else if (textOpacity <= -maxShowTextTime)
	{
		textOpacity = 0.0f;

		if (creditsIndex < RoleStringTexts.Num() - 1 &&
			creditsIndex < TeamMemberStringTexts.Num() - 1) // Less than final index
		{
			++creditsIndex; // Increment the credits index

			if (RoleText && !RoleStringTexts.IsEmpty())
			{
				RoleText->SetOpacity(textOpacity);
				RoleText->SetText(FText::FromString(RoleStringTexts[creditsIndex]));
			}

			if (TeamMemberText && !TeamMemberStringTexts.IsEmpty())
			{
				TeamMemberText->SetOpacity(textOpacity);
				TeamMemberText->SetText(FText::FromString(TeamMemberStringTexts[creditsIndex]));
			}
		}

		else // Reached final index
		{
			creditsIndex = 0; // Restart at index 0

			if (RoleText && !RoleStringTexts.IsEmpty())
			{
				RoleText->SetOpacity(textOpacity);
				RoleText->SetText(FText::FromString(RoleStringTexts[creditsIndex]));
			}

			if (TeamMemberText && !TeamMemberStringTexts.IsEmpty())
			{
				TeamMemberText->SetOpacity(textOpacity);
				TeamMemberText->SetText(FText::FromString(TeamMemberStringTexts[creditsIndex]));
			}
		}

		GetWorld()->GetTimerManager().ClearTimer(hideCreditsTextHandle);

		GetWorld()->GetTimerManager().SetTimer(showCreditsTextHandle, this, &UCreditsWidget::ShowCreditsText,
			GetWorld()->GetDeltaSeconds(), true);
	}
}

void UCreditsWidget::BackToMainMenu()
{
	RemoveBindedDelegates();

	if (!PressBackAnimation) return;

	PlayAnimation(PressBackAnimation);

	FTimerHandle animationHandle{};
	GetWorld()->GetTimerManager().SetTimer(animationHandle, this, &UCreditsWidget::TransitionToMainMenu,
		PressBackAnimation->GetEndTime(), false);
}

void UCreditsWidget::TransitionToMainMenu()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
	if (!widgetManager) return;

	widgetManager->PopWidget(widgetManager->GetCreditsClass());
	widgetManager->PushWidget(widgetManager->GetMainMenuClass());
}

void UCreditsWidget::RemoveBindedDelegates()
{
	if (BackButton) BackButton->OnPressed.RemoveDynamic(this, &UCreditsWidget::BackToMainMenu);
}
