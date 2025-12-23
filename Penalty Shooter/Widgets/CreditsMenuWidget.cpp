// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/CreditsMenuWidget.h"
#include "Player/PlayerCharacter.h"

UCreditsMenuWidget::UCreditsMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCreditsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (backButton)
	{
		backButton->OnHovered.AddUniqueDynamic(this, &UCreditsMenuWidget::HoverOnBackButton);
		backButton->OnClicked.AddUniqueDynamic(this, &UCreditsMenuWidget::GoBackToMainMenu);
	}

	// Set back images to be visible
	if (LeftBackImage && !LeftBackImage->IsVisible()) LeftBackImage->SetVisibility(ESlateVisibility::Visible);
	if (RightBackImage && !RightBackImage->IsVisible()) RightBackImage->SetVisibility(ESlateVisibility::Visible);

	// Set back text color and opacity
	if (BackText && BackText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		BackText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}

	// Start playing the scrolling credits animation on construct
	if (ScrollCredits) PlayAnimation(ScrollCredits);
}

void UCreditsMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Check if the credits animation isn't playing to play the scrolling credits animation again
	if (ScrollCredits && !IsAnimationPlaying(ScrollCredits)) PlayAnimation(ScrollCredits);
}

void UCreditsMenuWidget::GoBackToMainMenu()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		RemoveFromParent();

		playerCharacter->ShowMainMenu(false);

		playerCharacter = nullptr;
	}

	backButton->OnHovered.RemoveDynamic(this, &UCreditsMenuWidget::HoverOnBackButton);
	backButton->OnClicked.RemoveDynamic(this, &UCreditsMenuWidget::GoBackToMainMenu);
}

void UCreditsMenuWidget::HoverOnBackButton()
{
	// Set back images to be visible
	if (LeftBackImage && !LeftBackImage->IsVisible()) LeftBackImage->SetVisibility(ESlateVisibility::Visible);
	if (RightBackImage && !RightBackImage->IsVisible()) RightBackImage->SetVisibility(ESlateVisibility::Visible);

	// Set back text color and opacity
	if (BackText && BackText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		BackText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}
}