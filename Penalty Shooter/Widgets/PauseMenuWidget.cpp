// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/PauseMenuWidget.h"
#include "Player/PlayerCharacter.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LeftResumeImage) LeftResumeImage->SetVisibility(ESlateVisibility::Visible);
	if (RightResumeImage) RightResumeImage->SetVisibility(ESlateVisibility::Visible);
	if (ResumeText) ResumeText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));

	if (LeftQuitImage) LeftQuitImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightQuitImage) RightQuitImage->SetVisibility(ESlateVisibility::Hidden);
	if (QuitText) QuitText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	if (ResumeButton)
	{
		ResumeButton->OnHovered.AddUniqueDynamic(this, &UPauseMenuWidget::HoverOnResumeButton);
		ResumeButton->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.AddUniqueDynamic(this, &UPauseMenuWidget::HoverOnQuitButton);
		QuitButton->OnClicked.AddUniqueDynamic(this, &UPauseMenuWidget::QuitGame);
	}

	if (!UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UPauseMenuWidget::ResumeGame()
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
		playerCharacter->ShowPlayerHUD();
		playerCharacter = nullptr;
	}

	if (ResumeButton)
	{
		ResumeButton->OnHovered.RemoveDynamic(this, &UPauseMenuWidget::HoverOnResumeButton);
		ResumeButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.RemoveDynamic(this, &UPauseMenuWidget::HoverOnQuitButton);
		QuitButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::QuitGame);
	}
}

void UPauseMenuWidget::QuitGame()
{
	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;

	if (playerCharacter.IsValid())
	{
		RemoveFromParent();

		playerCharacter->DeleteCurrentPlayerInformation();
		playerCharacter->ResetCountingEXPStats();
		playerCharacter->InvalidateSoccerBall();
		playerCharacter->ShowMainMenu(false);
		playerCharacter = nullptr;
	}

	if (ResumeButton)
	{
		ResumeButton->OnHovered.RemoveDynamic(this, &UPauseMenuWidget::HoverOnResumeButton);
		ResumeButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::ResumeGame);
	}

	if (QuitButton)
	{
		QuitButton->OnHovered.RemoveDynamic(this, &UPauseMenuWidget::HoverOnQuitButton);
		QuitButton->OnClicked.RemoveDynamic(this, &UPauseMenuWidget::QuitGame);
	}
}

void UPauseMenuWidget::HoverOnResumeButton()
{
	if (LeftResumeImage && !LeftResumeImage->IsVisible()) LeftResumeImage->SetVisibility(ESlateVisibility::Visible);
	if (RightResumeImage && !RightResumeImage->IsVisible()) RightResumeImage->SetVisibility(ESlateVisibility::Visible);
	if (ResumeText) ResumeText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));

	if (LeftQuitImage && LeftQuitImage->IsVisible()) LeftQuitImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightQuitImage && RightQuitImage->IsVisible()) RightQuitImage->SetVisibility(ESlateVisibility::Hidden);
	if (QuitText) QuitText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

void UPauseMenuWidget::HoverOnQuitButton()
{
	if (LeftResumeImage && LeftResumeImage->IsVisible()) LeftResumeImage->SetVisibility(ESlateVisibility::Hidden);
	if (RightResumeImage && RightResumeImage->IsVisible()) RightResumeImage->SetVisibility(ESlateVisibility::Hidden);
	if (ResumeText) ResumeText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	if (LeftQuitImage && !LeftQuitImage->IsVisible()) LeftQuitImage->SetVisibility(ESlateVisibility::Visible);
	if (RightQuitImage && !RightQuitImage->IsVisible()) RightQuitImage->SetVisibility(ESlateVisibility::Visible);
	if (QuitText) QuitText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
}