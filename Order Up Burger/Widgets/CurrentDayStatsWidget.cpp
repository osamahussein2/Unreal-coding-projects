// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/CurrentDayStatsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Animation/WidgetAnimation.h"
#include "Player/PlayerCharacter.h"

UCurrentDayStatsWidget::UCurrentDayStatsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCurrentDayStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Just in case, the game isn't paused, pause the game using UGameplayStatics
	if (!UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->DivideUpAverageScore(gameMode->GetCustomerNumberArrayLength());
		gameMode->AddAverageScoreInformation();

		if (!gameMode->AreAllDaysCompleted())
		{
			if (CurrentDayStatsTitleText) CurrentDayStatsTitleText->SetText(FText::FromString(TEXT("Day ") +
				FString::FromInt(gameMode->GetCurrentDay()) + TEXT(" Stats")));

			CreateCustomerNumberTexts(gameMode);
			CreateCustomerScoreTexts(gameMode);
		}

		else
		{
			if (CurrentDayStatsTitleText) CurrentDayStatsTitleText->SetText(FText::FromString(TEXT("Day Score Averages")));

			CreateAverageScoreTexts(gameMode);
		}
	}

	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UCurrentDayStatsWidget::Continue);
		ContinueButton->OnPressed.AddDynamic(this, &UCurrentDayStatsWidget::Continue);
	}
}

void UCurrentDayStatsWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	switch (dayStatsTransitionState)
	{
	case DayStatsTransitionState::EContinue:
		Super::NativeTick(Geometry, DeltaTime);

		if (!IsAnimationPlaying(ContinueBurgerFlipAnimation))
		{
			if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				// Unpause the game using UGameplayStatics
				UGameplayStatics::SetGamePaused(GetWorld(), false);

				// Progress to the next day and show the current day widget
				if (!gameMode->AreAllDaysCompleted()) gameMode->InitializeCurrentDayWidget(true);
				else
				{
					if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
					{
						if (APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
						{
							// Enable player input again
							player->EnableInput(playerController);
						}
					}
				}

				// Then destroy the current day stats widget
				gameMode->DestroyCurrentDayStatsWidget();
			}
		}

		break;

	default:
		break;
	}
}

void UCurrentDayStatsWidget::NativeDestruct()
{
	Super::NativeDestruct();

	for (UTextBlock* CurrentDayCustomerNumberText : CurrentDayCustomerNumberTexts)
	{
		CurrentDayCustomerNumberText->RemoveFromParent();
		CurrentDayCustomerNumberText = nullptr;
	}

	for (UTextBlock* CurrentDayCustomerScoreText : CurrentDayCustomerScoreTexts)
	{
		CurrentDayCustomerScoreText->RemoveFromParent();
		CurrentDayCustomerScoreText = nullptr;
	}

	if (CurrentDayStatsTitleText)
	{
		CurrentDayStatsTitleText->RemoveFromParent();
		CurrentDayStatsTitleText = nullptr;
	}

	if (BlackImage)
	{
		ContinueButton->RemoveFromParent();
		ContinueButton = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (CurrentDayStatsCanvas)
	{
		CurrentDayStatsCanvas->RemoveFromParent();
		CurrentDayStatsCanvas = nullptr;
	}

	dayStatsTransitionState = DayStatsTransitionState::ENull;
}

void UCurrentDayStatsWidget::Continue()
{
	if (ContinueBurgerFlipAnimation) PlayAnimation(ContinueBurgerFlipAnimation);

	dayStatsTransitionState = DayStatsTransitionState::EContinue;

	if (ContinueButton->OnClicked.IsBound()) ContinueButton->OnClicked.RemoveDynamic(this, &UCurrentDayStatsWidget::Continue);
	if (ContinueButton->OnPressed.IsBound()) ContinueButton->OnPressed.RemoveDynamic(this, &UCurrentDayStatsWidget::Continue);
}

void UCurrentDayStatsWidget::CreateCustomerNumberTexts(APortfolioBuilders_GM* gameMode_)
{
	// Create customer number texts 
	for (int i = 0; i < gameMode_->GetCustomerNumberArrayLength(); ++i)
	{
		CurrentDayCustomerNumberTexts.Add(WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass()));

		FSlateFontInfo font = CurrentDayCustomerNumberTexts[i]->GetFont();
		font.Size = customerStatsFontSize;

		CurrentDayCustomerNumberTexts[i]->SetFont(font);

		CurrentDayCustomerNumberTexts[i]->SetText(FText::FromString(TEXT("Customer #") +
			FString::FromInt(gameMode_->GetCustomerNumber(i)) + TEXT(" Score: ")));

		CurrentDayStatsCanvas->AddChildToCanvas(CurrentDayCustomerNumberTexts[i]);

		if (UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(CurrentDayCustomerNumberTexts[i]->Slot))
		{
			canvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));

			canvasSlot->SetPosition(FVector2D(customerNumberTextsLocation.X, 
				customerNumberTextsLocation.Y + (i * customerStatsPaddingY)));

			canvasSlot->SetAlignment(FVector2D(1.0f, 0.5f));
			canvasSlot->SetAutoSize(true);
		}
	}
}

void UCurrentDayStatsWidget::CreateCustomerScoreTexts(APortfolioBuilders_GM* gameMode_)
{
	// Create customer score texts 
	for (int i = 0; i < gameMode_->GetCustomerScoreArrayLength(); ++i)
	{
		CurrentDayCustomerScoreTexts.Add(WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass()));

		FSlateFontInfo font = CurrentDayCustomerScoreTexts[i]->GetFont();
		font.Size = customerStatsFontSize;

		CurrentDayCustomerScoreTexts[i]->SetFont(font);

		CurrentDayCustomerScoreTexts[i]->SetText(FText::FromString(FString::FromInt(gameMode_->GetCustomerScore(i)) + TEXT("%")));

		CurrentDayStatsCanvas->AddChildToCanvas(CurrentDayCustomerScoreTexts[i]);

		if (UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(CurrentDayCustomerScoreTexts[i]->Slot))
		{
			canvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));

			canvasSlot->SetPosition(FVector2D(customerScoreTextsLocation.X,
				customerScoreTextsLocation.Y + (i * customerStatsPaddingY)));

			canvasSlot->SetAlignment(FVector2D(0.0f, 0.5f));
			canvasSlot->SetAutoSize(true);
		}
	}
}

void UCurrentDayStatsWidget::CreateAverageScoreTexts(APortfolioBuilders_GM* gameMode_)
{
	// Create average score texts 
	for (int i = 0; i < gameMode_->GetAverageScoreArrayLength(); ++i)
	{
		CurrentDayCustomerScoreTexts.Add(WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass()));

		FSlateFontInfo font = CurrentDayCustomerScoreTexts[i]->GetFont();
		font.Size = customerStatsFontSize;

		CurrentDayCustomerScoreTexts[i]->SetFont(font);

		CurrentDayCustomerScoreTexts[i]->SetText(FText::FromString(TEXT("Day ") + 
			FString::FromInt(i + 1) + TEXT(": ") +
			FString::FromInt(gameMode_->GetAverageScore(i)) + TEXT("%")));

		CurrentDayStatsCanvas->AddChildToCanvas(CurrentDayCustomerScoreTexts[i]);

		if (UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(CurrentDayCustomerScoreTexts[i]->Slot))
		{
			canvasSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));

			canvasSlot->SetPosition(FVector2D(averageScoreTextsLocation.X,
				averageScoreTextsLocation.Y + (i * averageStatsPaddingY)));

			canvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));
			canvasSlot->SetAutoSize(true);
		}
	}
}
