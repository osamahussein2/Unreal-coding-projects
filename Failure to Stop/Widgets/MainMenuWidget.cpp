// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MainMenuWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/GoedWareCutsceneGM.h"
#include "Player/CutscenePlayer.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayButton)
	{
		FButtonStyle playButtonStyle = PlayButton->GetStyle();
		FSlateBrush brushNormal;

		brushNormal.TintColor = FSlateColor(FLinearColor(0.445f, 0.41f, 0.234f, 1.0f));
		brushNormal.DrawAs = ESlateBrushDrawType::Box;

		playButtonStyle.SetNormal(brushNormal);
		playButtonStyle.SetNormalPadding(FMargin(12.0f, 3.0f, 12.0f, 3.0f));

		PlayButton->SetStyle(playButtonStyle);

		PlayButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
		PlayButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::PlayGame);
		PlayButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::PlayGame);
	}

	if (CreditsButton)
	{
		CreditsButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
		CreditsButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::GoToCredits);
		CreditsButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::GoToCredits);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
		QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::QuitGame);
		QuitButton->OnPressed.AddUniqueDynamic(this, &UMainMenuWidget::QuitGame);
	}

	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACutscenePlayer::StaticClass(), foundPlayer);

	cutscenePlayer = Cast<ACutscenePlayer>(foundPlayer[0]);
	foundPlayer[0] = nullptr;
}

void UMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (cutscenePlayer.IsValid())
	{
		if (!cutscenePlayer->ReachedMainMenu())
		{
			Super::NativeTick(MyGeometry, InDeltaTime);

			// Set camera location and rotation to the set camera credits location and rotation
			cutscenePlayer->MoveToMainMenuCamera(InDeltaTime);

			if (MainMenuCanvas->IsVisible()) MainMenuCanvas->SetVisibility(ESlateVisibility::Hidden);
		}

		else
		{
			// If the main menu canvas isn't visible, make it visible
			if (!MainMenuCanvas->IsVisible())
			{
				Super::NativeTick(MyGeometry, InDeltaTime);

				// Run reset cutscene camera transform only once with modified focus value
				cutscenePlayer->ResetCutsceneCameraTransform(true);
				MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UMainMenuWidget::PlayGame()
{
	if (AGoedWareCutsceneGM* cutsceneGM = Cast<AGoedWareCutsceneGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		cutsceneGM->HideMainMenu();
		cutsceneGM->ShowLoadingWidget(LoadToNextState::Play);

		if (PlayButton->OnHovered.IsBound())
		{
			PlayButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
		}

		if (CreditsButton->OnHovered.IsBound())
		{
			CreditsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
		}

		if (QuitButton->OnHovered.IsBound())
		{
			QuitButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
		}

		if (cutscenePlayer.IsValid()) cutscenePlayer = nullptr;

		PlayButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::PlayGame);
		PlayButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::PlayGame);

		CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToCredits);
		CreditsButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::GoToCredits);
		
		QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
		QuitButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::MakePlayNormalButtonInvisible()
{
	FButtonStyle playButtonStyle;
	FSlateBrush brushNormal, brushHovered;

	brushNormal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	brushNormal.DrawAs = ESlateBrushDrawType::Box;

	brushHovered.TintColor = FSlateColor(FLinearColor(0.445f, 0.41f, 0.234f, 1.0f));
	brushHovered.DrawAs = ESlateBrushDrawType::Box;

	playButtonStyle.SetNormal(brushNormal);
	playButtonStyle.SetNormalPadding(FMargin(12.0f, 3.0f, 12.0f, 3.0f));

	playButtonStyle.SetHovered(brushHovered);

	PlayButton->SetStyle(playButtonStyle);

	/*
		Remove all OnHovered bindings
	*/

	if (PlayButton->OnHovered.IsBound())
	{
		PlayButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
	}

	if (CreditsButton->OnHovered.IsBound())
	{
		CreditsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
	}

	if (QuitButton->OnHovered.IsBound())
	{
		QuitButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
	}
}

void UMainMenuWidget::GoToCredits()
{
	if (AGoedWareCutsceneGM* cutsceneGM = Cast<AGoedWareCutsceneGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		cutsceneGM->HideMainMenu();
		cutsceneGM->ShowGoBackWidget(); // Go back widget is for only showing the back button after looking at the credits billboard

		if (PlayButton->OnHovered.IsBound())
		{
			PlayButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
		}

		if (CreditsButton->OnHovered.IsBound())
		{
			CreditsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
		}

		if (QuitButton->OnHovered.IsBound())
		{
			QuitButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
		}

		if (cutscenePlayer.IsValid()) cutscenePlayer = nullptr;

		PlayButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::PlayGame);
		PlayButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::PlayGame);

		CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToCredits);
		CreditsButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::GoToCredits);

		QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
		QuitButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::QuitGame()
{
	// Quit the game
	UKismetSystemLibrary::QuitGame(nullptr, GetWorld()->GetFirstPlayerController(),
		EQuitPreference::Quit, true);

	if (PlayButton->OnHovered.IsBound())
	{
		PlayButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
	}

	if (CreditsButton->OnHovered.IsBound())
	{
		CreditsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
	}

	if (QuitButton->OnHovered.IsBound())
	{
		QuitButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::MakePlayNormalButtonInvisible);
	}

	if (cutscenePlayer.IsValid()) cutscenePlayer = nullptr;

	PlayButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::PlayGame);
	PlayButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::PlayGame);

	CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToCredits);
	CreditsButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::GoToCredits);

	QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	QuitButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
}