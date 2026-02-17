// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/CM_QuitConfirmationWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/GoedWareCommunityGM.h"

UCM_QuitConfirmationWidget::UCM_QuitConfirmationWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCM_QuitConfirmationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CM_QuitConfirmationCanvas && !CM_QuitConfirmationCanvas->IsVisible()) CM_QuitConfirmationCanvas->SetVisibility(ESlateVisibility::Visible);
	if (CM_QuitConfirmationText && !CM_QuitConfirmationText->IsVisible()) CM_QuitConfirmationText->SetVisibility(ESlateVisibility::Visible);
	if (YesButton && !YesButton) YesButton->SetVisibility(ESlateVisibility::Visible);
	if (NoButton && !NoButton) NoButton->SetVisibility(ESlateVisibility::Visible);

	if (YesButton)
	{
		YesButton->OnClicked.AddDynamic(this, &UCM_QuitConfirmationWidget::GoBackToMainMenu);
		YesButton->OnPressed.AddDynamic(this, &UCM_QuitConfirmationWidget::GoBackToMainMenu);
	}

	if (NoButton)
	{
		NoButton->OnClicked.AddDynamic(this, &UCM_QuitConfirmationWidget::GoBackToCompletionMenu);
		NoButton->OnPressed.AddDynamic(this, &UCM_QuitConfirmationWidget::GoBackToCompletionMenu);
	}
}

/*void UCM_QuitConfirmationWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}*/

void UCM_QuitConfirmationWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (YesButton)
	{
		YesButton->RemoveFromParent();
		YesButton = nullptr;
	}

	if (NoButton)
	{
		NoButton->RemoveFromParent();
		NoButton = nullptr;
	}

	if (CM_QuitConfirmationText)
	{
		CM_QuitConfirmationText->RemoveFromParent();
		CM_QuitConfirmationText = nullptr;
	}

	if (CM_QuitConfirmationCanvas)
	{
		CM_QuitConfirmationCanvas->RemoveFromParent();
		CM_QuitConfirmationCanvas = nullptr;
	}
}

void UCM_QuitConfirmationWidget::GoBackToCompletionMenu()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetCM_QuitConfirmationClass());
		gameMode->PushWidget(gameMode->GetCompletionClass());
	}

	RemoveBindedButtonDelegates();
}

void UCM_QuitConfirmationWidget::GoBackToMainMenu()
{
	RemoveBindedButtonDelegates();

	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetCM_QuitConfirmationClass());
		gameMode->PushWidget(gameMode->GetMainMenuClass());

		gameMode->ResetGame();
	}

	RemoveBindedButtonDelegates();
}

void UCM_QuitConfirmationWidget::RemoveBindedButtonDelegates()
{
	if (YesButton)
	{
		YesButton->OnClicked.RemoveDynamic(this, &UCM_QuitConfirmationWidget::GoBackToMainMenu);
		YesButton->OnPressed.RemoveDynamic(this, &UCM_QuitConfirmationWidget::GoBackToMainMenu);
	}

	if (NoButton)
	{
		NoButton->OnClicked.RemoveDynamic(this, &UCM_QuitConfirmationWidget::GoBackToCompletionMenu);
		NoButton->OnPressed.RemoveDynamic(this, &UCM_QuitConfirmationWidget::GoBackToCompletionMenu);
	}
}