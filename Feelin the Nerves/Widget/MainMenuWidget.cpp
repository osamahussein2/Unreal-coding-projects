// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/GoedWareCommunityGM.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (MainMenuCanvas && MainMenuCanvas->GetVisibility() != ESlateVisibility::Visible)
	{
		MainMenuCanvas->SetVisibility(ESlateVisibility::Visible);
	}

	if (MainMenuTitleText && MainMenuTitleText->GetVisibility() != ESlateVisibility::Visible)
	{
		MainMenuTitleText->SetVisibility(ESlateVisibility::Visible);
	}

	if (CreditsButton && CreditsButton->GetVisibility() != ESlateVisibility::Visible)
	{
		CreditsButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayButton && PlayButton->GetVisibility() != ESlateVisibility::Visible)
	{
		PlayButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (LoadButton && LoadButton->GetVisibility() != ESlateVisibility::Visible)
	{
		LoadButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (QuitButton && QuitButton->GetVisibility() != ESlateVisibility::Visible)
	{
		QuitButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PlayGame);
		PlayButton->OnPressed.AddDynamic(this, &UMainMenuWidget::PlayGame);
	}

	if (LoadButton)
	{
		LoadButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToLoadGame);
		LoadButton->OnPressed.AddDynamic(this, &UMainMenuWidget::GoToLoadGame);
	}

	if (CreditsButton)
	{
		CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
		CreditsButton->OnPressed.AddDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGame);
		QuitButton->OnPressed.AddDynamic(this, &UMainMenuWidget::QuitGame);
	}
}

/*void UMainMenuWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}*/

void UMainMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (PlayButton)
	{
		PlayButton->RemoveFromParent();
		PlayButton = nullptr;
	}

	if (LoadButton)
	{
		LoadButton->RemoveFromParent();
		LoadButton = nullptr;
	}

	if (CreditsButton)
	{
		CreditsButton->RemoveFromParent();
		CreditsButton = nullptr;
	}

	if (QuitButton)
	{
		QuitButton->RemoveFromParent();
		QuitButton = nullptr;
	}

	if (MainMenuTitleText)
	{
		MainMenuTitleText->RemoveFromParent();
		MainMenuTitleText = nullptr;
	}

	if (MainMenuCanvas)
	{
		MainMenuCanvas->RemoveFromParent();
		MainMenuCanvas = nullptr;
	}
}

void UMainMenuWidget::PlayGame()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->SpawnBodyActor();

		gameMode->AddRandomNerves();
		
		gameMode->PopWidget(gameMode->GetMainMenuClass());
		gameMode->PushWidget(gameMode->GetNerveSequenceClass());
	}

	RemoveBindedButtonDelegates();
}

void UMainMenuWidget::GoToLoadGame()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetMainMenuClass());
		gameMode->PushWidget(gameMode->GetMM_LoadGameClass());
	}

	RemoveBindedButtonDelegates();
}

void UMainMenuWidget::GoToCreditsMenu()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetMainMenuClass());
		gameMode->PushWidget(gameMode->GetCreditsClass());
	}

	RemoveBindedButtonDelegates();
}

void UMainMenuWidget::QuitGame()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetMainMenuClass());
		gameMode->PushWidget(gameMode->GetMM_QuitConfirmationClass());
	}

	RemoveBindedButtonDelegates();
}

void UMainMenuWidget::RemoveBindedButtonDelegates()
{
	if (PlayButton)
	{
		PlayButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::PlayGame);
		PlayButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::PlayGame);
	}

	if (LoadButton)
	{
		LoadButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToLoadGame);
		LoadButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::GoToLoadGame);
	}

	if (CreditsButton)
	{
		CreditsButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
		CreditsButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::GoToCreditsMenu);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
		QuitButton->OnPressed.RemoveDynamic(this, &UMainMenuWidget::QuitGame);
	}
}