// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainOptionsMenuWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Player/PlayerCharacter.h"
#include "GameMode/BrackeysGM.h"
#include "Kismet/GameplayStatics.h"

UMainOptionsMenuWidget::UMainOptionsMenuWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainOptionsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OptionsMenuCanvas && OptionsMenuCanvas->GetVisibility() != ESlateVisibility::Visible)
	{
		OptionsMenuCanvas->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton && BackButton->GetVisibility() != ESlateVisibility::Visible)
	{
		BackButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (ApplyButton && ApplyButton->GetVisibility() != ESlateVisibility::Visible)
	{
		ApplyButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (MusicVolumeSlider && MusicVolumeSlider->GetVisibility() != ESlateVisibility::Visible)
	{
		MusicVolumeSlider->SetVisibility(ESlateVisibility::Visible);
	}

	if (MusicVolumePercentageText && MusicVolumePercentageText->GetVisibility() != ESlateVisibility::Visible)
	{
		MusicVolumePercentageText->SetVisibility(ESlateVisibility::Visible);
	}

	if (SFXVolumeSlider && SFXVolumeSlider->GetVisibility() != ESlateVisibility::Visible)
	{
		SFXVolumeSlider->SetVisibility(ESlateVisibility::Visible);
	}

	if (SFXVolumePercentageText && SFXVolumePercentageText->GetVisibility() != ESlateVisibility::Visible)
	{
		SFXVolumePercentageText->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &UMainOptionsMenuWidget::GoBackToMainMenu);
		BackButton->OnPressed.AddUniqueDynamic(this, &UMainOptionsMenuWidget::GoBackToMainMenu);
	}

	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddUniqueDynamic(this, &UMainOptionsMenuWidget::ApplyOptions);
		ApplyButton->OnPressed.AddUniqueDynamic(this, &UMainOptionsMenuWidget::ApplyOptions);
	}

	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// Set the music volume slider value to the current music volume from the game mode class
		if (MusicVolumeSlider) MusicVolumeSlider->SetValue(gameMode->GetMusicVolume());

		// Return the music volume percentage
		if (MusicVolumePercentageText)
		{
			MusicVolumePercentageText->SetText(FText::FromString(FString::FromInt(static_cast<int>(
				gameMode->GetMusicVolume() * 100.0f)) + TEXT("%")));
		}

		lastMusicVolume = gameMode->GetMusicVolume();

		// Set the music volume slider value to the current music volume from the game mode class
		if (SFXVolumeSlider) SFXVolumeSlider->SetValue(gameMode->GetSFXVolume());

		// Return the music volume percentage
		if (SFXVolumePercentageText)
		{
			SFXVolumePercentageText->SetText(FText::FromString(FString::FromInt(static_cast<int>(
				gameMode->GetSFXVolume() * 100.0f)) + TEXT("%")));
		}

		lastSFXVolume = gameMode->GetSFXVolume();
	}

	// Bind music volume slider OnValueChanged delegate to the update music volume function
	if (MusicVolumeSlider)
	{
		MusicVolumeSlider->OnValueChanged.AddUniqueDynamic(this, &UMainOptionsMenuWidget::UpdateMusicVolumeSlider);
	}

	// Bind music volume slider OnValueChanged delegate to the update music volume function
	if (SFXVolumeSlider)
	{
		SFXVolumeSlider->OnValueChanged.AddUniqueDynamic(this, &UMainOptionsMenuWidget::UpdateSFXVolumeSlider);
	}
}

void UMainOptionsMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (OptionsMenuCanvas)
	{
		OptionsMenuCanvas->RemoveFromParent();
		OptionsMenuCanvas = nullptr;
	}

	if (ApplyButton)
	{
		ApplyButton->RemoveFromParent();
		ApplyButton = nullptr;
	}

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
	}

	if (MusicVolumeSlider)
	{
		MusicVolumeSlider->RemoveFromParent();
		MusicVolumeSlider = nullptr;
	}

	if (MusicVolumePercentageText)
	{
		MusicVolumePercentageText->RemoveFromParent();
		MusicVolumePercentageText = nullptr;
	}

	if (SFXVolumeSlider)
	{
		SFXVolumeSlider->RemoveFromParent();
		SFXVolumeSlider = nullptr;
	}

	if (SFXVolumePercentageText)
	{
		SFXVolumePercentageText->RemoveFromParent();
		SFXVolumePercentageText = nullptr;
	}
}

void UMainOptionsMenuWidget::GoBackToMainMenu()
{
	if (BackButton)
	{
		BackButton->OnClicked.RemoveDynamic(this, &UMainOptionsMenuWidget::GoBackToMainMenu);
		BackButton->OnPressed.RemoveDynamic(this, &UMainOptionsMenuWidget::GoBackToMainMenu);
	}

	if (ApplyButton)
	{
		ApplyButton->OnClicked.RemoveDynamic(this, &UMainOptionsMenuWidget::ApplyOptions);
		ApplyButton->OnPressed.RemoveDynamic(this, &UMainOptionsMenuWidget::ApplyOptions);
	}

	// Remove the update music volume slider binding from the OnValueChanged signature once going back to the main menu
	if (MusicVolumeSlider) MusicVolumeSlider->OnValueChanged.RemoveDynamic(this, &UMainOptionsMenuWidget::UpdateMusicVolumeSlider);

	// Remove the update SFX volume slider binding from the OnValueChanged signature once going back to the main menu
	if (SFXVolumeSlider) SFXVolumeSlider->OnValueChanged.RemoveDynamic(this, &UMainOptionsMenuWidget::UpdateSFXVolumeSlider);

	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->SetMusicVolume(lastMusicVolume);
		gameMode->SetSFXVolume(lastSFXVolume);

		if (menuCancel) UGameplayStatics::PlaySound2D(GetWorld(), menuCancel, gameMode->GetSFXVolume());

		gameMode->PopWidget(gameMode->GetMainOptionsMenuClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());
	}
}

void UMainOptionsMenuWidget::ApplyOptions()
{
	if (BackButton)
	{
		BackButton->OnClicked.RemoveDynamic(this, &UMainOptionsMenuWidget::GoBackToMainMenu);
		BackButton->OnPressed.RemoveDynamic(this, &UMainOptionsMenuWidget::GoBackToMainMenu);
	}

	if (ApplyButton)
	{
		ApplyButton->OnClicked.RemoveDynamic(this, &UMainOptionsMenuWidget::ApplyOptions);
		ApplyButton->OnPressed.RemoveDynamic(this, &UMainOptionsMenuWidget::ApplyOptions);
	}

	// Remove the update music volume slider binding from the OnValueChanged signature once going back to the main menu
	if (MusicVolumeSlider) MusicVolumeSlider->OnValueChanged.RemoveDynamic(this, &UMainOptionsMenuWidget::UpdateMusicVolumeSlider);

	// Remove the update SFX volume slider binding from the OnValueChanged signature once going back to the main menu
	if (SFXVolumeSlider) SFXVolumeSlider->OnValueChanged.RemoveDynamic(this, &UMainOptionsMenuWidget::UpdateSFXVolumeSlider);

	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (menuConfirm) UGameplayStatics::PlaySound2D(GetWorld(), menuConfirm, gameMode->GetSFXVolume());

		gameMode->PopWidget(gameMode->GetMainOptionsMenuClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());
	}
}

void UMainOptionsMenuWidget::UpdateMusicVolumeSlider(float musicVolumeValue_)
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// Update music volume
		gameMode->SetMusicVolume(musicVolumeValue_);

		if (menuOptionChange) UGameplayStatics::PlaySound2D(GetWorld(), menuOptionChange, gameMode->GetSFXVolume());

		if (MusicVolumePercentageText)
		{
			// Return the updated music volume percentage
			MusicVolumePercentageText->SetText(FText::FromString(FString::FromInt(static_cast<int>(musicVolumeValue_ * 100.0f))
				+ TEXT("%")));
		}
	}
}

void UMainOptionsMenuWidget::UpdateSFXVolumeSlider(float sfxVolumeValue_)
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// Update SFX volume
		gameMode->SetSFXVolume(sfxVolumeValue_);

		if (menuOptionChange) UGameplayStatics::PlaySound2D(GetWorld(), menuOptionChange, gameMode->GetSFXVolume());

		if (SFXVolumePercentageText)
		{
			// Return the updated SFX volume percentage
			SFXVolumePercentageText->SetText(FText::FromString(FString::FromInt(static_cast<int>(sfxVolumeValue_ * 100.0f))
				+ TEXT("%")));
		}
	}
}
