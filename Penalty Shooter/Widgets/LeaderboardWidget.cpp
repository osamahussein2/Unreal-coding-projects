// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/LeaderboardWidget.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/PlayerInfoWidget.h"
#include "Blueprint/WidgetTree.h"

ULeaderboardWidget::ULeaderboardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ULeaderboardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (backButton)
	{
		backButton->OnHovered.AddUniqueDynamic(this, &ULeaderboardWidget::HoverOnBackButton);
		backButton->OnClicked.AddUniqueDynamic(this, &ULeaderboardWidget::GoBackToMainMenu);
	}

	if (resetButton)
	{
		resetButton->OnHovered.AddUniqueDynamic(this, &ULeaderboardWidget::HoverOnResetButton);
		resetButton->OnClicked.AddUniqueDynamic(this, &ULeaderboardWidget::ResetLeaderboard);
	}

	// Set button images to be visible/invisible
	if (LeftBackImage && !LeftBackImage->IsVisible()) LeftBackImage->SetVisibility(ESlateVisibility::Visible);
	if (RightBackImage && !RightBackImage->IsVisible()) RightBackImage->SetVisibility(ESlateVisibility::Visible);

	if (LeftResetImage && LeftResetImage->IsVisible()) LeftResetImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightResetImage && RightResetImage->IsVisible()) RightResetImage->SetVisibility(ESlateVisibility::Hidden);

	// Set text color and opacity
	if (BackText) BackText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	if (ResetText) ResetText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	if (playerInfoWidgetClass)
	{
		TArray<AActor*> foundPlayer;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

		TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

		foundPlayer[0] = nullptr;

		if (playerCharacter.IsValid())
		{
			for (int i = 0; i < playerCharacter->GetPlayerLeaderboardStatsSize(); i++)
			{
				if (!playerCharacter->GetNamesReceived(i))
				{
					playerInfoWidget = WidgetTree->ConstructWidget<UPlayerInfoWidget>(playerInfoWidgetClass);

					if (!playerInfoWidget) return;

					playerContainer->AddChildToVerticalBox(playerInfoWidget);

					playerInfoWidget->ShowEditableText();

					if (backButton) backButton->SetVisibility(ESlateVisibility::Hidden);
					if (resetButton) resetButton->SetVisibility(ESlateVisibility::Hidden);

					if (LeftBackImage) LeftBackImage->SetVisibility(ESlateVisibility::Hidden);
					if (RightBackImage) RightBackImage->SetVisibility(ESlateVisibility::Hidden);

					if (LeftResetImage) LeftResetImage->SetVisibility(ESlateVisibility::Hidden);
					if (RightResetImage) RightResetImage->SetVisibility(ESlateVisibility::Hidden);

					playerInfoWidget->SetLeaderboardButtons(backButton, resetButton, LeftBackImage, RightBackImage, 
						LeftResetImage, RightResetImage);

					playerInfoWidget = nullptr;
				}

				else
				{
					// Set all player names and highscores texts to the player's names and scores right away 
					playerInfoWidget = WidgetTree->ConstructWidget<UPlayerInfoWidget>(playerInfoWidgetClass);

					if (!playerInfoWidget) return;

					playerContainer->AddChildToVerticalBox(playerInfoWidget);

					playerInfoWidget->SetPlayerName(playerCharacter->GetPlayerNames(i));
					playerInfoWidget->SetPlayerScore(playerCharacter->GetPlayerScores(i));

					playerInfoWidget->ShowActualLeaderboard();

					playerInfoWidget = nullptr;
				}
			}

			playerCharacter = nullptr;
		}
	}
}

void ULeaderboardWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}

void ULeaderboardWidget::GoBackToMainMenu()
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

	if (backButton)
	{
		backButton->OnHovered.RemoveDynamic(this, &ULeaderboardWidget::HoverOnBackButton);
		backButton->OnClicked.RemoveDynamic(this, &ULeaderboardWidget::GoBackToMainMenu);
	}

	if (resetButton)
	{
		resetButton->OnHovered.RemoveDynamic(this, &ULeaderboardWidget::HoverOnResetButton);
		resetButton->OnClicked.RemoveDynamic(this, &ULeaderboardWidget::ResetLeaderboard);
	}
}

void ULeaderboardWidget::ResetLeaderboard()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		switch (playerCharacter->GetPlayerLeaderboardStatsSize())
		{
		case 1:
			// Hardcoded value to remove wherever the 1st player entry widget got instantiated at
			playerContainer->RemoveChildAt(0);
			break;

		case 2:
			for (int i = 0; i < 2; i++) if (playerContainer->HasAnyChildren()) playerContainer->RemoveChildAt(i);

		case 3:
			for (int i = 0; i < 3; i++) if (playerContainer->HasAnyChildren()) playerContainer->RemoveChildAt(i);

		case 4:
			for (int i = 0; i < 4; i++) if (playerContainer->HasAnyChildren()) playerContainer->RemoveChildAt(i);

		case 5:
			for (int i = 0; i < 5; i++) if (playerContainer->HasAnyChildren()) playerContainer->RemoveChildAt(i);

		case 6:
			for (int i = 0; i < 6; i++) if (playerContainer->HasAnyChildren()) playerContainer->RemoveChildAt(i);

		case 7:
			for (int i = 0; i < 7; i++) if (playerContainer->HasAnyChildren()) playerContainer->RemoveChildAt(i);

		case 8:
			for (int i = 0; i < 8; i++) if (playerContainer->HasAnyChildren()) playerContainer->RemoveChildAt(i);

		case 9:
			for (int i = 0; i < 9; i++) if (playerContainer->HasAnyChildren()) playerContainer->RemoveChildAt(i);

		case 10:
			for (int i = 0; i < 10; i++) if (playerContainer->HasAnyChildren()) playerContainer->RemoveChildAt(i);
			break;

		default:
			break;
		}

		playerCharacter->DeleteAllLeaderboardInformation();
		playerCharacter = nullptr;
	}
}

void ULeaderboardWidget::HoverOnBackButton()
{
	// Set button images to be visible/invisible
	if (LeftBackImage && !LeftBackImage->IsVisible()) LeftBackImage->SetVisibility(ESlateVisibility::Visible);
	if (RightBackImage && !RightBackImage->IsVisible()) RightBackImage->SetVisibility(ESlateVisibility::Visible);

	if (LeftResetImage && LeftResetImage->IsVisible()) LeftResetImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightResetImage && RightResetImage->IsVisible()) RightResetImage->SetVisibility(ESlateVisibility::Hidden);

	// Set text color and opacity
	if (BackText && BackText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		BackText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}

	if (ResetText && ResetText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		ResetText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void ULeaderboardWidget::HoverOnResetButton()
{
	// Set button images to be visible/invisible
	if (LeftBackImage && LeftBackImage->IsVisible()) LeftBackImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightBackImage && RightBackImage->IsVisible()) RightBackImage->SetVisibility(ESlateVisibility::Hidden);

	if (LeftResetImage && !LeftResetImage->IsVisible()) LeftResetImage->SetVisibility(ESlateVisibility::Visible);
	if (RightResetImage && !RightResetImage->IsVisible()) RightResetImage->SetVisibility(ESlateVisibility::Visible);

	// Set text color and opacity
	if (BackText && BackText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		BackText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (ResetText && ResetText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		ResetText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}
}