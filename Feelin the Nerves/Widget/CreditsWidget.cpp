// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/CreditsWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/GoedWareCommunityGM.h"

UCreditsWidget::UCreditsWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CreditsMenuCanvas && CreditsMenuCanvas->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		CreditsMenuCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (CreditsTitleText && CreditsTitleText->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		CreditsTitleText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (BackButton && BackButton->GetVisibility() != ESlateVisibility::Visible)
	{
		BackButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (RoleText)
	{
		if (RoleText->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
			RoleText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		RoleText->SetOpacity(textOpacity);

		if (RoleTextStrings.IsValidIndex(creditsIndex)) RoleText->SetText(FText::FromString(RoleTextStrings[creditsIndex]));
	}

	if (TeamMemberText)
	{
		if (TeamMemberText->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
			TeamMemberText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		TeamMemberText->SetOpacity(textOpacity);

		if (TeamMemberTextStrings.IsValidIndex(creditsIndex)) TeamMemberText->SetText(FText::FromString(TeamMemberTextStrings[creditsIndex]));
	}

	// Assert that the array size of the role text strings is the same as team member text strings
	check(RoleTextStrings.Num() == TeamMemberTextStrings.Num());

	// Bind the back button's clicked and pressed delegates
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UCreditsWidget::GoBackToMainMenu);
		BackButton->OnPressed.AddDynamic(this, &UCreditsWidget::GoBackToMainMenu);
	}

	// Animate the credits text every frame
	FTimerHandle timerHandler;
	GetWorld()->GetTimerManager().SetTimer(timerHandler, this, &UCreditsWidget::AnimateCreditsText, 
		GetWorld()->GetDeltaSeconds(), true);
}

/*void UCreditsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}*/

void UCreditsWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (!RoleTextStrings.IsEmpty()) RoleTextStrings.Empty();
	if (!TeamMemberTextStrings.IsEmpty()) TeamMemberTextStrings.Empty();

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
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

	if (CreditsTitleText)
	{
		CreditsTitleText->RemoveFromParent();
		CreditsTitleText = nullptr;
	}

	if (CreditsMenuCanvas)
	{
		CreditsMenuCanvas->RemoveFromParent();
		CreditsMenuCanvas = nullptr;
	}
}

void UCreditsWidget::AnimateCreditsText()
{
	if (textHidden)
	{
		hideTextTimer += GetWorld()->GetDeltaSeconds();

		if (hideTextTimer >= maxHideTextTime) // Hide text timer exceeds max hide text time
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

				hideTextTimer = 0.0f;
				textHidden = false;
			}
		}
	}

	else
	{
		showTextTimer += GetWorld()->GetDeltaSeconds();

		if (showTextTimer >= maxShowTextTime)
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
	if (creditsIndex < RoleTextStrings.Num() - 1) ++creditsIndex; // Increment the credits index
	else creditsIndex = 0; // Reset its index back o 0 after reaching the last index of text strings

	if (RoleText && RoleTextStrings.IsValidIndex(creditsIndex))
	{
		RoleText->SetText(FText::FromString(RoleTextStrings[creditsIndex]));
	}

	if (TeamMemberText && TeamMemberTextStrings.IsValidIndex(creditsIndex))
	{
		TeamMemberText->SetText(FText::FromString(TeamMemberTextStrings[creditsIndex]));
	}
}

void UCreditsWidget::GoBackToMainMenu()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetCreditsClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());
	}

	RemoveBindedButtonDelegates();
}

void UCreditsWidget::RemoveBindedButtonDelegates()
{
	if (BackButton)
	{
		BackButton->OnClicked.RemoveDynamic(this, &UCreditsWidget::GoBackToMainMenu);
		BackButton->OnPressed.RemoveDynamic(this, &UCreditsWidget::GoBackToMainMenu);
	}
}
