// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/UpgradeMenuWidget.h"
#include "Player/PlayerCharacter.h"

UUpgradeMenuWidget::UUpgradeMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUpgradeMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		// Update ball speed upgrade title and information texts based on ball speed upgrade index on construct
		if (BallSpeedUpgradeTitle && BallSpeedUpgradeInformation)
		{
			InitializeBallSpeedTexts(playerCharacter);
		}

		// Update goalkeeper speed upgrade title and information texts based on goalkeeper speed upgrade index on construct
		if (GoalkeeperSpeedUpgradeTitle && GoalkeeperSpeedUpgradeInformation)
		{
			InitializeGoalkeeperSpeedTexts(playerCharacter);
		}

		if (EXPMultiplierUpgradeTitle && EXPMultiplierUpgradeInformation)
		{
			InitializeEXPMultiplierTexts(playerCharacter);
		}

		// Set player money text on construct
		if (PlayerMoneyText)
		{
			PlayerMoneyText->SetText(FText::FromString(FString::FromInt(playerCharacter->GetUpgradePoint()) + 
				TEXT(" upgrade points available")));
		}

		playerCharacter = nullptr;
	}

	// Bind clicked button delegates
	if (ContinueButton)
	{
		ContinueButton->OnHovered.AddUniqueDynamic(this, &UUpgradeMenuWidget::HoverOnContinueButton);
		ContinueButton->OnClicked.AddUniqueDynamic(this, &UUpgradeMenuWidget::ContinueToLeaderboard);
	}

	if (EXPMultiplierUpgradeButton)
	{
		EXPMultiplierUpgradeButton->OnHovered.AddUniqueDynamic(this, &UUpgradeMenuWidget::HoverOnEXPMultiplierUpgrade);
		EXPMultiplierUpgradeButton->OnClicked.AddUniqueDynamic(this, &UUpgradeMenuWidget::UpgradeEXPMultiplier);
	}

	if (GoalkeeperSpeedUpgradeButton)
	{
		GoalkeeperSpeedUpgradeButton->OnHovered.AddUniqueDynamic(this, &UUpgradeMenuWidget::HoverOnGoalkeeperSpeedUpgrade);
		GoalkeeperSpeedUpgradeButton->OnClicked.AddUniqueDynamic(this, &UUpgradeMenuWidget::UpgradeGoalkeeperSpeed);
	}

	if (SpeedBallUpgradeButton)
	{
		SpeedBallUpgradeButton->OnHovered.AddUniqueDynamic(this, &UUpgradeMenuWidget::HoverOnBallSpeedUpgrade);
		SpeedBallUpgradeButton->OnClicked.AddUniqueDynamic(this, &UUpgradeMenuWidget::UpgradeBallSpeed);
	}

	// Continue text
	if (ContinueText) ContinueText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
}

void UUpgradeMenuWidget::ContinueToLeaderboard()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		RemoveFromParent();

		playerCharacter->InvalidateSoccerBall();
		playerCharacter->ShowLeaderboardMenu();
		playerCharacter = nullptr;
	}

	if (ContinueButton)
	{
		ContinueButton->OnHovered.RemoveDynamic(this, &UUpgradeMenuWidget::HoverOnContinueButton);
		ContinueButton->OnClicked.RemoveDynamic(this, &UUpgradeMenuWidget::ContinueToLeaderboard);
	}

	if (EXPMultiplierUpgradeButton)
	{
		EXPMultiplierUpgradeButton->OnHovered.RemoveDynamic(this, &UUpgradeMenuWidget::HoverOnEXPMultiplierUpgrade);
		EXPMultiplierUpgradeButton->OnClicked.RemoveDynamic(this, &UUpgradeMenuWidget::UpgradeEXPMultiplier);
	}

	if (GoalkeeperSpeedUpgradeButton)
	{
		GoalkeeperSpeedUpgradeButton->OnHovered.RemoveDynamic(this, &UUpgradeMenuWidget::HoverOnGoalkeeperSpeedUpgrade);
		GoalkeeperSpeedUpgradeButton->OnClicked.RemoveDynamic(this, &UUpgradeMenuWidget::UpgradeGoalkeeperSpeed);
	}

	if (SpeedBallUpgradeButton)
	{
		SpeedBallUpgradeButton->OnHovered.RemoveDynamic(this, &UUpgradeMenuWidget::HoverOnBallSpeedUpgrade);
		SpeedBallUpgradeButton->OnClicked.RemoveDynamic(this, &UUpgradeMenuWidget::UpgradeBallSpeed);
	}
}

void UUpgradeMenuWidget::UpgradeBallSpeed()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		FSlateFontInfo font = BallSpeedUpgradeTitle->GetFont();

		switch (playerCharacter->GetBallSpeedUpgradeIndex())
		{
		case 1:

			if (playerCharacter->GetUpgradePoint() >= 1.0f)
			{
				playerCharacter->UpgradeBallSpeed();
				playerCharacter->DecreaseUpgradePoint(1.0f);

				BallSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Ball Speed Upgrade ") +
					FString::FromInt(playerCharacter->GetBallSpeedUpgradeIndex())));

				BallSpeedUpgradeInformation->SetText(FText::FromString(TEXT("Upgrade ball speed by 10 points")));

				if (PlayerMoneyText)
				{
					PlayerMoneyText->SetText(FText::FromString(FString::FromInt(playerCharacter->GetUpgradePoint()) +
						TEXT(" upgrade points available")));
				}
			}

			playerCharacter = nullptr;

			break;

		case 2:

			if (playerCharacter->GetUpgradePoint() >= 1.0f)
			{
				playerCharacter->UpgradeBallSpeed();
				playerCharacter->DecreaseUpgradePoint(1.0f);

				BallSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Ball Speed Upgrade ") +
					FString::FromInt(playerCharacter->GetBallSpeedUpgradeIndex())));

				BallSpeedUpgradeInformation->SetText(FText::FromString(TEXT("Upgrade ball speed by 15 points")));

				if (PlayerMoneyText)
				{
					PlayerMoneyText->SetText(FText::FromString(FString::FromInt(playerCharacter->GetUpgradePoint()) +
						TEXT(" upgrade points available")));
				}
			}

			playerCharacter = nullptr;

			break;

		case 3:

			if (playerCharacter->GetUpgradePoint() >= 1.0f)
			{
				playerCharacter->UpgradeBallSpeed();
				playerCharacter->DecreaseUpgradePoint(1.0f);

				BallSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Received all ball speed upgrades!")));
				font.Size = 15;

				BallSpeedUpgradeTitle->SetFont(font);

				if (BallSpeedUpgradeInformation)
				{
					BallSpeedUpgradeInformation->RemoveFromParent();
					BallSpeedUpgradeInformation->ConditionalBeginDestroy();
					BallSpeedUpgradeInformation = nullptr;
				}

				if (SpeedBallUpgradeButton) SpeedBallUpgradeButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));

				if (PlayerMoneyText)
				{
					PlayerMoneyText->SetText(FText::FromString(FString::FromInt(playerCharacter->GetUpgradePoint()) +
						TEXT(" upgrade points available")));
				}
			}

			playerCharacter = nullptr;

			break;

		default:

			playerCharacter = nullptr;
			break;
		}
	}
}

void UUpgradeMenuWidget::UpgradeGoalkeeperSpeed()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		FSlateFontInfo font = GoalkeeperSpeedUpgradeTitle->GetFont();

		switch (playerCharacter->GetGoalkeeperSpeedUpgradeIndex())
		{
		case 1:

			if (playerCharacter->GetUpgradePoint() >= 1.0f)
			{
				playerCharacter->UpgradeGoalkeeperSpeed();
				playerCharacter->DecreaseUpgradePoint(1.0f);

				GoalkeeperSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Goalkeeper Speed Upgrade ") +
					FString::FromInt(playerCharacter->GetGoalkeeperSpeedUpgradeIndex())));

				GoalkeeperSpeedUpgradeInformation->SetText(FText::FromString(TEXT("Lower goalkeeper speed by 10%")));

				if (PlayerMoneyText)
				{
					PlayerMoneyText->SetText(FText::FromString(FString::FromInt(playerCharacter->GetUpgradePoint()) +
						TEXT(" upgrade points available")));
				}
			}

			playerCharacter = nullptr;

			break;

		case 2:

			if (playerCharacter->GetUpgradePoint() >= 1.0f)
			{
				playerCharacter->UpgradeGoalkeeperSpeed();
				playerCharacter->DecreaseUpgradePoint(1.0f);

				GoalkeeperSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Goalkeeper Speed Upgrade ") +
					FString::FromInt(playerCharacter->GetGoalkeeperSpeedUpgradeIndex())));

				GoalkeeperSpeedUpgradeInformation->SetText(FText::FromString(TEXT("Lower goalkeeper speed by 20%")));

				if (PlayerMoneyText)
				{
					PlayerMoneyText->SetText(FText::FromString(FString::FromInt(playerCharacter->GetUpgradePoint()) +
						TEXT(" upgrade points available")));
				}
			}

			playerCharacter = nullptr;

			break;

		case 3:

			if (playerCharacter->GetUpgradePoint() >= 1.0f)
			{
				playerCharacter->UpgradeGoalkeeperSpeed();
				playerCharacter->DecreaseUpgradePoint(1.0f);

				GoalkeeperSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Received all goalkeeper speed upgrades!")));
				font.Size = 15;

				GoalkeeperSpeedUpgradeTitle->SetFont(font);

				if (GoalkeeperSpeedUpgradeInformation)
				{
					GoalkeeperSpeedUpgradeInformation->RemoveFromParent();
					GoalkeeperSpeedUpgradeInformation->ConditionalBeginDestroy();
					GoalkeeperSpeedUpgradeInformation = nullptr;
				}

				if (GoalkeeperSpeedUpgradeButton) GoalkeeperSpeedUpgradeButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));

				if (PlayerMoneyText)
				{
					PlayerMoneyText->SetText(FText::FromString(FString::FromInt(playerCharacter->GetUpgradePoint()) +
						TEXT(" upgrade points available")));
				}
			}

			playerCharacter = nullptr;

			break;

		default:

			playerCharacter = nullptr;
			break;
		}
	}
}

void UUpgradeMenuWidget::UpgradeEXPMultiplier()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		FSlateFontInfo font = EXPMultiplierUpgradeTitle->GetFont();

		switch (playerCharacter->GetEXPMultiplierUpgradeIndex())
		{
		case 1:

			if (playerCharacter->GetUpgradePoint() >= 1.0f)
			{
				playerCharacter->UpgradeEXPMultiplier();
				playerCharacter->DecreaseUpgradePoint(1.0f);

				EXPMultiplierUpgradeTitle->SetText(FText::FromString(TEXT("EXP Multiplier Upgrade ") +
					FString::FromInt(playerCharacter->GetEXPMultiplierUpgradeIndex())));

				EXPMultiplierUpgradeInformation->SetText(FText::FromString(TEXT("Triple the amount of XP you'll earn")));

				if (PlayerMoneyText)
				{
					PlayerMoneyText->SetText(FText::FromString(FString::FromInt(playerCharacter->GetUpgradePoint()) +
						TEXT(" upgrade points available")));
				}
			}

			playerCharacter = nullptr;

			break;

		case 2:

			if (playerCharacter->GetUpgradePoint() >= 1.0f)
			{
				playerCharacter->UpgradeEXPMultiplier();
				playerCharacter->DecreaseUpgradePoint(1.0f);

				EXPMultiplierUpgradeTitle->SetText(FText::FromString(TEXT("Received all XP upgrades!")));
				font.Size = 15;

				EXPMultiplierUpgradeTitle->SetFont(font);

				if (EXPMultiplierUpgradeInformation)
				{
					EXPMultiplierUpgradeInformation->RemoveFromParent();
					EXPMultiplierUpgradeInformation->ConditionalBeginDestroy();
					EXPMultiplierUpgradeInformation = nullptr;
				}

				if (EXPMultiplierUpgradeButton) EXPMultiplierUpgradeButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));

				if (PlayerMoneyText)
				{
					PlayerMoneyText->SetText(FText::FromString(FString::FromInt(playerCharacter->GetUpgradePoint()) +
						TEXT(" upgrade points available")));
				}
			}

			playerCharacter = nullptr;

			break;

		default:

			playerCharacter = nullptr;
			break;
		}
	}
}

void UUpgradeMenuWidget::HoverOnBallSpeedUpgrade()
{
	if (ContinueText && ContinueText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		ContinueText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftContinueImage && LeftContinueImage->IsVisible()) LeftContinueImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightContinueImage && RightContinueImage->IsVisible()) RightContinueImage->SetVisibility(ESlateVisibility::Hidden);
}

void UUpgradeMenuWidget::HoverOnGoalkeeperSpeedUpgrade()
{
	if (ContinueText && ContinueText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		ContinueText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftContinueImage && LeftContinueImage->IsVisible()) LeftContinueImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightContinueImage && RightContinueImage->IsVisible()) RightContinueImage->SetVisibility(ESlateVisibility::Hidden);
}

void UUpgradeMenuWidget::HoverOnEXPMultiplierUpgrade()
{
	if (ContinueText && ContinueText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		ContinueText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftContinueImage && LeftContinueImage->IsVisible()) LeftContinueImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightContinueImage && RightContinueImage->IsVisible()) RightContinueImage->SetVisibility(ESlateVisibility::Hidden);
}

void UUpgradeMenuWidget::HoverOnContinueButton()
{
	if (ContinueText && ContinueText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		ContinueText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}

	if (LeftContinueImage && !LeftContinueImage->IsVisible()) LeftContinueImage->SetVisibility(ESlateVisibility::Visible);
	if (RightContinueImage && !RightContinueImage->IsVisible()) RightContinueImage->SetVisibility(ESlateVisibility::Visible);
}

void UUpgradeMenuWidget::InitializeBallSpeedTexts(TWeakObjectPtr<APlayerCharacter> playerCharacter_)
{
	FSlateFontInfo font = BallSpeedUpgradeTitle->GetFont();

	switch (playerCharacter_->GetBallSpeedUpgradeIndex())
	{
	case 1:

		BallSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Ball Speed Upgrade ") +
			FString::FromInt(playerCharacter_->GetBallSpeedUpgradeIndex())));

		BallSpeedUpgradeInformation->SetText(FText::FromString(TEXT("Upgrade ball speed by 5 points")));

		break;

	case 2:

		BallSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Ball Speed Upgrade ") +
			FString::FromInt(playerCharacter_->GetBallSpeedUpgradeIndex())));

		BallSpeedUpgradeInformation->SetText(FText::FromString(TEXT("Upgrade ball speed by 10 points")));

		break;

	case 3:

		BallSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Ball Speed Upgrade ") +
			FString::FromInt(playerCharacter_->GetBallSpeedUpgradeIndex()) + TEXT(" - $20")));

		BallSpeedUpgradeInformation->SetText(FText::FromString(TEXT("Upgrade ball speed by 20 points")));

		break;

	case 4:

		BallSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Received all ball speed upgrades!")));

		font.Size = 15;

		BallSpeedUpgradeTitle->SetFont(font);

		if (SpeedBallUpgradeButton) SpeedBallUpgradeButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));

		BallSpeedUpgradeInformation->RemoveFromParent();
		BallSpeedUpgradeInformation->ConditionalBeginDestroy();
		BallSpeedUpgradeInformation = nullptr;

		break;

	default:
		break;
	}
}

void UUpgradeMenuWidget::InitializeGoalkeeperSpeedTexts(TWeakObjectPtr<class APlayerCharacter> playerCharacter_)
{
	FSlateFontInfo font = GoalkeeperSpeedUpgradeTitle->GetFont();

	switch (playerCharacter_->GetGoalkeeperSpeedUpgradeIndex())
	{
	case 1:

		GoalkeeperSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Goalkeeper Speed Upgrade ") +
			FString::FromInt(playerCharacter_->GetGoalkeeperSpeedUpgradeIndex())));

		GoalkeeperSpeedUpgradeInformation->SetText(FText::FromString(TEXT("Lower goalkeeper speed by 10%")));

		break;

	case 2:

		GoalkeeperSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Goalkeeper Speed Upgrade ") +
			FString::FromInt(playerCharacter_->GetGoalkeeperSpeedUpgradeIndex())));

		GoalkeeperSpeedUpgradeInformation->SetText(FText::FromString(TEXT("Lower goalkeeper speed by 10%")));

		break;

	case 3:

		GoalkeeperSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Goalkeeper Speed Upgrade ") +
			FString::FromInt(playerCharacter_->GetGoalkeeperSpeedUpgradeIndex())));

		GoalkeeperSpeedUpgradeInformation->SetText(FText::FromString(TEXT("Lower goalkeeper speed by 20%")));

		break;

	case 4:

		GoalkeeperSpeedUpgradeTitle->SetText(FText::FromString(TEXT("Received all goalkeeper speed upgrades!")));

		font.Size = 15;

		GoalkeeperSpeedUpgradeTitle->SetFont(font);
		
		if (GoalkeeperSpeedUpgradeButton) GoalkeeperSpeedUpgradeButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));

		GoalkeeperSpeedUpgradeInformation->RemoveFromParent();
		GoalkeeperSpeedUpgradeInformation->ConditionalBeginDestroy();
		GoalkeeperSpeedUpgradeInformation = nullptr;

		break;

	default:
		break;
	}
}

void UUpgradeMenuWidget::InitializeEXPMultiplierTexts(TWeakObjectPtr<class APlayerCharacter> playerCharacter_)
{
	FSlateFontInfo font = EXPMultiplierUpgradeTitle->GetFont();

	switch (playerCharacter_->GetEXPMultiplierUpgradeIndex())
	{
	case 1:

		EXPMultiplierUpgradeTitle->SetText(FText::FromString(TEXT("EXP Multiplier Upgrade ") +
			FString::FromInt(playerCharacter_->GetEXPMultiplierUpgradeIndex())));

		EXPMultiplierUpgradeInformation->SetText(FText::FromString(TEXT("Double the amount of XP you'll earn")));

		break;

	case 2:

		EXPMultiplierUpgradeTitle->SetText(FText::FromString(TEXT("EXP Multiplier Upgrade ") +
			FString::FromInt(playerCharacter_->GetEXPMultiplierUpgradeIndex())));

		EXPMultiplierUpgradeInformation->SetText(FText::FromString(TEXT("Triple the amount of XP you'll earn")));

		break;

	case 3:

		EXPMultiplierUpgradeTitle->SetText(FText::FromString(TEXT("Received all XP upgrades!")));

		font.Size = 15;

		EXPMultiplierUpgradeTitle->SetFont(font);

		if (EXPMultiplierUpgradeButton) EXPMultiplierUpgradeButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.5f));

		EXPMultiplierUpgradeInformation->RemoveFromParent();
		EXPMultiplierUpgradeInformation->ConditionalBeginDestroy();
		EXPMultiplierUpgradeInformation = nullptr;

		break;

	default:
		break;
	}
}
