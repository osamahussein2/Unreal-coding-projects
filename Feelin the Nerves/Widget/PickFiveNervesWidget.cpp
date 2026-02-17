// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PickFiveNervesWidget.h"
#include "GameMode/GoedWareCommunityGM.h"
#include "Kismet/GameplayStatics.h"

UPickFiveNervesWidget::UPickFiveNervesWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPickFiveNervesWidget::NativeConstruct()
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

	if (nerveFourButton && nerveFourButton->GetVisibility() != ESlateVisibility::Visible)
	{
		nerveFourButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (nerveFiveButton && nerveFiveButton->GetVisibility() != ESlateVisibility::Visible)
	{
		nerveFiveButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (restartSequenceButton && restartSequenceButton->GetVisibility() != ESlateVisibility::Visible)
	{
		restartSequenceButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (restartSequenceButton)
	{
		restartSequenceButton->OnPressed.AddDynamic(this, &UPickFiveNervesWidget::RestartNerveSequence);
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

void UPickFiveNervesWidget::NativeDestruct()
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

	if (nerveFourButton)
	{
		nerveFourButton->RemoveFromParent();
		nerveFourButton = nullptr;
	}

	if (nerveFiveButton)
	{
		nerveFiveButton->RemoveFromParent();
		nerveFiveButton = nullptr;
	}
}

void UPickFiveNervesWidget::RemoveBindedButtonDelegates()
{
	if (nerveOneButton)
	{
		nerveOneButton->OnPressed.RemoveDynamic(this, &UPickFiveNervesWidget::NerveOneSelected);
	}

	if (nerveTwoButton)
	{
		nerveTwoButton->OnPressed.RemoveDynamic(this, &UPickFiveNervesWidget::NerveTwoSelected);
	}

	if (nerveThreeButton)
	{
		nerveThreeButton->OnPressed.RemoveDynamic(this, &UPickFiveNervesWidget::NerveThreeSelected);
	}

	if (nerveFourButton)
	{
		nerveFourButton->OnPressed.RemoveDynamic(this, &UPickFiveNervesWidget::NerveFourSelected);
	}

	if (nerveFiveButton)
	{
		nerveFiveButton->OnPressed.RemoveDynamic(this, &UPickFiveNervesWidget::NerveFiveSelected);
	}

	if (restartSequenceButton)
	{
		restartSequenceButton->OnPressed.RemoveDynamic(this, &UPickFiveNervesWidget::RestartNerveSequence);
	}
}

void UPickFiveNervesWidget::SetupNerveOneButtonImageDependingOnNerve()
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
			const int& randomizedIndex = FMath::RandRange(0, 4);
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

		nerveOneButton->OnPressed.AddDynamic(this, &UPickFiveNervesWidget::NerveOneSelected);

		SetupNerveTwoButtonImageDependingOnNerve();
	}
}

void UPickFiveNervesWidget::SetupNerveTwoButtonImageDependingOnNerve()
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
			const int& randomizedIndex = FMath::RandRange(0, 4);
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

		nerveTwoButton->OnPressed.AddDynamic(this, &UPickFiveNervesWidget::NerveTwoSelected);

		SetupNerveThreeButtonImageDependingOnNerve();
	}
}

void UPickFiveNervesWidget::SetupNerveThreeButtonImageDependingOnNerve()
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
			const int& randomizedIndex = FMath::RandRange(0, 4);
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

		nerveThreeButton->OnPressed.AddDynamic(this, &UPickFiveNervesWidget::NerveThreeSelected);

		SetupNerveFourButtonImageDependingOnNerve();
	}
}

void UPickFiveNervesWidget::SetupNerveFourButtonImageDependingOnNerve()
{
	if (nerveFourButton)
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
			const int& randomizedIndex = FMath::RandRange(0, 4);
			buttonFourTextureNumber = randomizedIndex;

			// If the 4th button texture has the same index as button one, two or three, re-execute this function
			if (buttonFourTextureNumber == buttonOneTextureNumber || buttonFourTextureNumber == buttonTwoTextureNumber ||
				buttonFourTextureNumber == buttonThreeTextureNumber)
			{
				SetupNerveFourButtonImageDependingOnNerve();
				return;
			}

			switch (gameMode->GetCurrentCompletedSequences())
			{
			case 0: // Blue nerves
				if (gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
				}

				break;

			case 1: // Orange nerves
				if (gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
				}

				break;

			case 2: // Purple nerves
				if (gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
				}

				break;

			case 3: // Yellow nerves
				if (gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFourTextureNumber)));
				}

				break;
			}
		}

		if (sequencePressedSounds[3])
		{
			FSlateSound sound;
			sound.SetResourceObject(sequencePressedSounds[3]);

			buttonStyle.SetPressedSound(sound);
			buttonStyle.SetClickedSound(sound);
		}

		buttonStyle.SetNormal(normalBrush);
		buttonStyle.SetHovered(hoveredBrush);
		buttonStyle.SetPressed(pressedBrush);

		nerveFourButton->SetStyle(buttonStyle);

		nerveFourButton->OnPressed.AddDynamic(this, &UPickFiveNervesWidget::NerveFourSelected);

		SetupNerveFiveButtonImageDependingOnNerve();
	}
}

void UPickFiveNervesWidget::SetupNerveFiveButtonImageDependingOnNerve()
{
	if (nerveFiveButton)
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
			const int& randomizedIndex = FMath::RandRange(0, 4);
			buttonFiveTextureNumber = randomizedIndex;

			// If the 4th button texture has the same index as button one, two or three, re-execute this function
			if (buttonFiveTextureNumber == buttonOneTextureNumber || buttonFiveTextureNumber == buttonTwoTextureNumber ||
				buttonFiveTextureNumber == buttonThreeTextureNumber || buttonFiveTextureNumber == buttonFourTextureNumber)
			{
				SetupNerveFiveButtonImageDependingOnNerve();
				return;
			}

			switch (gameMode->GetCurrentCompletedSequences())
			{
			case 0: // Blue nerves
				if (gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetBlueNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
				}

				break;

			case 1: // Orange nerves
				if (gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
				}

				break;

			case 2: // Purple nerves
				if (gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
				}

				break;

			case 3: // Yellow nerves
				if (gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)))
				{
					normalBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
					hoveredBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
					pressedBrush.SetResourceObject(gameMode->GetYellowNerveTextureByIndex(gameMode->GetReferencedRandomNerveIndex(buttonFiveTextureNumber)));
				}

				break;
			}
		}

		if (sequencePressedSounds[4])
		{
			FSlateSound sound;
			sound.SetResourceObject(sequencePressedSounds[4]);

			buttonStyle.SetPressedSound(sound);
			buttonStyle.SetClickedSound(sound);
		}

		buttonStyle.SetNormal(normalBrush);
		buttonStyle.SetHovered(hoveredBrush);
		buttonStyle.SetPressed(pressedBrush);

		nerveFiveButton->SetStyle(buttonStyle);

		nerveFiveButton->OnPressed.AddDynamic(this, &UPickFiveNervesWidget::NerveFiveSelected);
	}
}

void UPickFiveNervesWidget::RestartNerveSequence()
{
	RemoveBindedButtonDelegates();

	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetPlayerHUDClass());
		gameMode->PopWidget(gameMode->GetPickFiveNervesClass());
		gameMode->ResetCurrentSequenceStats(false);

		gameMode->PushWidget(gameMode->GetNerveSequenceClass());
	}
}

void UPickFiveNervesWidget::NerveOneSelected()
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
			gameMode->PopWidget(gameMode->GetPickFiveNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::ENone);

			gameMode->PushWidget(gameMode->GetSequenceStatsClass());
		}
	}
}

void UPickFiveNervesWidget::NerveTwoSelected()
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
			gameMode->PopWidget(gameMode->GetPickFiveNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::ENone);

			gameMode->PushWidget(gameMode->GetSequenceStatsClass());
		}
	}
}

void UPickFiveNervesWidget::NerveThreeSelected()
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
			gameMode->PopWidget(gameMode->GetPickFiveNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::ENone);

			gameMode->PushWidget(gameMode->GetSequenceStatsClass());
		}
	}
}

void UPickFiveNervesWidget::NerveFourSelected()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (gameMode->GetCurrentPickedSequences() == gameMode->FindRandomNerveAtReferencedIndex(buttonFourTextureNumber))
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
			gameMode->PopWidget(gameMode->GetPickFiveNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::ENone);

			gameMode->PushWidget(gameMode->GetSequenceStatsClass());
		}
	}
}

void UPickFiveNervesWidget::NerveFiveSelected()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (gameMode->GetCurrentPickedSequences() == gameMode->FindRandomNerveAtReferencedIndex(buttonFiveTextureNumber))
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
			gameMode->PopWidget(gameMode->GetPickFiveNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::ENone);

			gameMode->PushWidget(gameMode->GetSequenceStatsClass());
		}
	}
}