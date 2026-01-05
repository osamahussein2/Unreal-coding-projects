// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/CutsceneFinishedWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameMode/GoedWareCutsceneGM.h"
#include "Kismet/GameplayStatics.h"

UCutsceneFinishedWidget::UCutsceneFinishedWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCutsceneFinishedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (GoBackToMainMenuButton)
	{
		FButtonStyle backButtonStyle = GoBackToMainMenuButton->GetStyle();
		FSlateBrush brushNormal;

		brushNormal.TintColor = FSlateColor(FLinearColor(0.445f, 0.41f, 0.234f, 1.0f));
		backButtonStyle.SetNormal(brushNormal);

		GoBackToMainMenuButton->SetStyle(backButtonStyle);

		GoBackToMainMenuButton->OnHovered.AddUniqueDynamic(this, &UCutsceneFinishedWidget::MakeBackNormalButtonInvisible);
		GoBackToMainMenuButton->OnClicked.AddUniqueDynamic(this, &UCutsceneFinishedWidget::GoBackToMainMenu);
		GoBackToMainMenuButton->OnPressed.AddUniqueDynamic(this, &UCutsceneFinishedWidget::GoBackToMainMenu);
	}
}

void UCutsceneFinishedWidget::GoBackToMainMenu()
{
	if (AGoedWareCutsceneGM* cutsceneGM = Cast<AGoedWareCutsceneGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		cutsceneGM->HideCutsceneFinished();
		cutsceneGM->ShowLoadingWidget(LoadToNextState::BackToMain);

		if (GoBackToMainMenuButton->OnHovered.IsBound())
		{
			GoBackToMainMenuButton->OnHovered.RemoveDynamic(this, &UCutsceneFinishedWidget::MakeBackNormalButtonInvisible);
		}

		GoBackToMainMenuButton->OnClicked.RemoveDynamic(this, &UCutsceneFinishedWidget::GoBackToMainMenu);
		GoBackToMainMenuButton->OnPressed.RemoveDynamic(this, &UCutsceneFinishedWidget::GoBackToMainMenu);
	}
}

void UCutsceneFinishedWidget::MakeBackNormalButtonInvisible()
{
	FButtonStyle backButtonStyle = GoBackToMainMenuButton->GetStyle();
	FSlateBrush brushNormal, brushHovered;

	brushNormal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	brushHovered.TintColor = FSlateColor(FLinearColor(0.445f, 0.41f, 0.234f, 1.0f));

	backButtonStyle.SetNormal(brushNormal);
	backButtonStyle.SetHovered(brushHovered);

	GoBackToMainMenuButton->SetStyle(backButtonStyle);

	if (GoBackToMainMenuButton->OnHovered.IsBound())
	{
		GoBackToMainMenuButton->OnHovered.RemoveDynamic(this, &UCutsceneFinishedWidget::MakeBackNormalButtonInvisible);
	}
}