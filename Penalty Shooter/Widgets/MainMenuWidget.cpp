// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MainMenuWidget.h"
#include "Player/PlayerCharacter.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LeftPlayImage) LeftPlayImage->SetVisibility(ESlateVisibility::Visible);
	if (RightPlayImage) RightPlayImage->SetVisibility(ESlateVisibility::Visible);
	if (PlayText) PlayText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));

	if (LeftOptionsImage) LeftOptionsImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightOptionsImage) RightOptionsImage->SetVisibility(ESlateVisibility::Hidden);
	if (OptionsText) OptionsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	if (LeftCreditsImage) LeftCreditsImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightCreditsImage) RightCreditsImage->SetVisibility(ESlateVisibility::Hidden);
	if (CreditsText) CreditsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	if (LeftLeaderboardImage) LeftLeaderboardImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightLeaderboardImage) RightLeaderboardImage->SetVisibility(ESlateVisibility::Hidden);
	if (LeaderboardText) LeaderboardText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	if (LeftQuitImage) LeftQuitImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightQuitImage) RightQuitImage->SetVisibility(ESlateVisibility::Hidden);
	if (QuitText) QuitText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	if (PlayButton)
	{
		PlayButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::HoverOnPlayButton);
		PlayButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::PlayLevel);
	}

	if (OptionsButton)
	{
		OptionsButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::HoverOnOptionsButton);
		OptionsButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::GoToOptionsMenu);
	}

	if (CreditsButton)
	{
		CreditsButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::HoverOnCreditsButton);
		CreditsButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
	}

	if (LeaderboardButton)
	{
		LeaderboardButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::HoverOnLeaderboardButton);
		LeaderboardButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::GoToLeaderboard);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.AddUniqueDynamic(this, &UMainMenuWidget::HoverOnQuitButton);
		QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::QuitGame);
	}

	if (!UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UMainMenuWidget::PlayLevel()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		RemoveFromParent();

		UGameplayStatics::SetGamePaused(GetWorld(), false);

		playerCharacter->DisableMouseCursor();
		playerCharacter->FindSoccerBall();
		playerCharacter->ResetGame();
		playerCharacter->AddPlayerInformation();
		playerCharacter->ShowPlayerHUD();
		playerCharacter = nullptr;
	}

	if (PlayButton)
	{
		PlayButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnPlayButton);
		PlayButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::PlayLevel);
	}

	if (OptionsButton)
	{
		OptionsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnOptionsButton);
		OptionsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToOptionsMenu);
	}

	if (CreditsButton)
	{
		CreditsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnCreditsButton);
		CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
	}

	if (LeaderboardButton)
	{
		LeaderboardButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnLeaderboardButton);
		LeaderboardButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToLeaderboard);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnQuitButton);
		QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::GoToOptionsMenu()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		RemoveFromParent();

		playerCharacter->ShowOptionsMenu();
		playerCharacter = nullptr;
	}

	if (PlayButton)
	{
		PlayButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnPlayButton);
		PlayButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::PlayLevel);
	}

	if (OptionsButton)
	{
		OptionsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnOptionsButton);
		OptionsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToOptionsMenu);
	}

	if (CreditsButton)
	{
		CreditsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnCreditsButton);
		CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
	}

	if (LeaderboardButton)
	{
		LeaderboardButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnLeaderboardButton);
		LeaderboardButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToLeaderboard);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnQuitButton);
		QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::GoToCreditsMenu()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		RemoveFromParent();

		playerCharacter->ShowCreditsMenu();
		playerCharacter = nullptr;
	}

	if (PlayButton)
	{
		PlayButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnPlayButton);
		PlayButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::PlayLevel);
	}

	if (OptionsButton)
	{
		OptionsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnOptionsButton);
		OptionsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToOptionsMenu);
	}

	if (CreditsButton)
	{
		CreditsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnCreditsButton);
		CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
	}

	if (LeaderboardButton)
	{
		LeaderboardButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnLeaderboardButton);
		LeaderboardButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToLeaderboard);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnQuitButton);
		QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::GoToLeaderboard()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		RemoveFromParent();
		
		playerCharacter->ShowLeaderboardMenu();
		playerCharacter = nullptr;
	}

	if (PlayButton)
	{
		PlayButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnPlayButton);
		PlayButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::PlayLevel);
	}

	if (OptionsButton)
	{
		OptionsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnOptionsButton);
		OptionsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToOptionsMenu);
	}

	if (CreditsButton)
	{
		CreditsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnCreditsButton);
		CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
	}

	if (LeaderboardButton)
	{
		LeaderboardButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnLeaderboardButton);
		LeaderboardButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToLeaderboard);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnQuitButton);
		QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::QuitGame()
{
	// Quit the game
	UKismetSystemLibrary::QuitGame(nullptr, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);

	if (PlayButton)
	{
		PlayButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnPlayButton);
		PlayButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::PlayLevel);
	}

	if (OptionsButton)
	{
		OptionsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnOptionsButton);
		OptionsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToOptionsMenu);
	}

	if (CreditsButton)
	{
		CreditsButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnCreditsButton);
		CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
	}

	if (LeaderboardButton)
	{
		LeaderboardButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnLeaderboardButton);
		LeaderboardButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToLeaderboard);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.RemoveDynamic(this, &UMainMenuWidget::HoverOnQuitButton);
		QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

void UMainMenuWidget::HoverOnPlayButton()
{
	if (LeftPlayImage && !LeftPlayImage->IsVisible()) LeftPlayImage->SetVisibility(ESlateVisibility::Visible);
	if (RightPlayImage && !RightPlayImage->IsVisible()) RightPlayImage->SetVisibility(ESlateVisibility::Visible);

	if (PlayText && PlayText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		PlayText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}

	if (LeftOptionsImage && LeftOptionsImage->IsVisible()) LeftOptionsImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightOptionsImage && RightOptionsImage->IsVisible()) RightOptionsImage->SetVisibility(ESlateVisibility::Hidden);

	if (OptionsText && OptionsText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		OptionsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftCreditsImage && LeftCreditsImage->IsVisible()) LeftCreditsImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightCreditsImage && RightCreditsImage->IsVisible()) RightCreditsImage->SetVisibility(ESlateVisibility::Hidden);

	if (CreditsText && CreditsText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		CreditsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftLeaderboardImage && LeftLeaderboardImage->IsVisible()) LeftLeaderboardImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightLeaderboardImage && RightLeaderboardImage->IsVisible()) RightLeaderboardImage->SetVisibility(ESlateVisibility::Hidden);

	if (LeaderboardText && LeaderboardText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		LeaderboardText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftQuitImage && LeftQuitImage->IsVisible()) LeftQuitImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightQuitImage && RightQuitImage->IsVisible()) RightQuitImage->SetVisibility(ESlateVisibility::Hidden);

	if (QuitText && QuitText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		QuitText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UMainMenuWidget::HoverOnOptionsButton()
{
	if (LeftPlayImage && LeftPlayImage->IsVisible()) LeftPlayImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightPlayImage && RightPlayImage->IsVisible()) RightPlayImage->SetVisibility(ESlateVisibility::Hidden);

	if (PlayText && PlayText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		PlayText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftOptionsImage && !LeftOptionsImage->IsVisible()) LeftOptionsImage->SetVisibility(ESlateVisibility::Visible);
	if (RightOptionsImage && !RightOptionsImage->IsVisible()) RightOptionsImage->SetVisibility(ESlateVisibility::Visible);

	if (OptionsText && OptionsText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		OptionsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}

	if (LeftCreditsImage && LeftCreditsImage->IsVisible()) LeftCreditsImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightCreditsImage && RightCreditsImage->IsVisible()) RightCreditsImage->SetVisibility(ESlateVisibility::Hidden);

	if (CreditsText && CreditsText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		CreditsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftLeaderboardImage && LeftLeaderboardImage->IsVisible()) LeftLeaderboardImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightLeaderboardImage && RightLeaderboardImage->IsVisible()) RightLeaderboardImage->SetVisibility(ESlateVisibility::Hidden);

	if (LeaderboardText && LeaderboardText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		LeaderboardText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftQuitImage && LeftQuitImage->IsVisible()) LeftQuitImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightQuitImage && RightQuitImage->IsVisible()) RightQuitImage->SetVisibility(ESlateVisibility::Hidden);

	if (QuitText && QuitText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		QuitText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UMainMenuWidget::HoverOnCreditsButton()
{
	if (LeftPlayImage && LeftPlayImage->IsVisible()) LeftPlayImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightPlayImage && RightPlayImage->IsVisible()) RightPlayImage->SetVisibility(ESlateVisibility::Hidden);

	if (PlayText && PlayText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		PlayText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftOptionsImage && LeftOptionsImage->IsVisible()) LeftOptionsImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightOptionsImage && RightOptionsImage->IsVisible()) RightOptionsImage->SetVisibility(ESlateVisibility::Hidden);

	if (OptionsText && OptionsText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		OptionsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftCreditsImage && !LeftCreditsImage->IsVisible()) LeftCreditsImage->SetVisibility(ESlateVisibility::Visible);
	if (RightCreditsImage && !RightCreditsImage->IsVisible()) RightCreditsImage->SetVisibility(ESlateVisibility::Visible);

	if (CreditsText && CreditsText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		CreditsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}

	if (LeftLeaderboardImage && LeftLeaderboardImage->IsVisible()) LeftLeaderboardImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightLeaderboardImage && RightLeaderboardImage->IsVisible()) RightLeaderboardImage->SetVisibility(ESlateVisibility::Hidden);

	if (LeaderboardText && LeaderboardText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		LeaderboardText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftQuitImage && LeftQuitImage->IsVisible()) LeftQuitImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightQuitImage && RightQuitImage->IsVisible()) RightQuitImage->SetVisibility(ESlateVisibility::Hidden);

	if (QuitText && QuitText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		QuitText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UMainMenuWidget::HoverOnLeaderboardButton()
{
	if (LeftPlayImage && LeftPlayImage->IsVisible()) LeftPlayImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightPlayImage && RightPlayImage->IsVisible()) RightPlayImage->SetVisibility(ESlateVisibility::Hidden);
	
	if (PlayText && PlayText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		PlayText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftOptionsImage && LeftOptionsImage->IsVisible()) LeftOptionsImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightOptionsImage && RightOptionsImage->IsVisible()) RightOptionsImage->SetVisibility(ESlateVisibility::Hidden);

	if (OptionsText && OptionsText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		OptionsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftCreditsImage && LeftCreditsImage->IsVisible()) LeftCreditsImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightCreditsImage && RightCreditsImage->IsVisible()) RightCreditsImage->SetVisibility(ESlateVisibility::Hidden);

	if (CreditsText && CreditsText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		CreditsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftLeaderboardImage && !LeftLeaderboardImage->IsVisible()) LeftLeaderboardImage->SetVisibility(ESlateVisibility::Visible);
	if (RightLeaderboardImage && !RightLeaderboardImage->IsVisible()) RightLeaderboardImage->SetVisibility(ESlateVisibility::Visible);

	if (LeaderboardText && LeaderboardText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		LeaderboardText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}

	if (LeftQuitImage && LeftQuitImage->IsVisible()) LeftQuitImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightQuitImage && RightQuitImage->IsVisible()) RightQuitImage->SetVisibility(ESlateVisibility::Hidden);
	
	if (QuitText && LeaderboardText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		QuitText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UMainMenuWidget::HoverOnQuitButton()
{
	if (LeftPlayImage && LeftPlayImage->IsVisible()) LeftPlayImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightPlayImage && RightPlayImage->IsVisible()) RightPlayImage->SetVisibility(ESlateVisibility::Hidden);

	if (PlayText && PlayText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		PlayText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftOptionsImage && LeftOptionsImage->IsVisible()) LeftOptionsImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightOptionsImage && RightOptionsImage->IsVisible()) RightOptionsImage->SetVisibility(ESlateVisibility::Hidden);

	if (OptionsText && OptionsText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		OptionsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftCreditsImage && LeftCreditsImage->IsVisible()) LeftCreditsImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightCreditsImage && RightCreditsImage->IsVisible()) RightCreditsImage->SetVisibility(ESlateVisibility::Hidden);

	if (CreditsText && CreditsText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		CreditsText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftLeaderboardImage && LeftLeaderboardImage->IsVisible()) LeftLeaderboardImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightLeaderboardImage && RightLeaderboardImage->IsVisible()) RightLeaderboardImage->SetVisibility(ESlateVisibility::Hidden);

	if (LeaderboardText && LeaderboardText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
	{
		LeaderboardText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (LeftQuitImage && !LeftQuitImage->IsVisible()) LeftQuitImage->SetVisibility(ESlateVisibility::Visible);
	if (RightQuitImage && !RightQuitImage->IsVisible()) RightQuitImage->SetVisibility(ESlateVisibility::Visible);
	
	if (QuitText && QuitText->GetColorAndOpacity() != FLinearColor(1.0f, 1.0f, 0.0f, 1.0f))
	{
		QuitText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
	}
}
