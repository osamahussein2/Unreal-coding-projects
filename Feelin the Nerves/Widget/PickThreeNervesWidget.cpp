// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PickThreeNervesWidget.h"
#include "GameMode/GoedWareCommunityGM.h"
#include "Kismet/GameplayStatics.h"

UPickThreeNervesWidget::UPickThreeNervesWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPickThreeNervesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (nerveOneButton && nerveOneButton->GetVisibility() != ESlateVisibility::Visible)
	{
		nerveOneButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (nerveTwoButton && nerveTwoButton->GetVisibility() != ESlateVisibility::Visible)
	{
		nerveTwoButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (nerveThreeButton && nerveThreeButton->GetVisibility() != ESlateVisibility::Visible)
	{
		nerveThreeButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (restartSequenceButton && restartSequenceButton->GetVisibility() != ESlateVisibility::Visible)
	{
		restartSequenceButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (restartSequenceButton)
	{
		restartSequenceButton->OnPressed.AddDynamic(this, &UPickThreeNervesWidget::RestartNerveSequence);
	}

	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (sequencesText)
		{
			if (sequencesText->GetVisibility() != ESlateVisibility::Visible)
				sequencesText->SetVisibility(ESlateVisibility::Visible);

			sequencesText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPickedSequences()) + TEXT("/") + 
				FString::FromInt(gameMode->GetMaxPlayedSequences())));
		}
	}

	SetupNerveOneButtonImageDependingOnNerve();
}

void UPickThreeNervesWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (nerveOneButton)
	{
		nerveOneButton->RemoveFromParent();
		nerveOneButton = nullptr;
	}

	if (nerveTwoButton)
	{
		nerveTwoButton->RemoveFromParent();
		nerveTwoButton = nullptr;
	}

	if (nerveThreeButton)
	{
		nerveThreeButton->RemoveFromParent();
		nerveThreeButton = nullptr;
	}
}

void UPickThreeNervesWidget::RemoveBindedButtonDelegates()
{
	if (nerveOneButton)
	{
		nerveOneButton->OnPressed.RemoveDynamic(this, &UPickThreeNervesWidget::NerveOneSelected);
	}

	if (nerveTwoButton)
	{
		nerveTwoButton->OnPressed.RemoveDynamic(this, &UPickThreeNervesWidget::NerveTwoSelected);
	}

	if (nerveThreeButton)
	{
		nerveThreeButton->OnPressed.RemoveDynamic(this, &UPickThreeNervesWidget::NerveThreeSelected);
	}

	if (restartSequenceButton)
	{
		restartSequenceButton->OnPressed.RemoveDynamic(this, &UPickThreeNervesWidget::RestartNerveSequence);
	}
}

void UPickThreeNervesWidget::SetupNerveOneButtonImageDependingOnNerve()
{
	if (nerveOneButton)
	{
		FButtonStyle buttonStyle;
		FSlateBrush normalBrush, hoveredBrush, pressedBrush;

		normalBrush.TintColor = FSlateColor(FLinearColor(0.495466f, 0.495466f, 0.495466f));
		hoveredBrush.TintColor = FSlateColor(FLinearColor(0.724268f, 0.724268f, 0.724268f));
		pressedBrush.TintColor = FSlateColor(FLinearColor(0.384266f, 0.384266f, 0.384266f));

		normalBrush.SetImageSize(nerveImageSize);
		hoveredBrush.SetImageSize(nerveImageSize);
		pressedBrush.SetImageSize(nerveImageSize);

		/*normalBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
		hoveredBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
		pressedBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

		normalBrush.OutlineSettings.CornerRadii = FVector4(4.0f, 4.0f, 4.0f, 4.0f);
		normalBrush.OutlineSettings.Color = FSlateColor(FLinearColor(0.695f, 0.695f, 0.695f));
		normalBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
		normalBrush.OutlineSettings.bUseBrushTransparency = true;
		normalBrush.OutlineSettings.Width = 1.0f;

		hoveredBrush.OutlineSettings.CornerRadii = FVector4(4.0f, 4.0f, 4.0f, 4.0f);
		hoveredBrush.OutlineSettings.Color = FSlateColor(FLinearColor(0.695f, 0.695f, 0.695f));
		hoveredBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
		hoveredBrush.OutlineSettings.bUseBrushTransparency = true;
		hoveredBrush.OutlineSettings.Width = 1.0f;

		pressedBrush.OutlineSettings.CornerRadii = FVector4(4.0f, 4.0f, 4.0f, 4.0f);
		pressedBrush.OutlineSettings.Color = FSlateColor(FLinearColor(0.695f, 0.695f, 0.695f));
		pressedBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
		pressedBrush.OutlineSettings.bUseBrushTransparency = true;
		pressedBrush.OutlineSettings.Width = 1.0f;*/

		if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			const int& randomizedIndex = FMath::RandRange(0, 2);
			buttonOneTextureNumber = randomizedIndex;

			switch (gameMode->GetCurrentCompletedSequences())
			{
			case 0: // Blue nerves
				if (gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
				}

				break;

			case 1: // Orange nerves
				if (gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
				}

				break;

			case 2: // Purple nerves
				if (gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
				}

				break;

			case 3: // Yellow nerves
				if (gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonOneTextureNumber)));
				}

				break;
			}
		}

		if (sequencePressedSounds[0])
		{
			FSlateSound sound;
			sound.SetResourceObject(sequencePressedSounds[0]);

			buttonStyle.SetPressedSound(sound);
			buttonStyle.SetClickedSound(sound);
		}

		buttonStyle.SetNormal(normalBrush);
		buttonStyle.SetHovered(hoveredBrush);
		buttonStyle.SetPressed(pressedBrush);

		nerveOneButton->SetStyle(buttonStyle);

		nerveOneButton->OnPressed.AddDynamic(this, &UPickThreeNervesWidget::NerveOneSelected);

		SetupNerveTwoButtonImageDependingOnNerve();
	}
}

void UPickThreeNervesWidget::SetupNerveTwoButtonImageDependingOnNerve()
{
	if (nerveTwoButton)
	{
		FButtonStyle buttonStyle;
		FSlateBrush normalBrush, hoveredBrush, pressedBrush;

		normalBrush.TintColor = FSlateColor(FLinearColor(0.495466f, 0.495466f, 0.495466f));
		hoveredBrush.TintColor = FSlateColor(FLinearColor(0.724268f, 0.724268f, 0.724268f));
		pressedBrush.TintColor = FSlateColor(FLinearColor(0.384266f, 0.384266f, 0.384266f));

		normalBrush.SetImageSize(nerveImageSize);
		hoveredBrush.SetImageSize(nerveImageSize);
		pressedBrush.SetImageSize(nerveImageSize);

		/*normalBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
		hoveredBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
		pressedBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

		normalBrush.OutlineSettings.CornerRadii = FVector4(4.0f, 4.0f, 4.0f, 4.0f);
		normalBrush.OutlineSettings.Color = FSlateColor(FLinearColor(0.695f, 0.695f, 0.695f));
		normalBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
		normalBrush.OutlineSettings.bUseBrushTransparency = true;
		normalBrush.OutlineSettings.Width = 1.0f;

		hoveredBrush.OutlineSettings.CornerRadii = FVector4(4.0f, 4.0f, 4.0f, 4.0f);
		hoveredBrush.OutlineSettings.Color = FSlateColor(FLinearColor(0.695f, 0.695f, 0.695f));
		hoveredBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
		hoveredBrush.OutlineSettings.bUseBrushTransparency = true;
		hoveredBrush.OutlineSettings.Width = 1.0f;

		pressedBrush.OutlineSettings.CornerRadii = FVector4(4.0f, 4.0f, 4.0f, 4.0f);
		pressedBrush.OutlineSettings.Color = FSlateColor(FLinearColor(0.695f, 0.695f, 0.695f));
		pressedBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
		pressedBrush.OutlineSettings.bUseBrushTransparency = true;
		pressedBrush.OutlineSettings.Width = 1.0f;*/

		if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			const int& randomizedIndex = FMath::RandRange(0, 2);
			buttonTwoTextureNumber = randomizedIndex;

			// If the 2nd button texture has the same index as button one, re-execute this function
			if (buttonTwoTextureNumber == buttonOneTextureNumber)
			{
				SetupNerveTwoButtonImageDependingOnNerve();
				return;
			}
			
			switch (gameMode->GetCurrentCompletedSequences())
			{
			case 0: // Blue nerves
				if (gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
				}

				break;

			case 1: // Orange nerves
				if (gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
				}

				break;

			case 2: // Purple nerves
				if (gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
				}

				break;

			case 3: // Yellow nerves
				if (gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonTwoTextureNumber)));
				}

				break;
			}
		}

		if (sequencePressedSounds[1])
		{
			FSlateSound sound;
			sound.SetResourceObject(sequencePressedSounds[1]);

			buttonStyle.SetPressedSound(sound);
			buttonStyle.SetClickedSound(sound);
		}

		buttonStyle.SetNormal(normalBrush);
		buttonStyle.SetHovered(hoveredBrush);
		buttonStyle.SetPressed(pressedBrush);

		nerveTwoButton->SetStyle(buttonStyle);

		nerveTwoButton->OnPressed.AddDynamic(this, &UPickThreeNervesWidget::NerveTwoSelected);

		SetupNerveThreeButtonImageDependingOnNerve();
	}
}

void UPickThreeNervesWidget::SetupNerveThreeButtonImageDependingOnNerve()
{
	if (nerveThreeButton)
	{
		FButtonStyle buttonStyle;
		FSlateBrush normalBrush, hoveredBrush, pressedBrush;

		normalBrush.TintColor = FSlateColor(FLinearColor(0.495466f, 0.495466f, 0.495466f));
		hoveredBrush.TintColor = FSlateColor(FLinearColor(0.724268f, 0.724268f, 0.724268f));
		pressedBrush.TintColor = FSlateColor(FLinearColor(0.384266f, 0.384266f, 0.384266f));

		normalBrush.SetImageSize(nerveImageSize);
		hoveredBrush.SetImageSize(nerveImageSize);
		pressedBrush.SetImageSize(nerveImageSize);

		/*normalBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
		hoveredBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
		pressedBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

		normalBrush.OutlineSettings.CornerRadii = FVector4(4.0f, 4.0f, 4.0f, 4.0f);
		normalBrush.OutlineSettings.Color = FSlateColor(FLinearColor(0.695f, 0.695f, 0.695f));
		normalBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
		normalBrush.OutlineSettings.bUseBrushTransparency = true;
		normalBrush.OutlineSettings.Width = 1.0f;

		hoveredBrush.OutlineSettings.CornerRadii = FVector4(4.0f, 4.0f, 4.0f, 4.0f);
		hoveredBrush.OutlineSettings.Color = FSlateColor(FLinearColor(0.695f, 0.695f, 0.695f));
		hoveredBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
		hoveredBrush.OutlineSettings.bUseBrushTransparency = true;
		hoveredBrush.OutlineSettings.Width = 1.0f;

		pressedBrush.OutlineSettings.CornerRadii = FVector4(4.0f, 4.0f, 4.0f, 4.0f);
		pressedBrush.OutlineSettings.Color = FSlateColor(FLinearColor(0.695f, 0.695f, 0.695f));
		pressedBrush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
		pressedBrush.OutlineSettings.bUseBrushTransparency = true;
		pressedBrush.OutlineSettings.Width = 1.0f;*/

		if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			const int& randomizedIndex = FMath::RandRange(0, 2);
			buttonThreeTextureNumber = randomizedIndex;

			// If the 3rd button texture has the same index as button one or two, re-execute this function
			if (buttonThreeTextureNumber == buttonOneTextureNumber || buttonThreeTextureNumber == buttonTwoTextureNumber)
			{
				SetupNerveThreeButtonImageDependingOnNerve();
				return;
			}

			switch (gameMode->GetCurrentCompletedSequences())
			{
			case 0: // Blue nerves
				if (gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
				}

				break;

			case 1: // Orange nerves
				if (gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
				}

				break;

			case 2: // Purple nerves
				if (gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
				}

				break;

			case 3: // Yellow nerves
				if (gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonThreeTextureNumber)));
				}

				break;
			}
		}

		if (sequencePressedSounds[2])
		{
			FSlateSound sound;
			sound.SetResourceObject(sequencePressedSounds[2]);

			buttonStyle.SetPressedSound(sound);
			buttonStyle.SetClickedSound(sound);
		}

		buttonStyle.SetNormal(normalBrush);
		buttonStyle.SetHovered(hoveredBrush);
		buttonStyle.SetPressed(pressedBrush);

		nerveThreeButton->SetStyle(buttonStyle);

		nerveThreeButton->OnPressed.AddDynamic(this, &UPickThreeNervesWidget::NerveThreeSelected);
	}
}

void UPickThreeNervesWidget::RestartNerveSequence()
{
	RemoveBindedButtonDelegates();

	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetPlayerHUDClass());
		gameMode->PopWidget(gameMode->GetPickThreeNervesClass());
		gameMode->ResetCurrentSequenceStats(false);

		gameMode->PushWidget(gameMode->GetNerveSequenceClass());
	}
}

void UPickThreeNervesWidget::NerveOneSelected()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (gameMode->GetCurrentPickedSequences() == gameMode->FindRandomNerveAtReferencedIndex(buttonOneTextureNumber))
		{
			gameMode->IncrementCorrectSequences();
		}

		gameMode->IncrementPickedSequences();

		//UE_LOG(LogTemp, Warning, TEXT("%i/%i"), gameMode->GetCurrentCorrectSequences(), gameMode->GetMaxCorrectSequences());

		if (sequencesText)
		{
			sequencesText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPickedSequences()) + TEXT("/") +
				FString::FromInt(gameMode->GetMaxPlayedSequences())));
		}

		if (gameMode->GetCurrentPickedSequences() >= gameMode->GetMaxPlayedSequences())
		{
			RemoveBindedButtonDelegates();

			gameMode->PopWidget(gameMode->GetPlayerHUDClass());
			gameMode->PopWidget(gameMode->GetPickThreeNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::ENone);

			gameMode->PushWidget(gameMode->GetSequenceStatsClass());
		}
	}
}

void UPickThreeNervesWidget::NerveTwoSelected()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (gameMode->GetCurrentPickedSequences() == gameMode->FindRandomNerveAtReferencedIndex(buttonTwoTextureNumber))
		{
			gameMode->IncrementCorrectSequences();
		}

		gameMode->IncrementPickedSequences();

		//UE_LOG(LogTemp, Warning, TEXT("%i/%i"), gameMode->GetCurrentCorrectSequences(), gameMode->GetMaxCorrectSequences());

		if (sequencesText)
		{
			sequencesText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPickedSequences()) + TEXT("/") +
				FString::FromInt(gameMode->GetMaxPlayedSequences())));
		}

		if (gameMode->GetCurrentPickedSequences() >= gameMode->GetMaxPlayedSequences())
		{
			RemoveBindedButtonDelegates();

			gameMode->PopWidget(gameMode->GetPlayerHUDClass());
			gameMode->PopWidget(gameMode->GetPickThreeNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::ENone);

			gameMode->PushWidget(gameMode->GetSequenceStatsClass());
		}
	}
}

void UPickThreeNervesWidget::NerveThreeSelected()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (gameMode->GetCurrentPickedSequences() == gameMode->FindRandomNerveAtReferencedIndex(buttonThreeTextureNumber))
		{
			gameMode->IncrementCorrectSequences();
		}

		gameMode->IncrementPickedSequences();

		//UE_LOG(LogTemp, Warning, TEXT("%i/%i"), gameMode->GetCurrentCorrectSequences(), gameMode->GetMaxCorrectSequences());

		if (sequencesText)
		{
			sequencesText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPickedSequences()) + TEXT("/") +
				FString::FromInt(gameMode->GetMaxPlayedSequences())));
		}

		if (gameMode->GetCurrentPickedSequences() >= gameMode->GetMaxPlayedSequences())
		{
			RemoveBindedButtonDelegates();

			gameMode->PopWidget(gameMode->GetPlayerHUDClass());
			gameMode->PopWidget(gameMode->GetPickThreeNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::ENone);

			gameMode->PushWidget(gameMode->GetSequenceStatsClass());
		}
	}
}