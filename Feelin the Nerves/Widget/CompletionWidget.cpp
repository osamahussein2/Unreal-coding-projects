// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/CompletionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/GoedWareCommunityGM.h"

UCompletionWidget::UCompletionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCompletionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CompletionCanvas && !CompletionCanvas->IsVisible()) CompletionCanvas->SetVisibility(ESlateVisibility::Visible);
	if (CompletionText && !CompletionText->IsVisible()) CompletionText->SetVisibility(ESlateVisibility::Visible);
	if (RestartButton && !RestartButton) RestartButton->SetVisibility(ESlateVisibility::Visible);
	if (QuitButton && !QuitButton) QuitButton->SetVisibility(ESlateVisibility::Visible);

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UCompletionWidget::RestartGame);
		RestartButton->OnPressed.AddDynamic(this, &UCompletionWidget::RestartGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UCompletionWidget::GoToMainMenu);
		QuitButton->OnPressed.AddDynamic(this, &UCompletionWidget::GoToMainMenu);
	}
}

/*void UCompletionWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}*/

void UCompletionWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (RestartButton)
	{
		RestartButton->RemoveFromParent();
		RestartButton = nullptr;
	}

	if (QuitButton)
	{
		QuitButton->RemoveFromParent();
		QuitButton = nullptr;
	}

	if (CompletionText)
	{
		CompletionText->RemoveFromParent();
		CompletionText = nullptr;
	}

	if (CompletionCanvas)
	{
		CompletionCanvas->RemoveFromParent();
		CompletionCanvas = nullptr;
	}
}

void UCompletionWidget::RestartGame()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetCompletionClass());

		gameMode->ResetGame();
		
		gameMode->SpawnBodyActor();
		gameMode->AddRandomNerves();

		gameMode->PushWidget(gameMode->GetNerveSequenceClass());
	}

	RemoveBindedButtonDelegates();
}

void UCompletionWidget::GoToMainMenu()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetCompletionClass());
		gameMode->PushWidget(gameMode->GetCM_QuitConfirmationClass());
	}

	RemoveBindedButtonDelegates();
}

void UCompletionWidget::RemoveBindedButtonDelegates()
{
	if (RestartButton)
	{
		RestartButton->OnClicked.RemoveDynamic(this, &UCompletionWidget::RestartGame);
		RestartButton->OnPressed.RemoveDynamic(this, &UCompletionWidget::RestartGame);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.RemoveDynamic(this, &UCompletionWidget::GoToMainMenu);
		QuitButton->OnPressed.RemoveDynamic(this, &UCompletionWidget::GoToMainMenu);
	}
}