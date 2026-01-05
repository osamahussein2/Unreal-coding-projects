// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/GoBackWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "GameMode/GoedWareCutsceneGM.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CutscenePlayer.h"

UGoBackWidget::UGoBackWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UGoBackWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BackButton)
	{
		FButtonStyle backButtonStyle = BackButton->GetStyle();
		FSlateBrush brushNormal;

		brushNormal.TintColor = FSlateColor(FLinearColor(0.445f, 0.41f, 0.234f, 1.0f));
		backButtonStyle.SetNormal(brushNormal);

		BackButton->SetStyle(backButtonStyle);

		BackButton->OnHovered.AddUniqueDynamic(this, &UGoBackWidget::MakeBackNormalButtonInvisible);
		BackButton->OnClicked.AddUniqueDynamic(this, &UGoBackWidget::GoBackToMainMenu);
		BackButton->OnPressed.AddUniqueDynamic(this, &UGoBackWidget::GoBackToMainMenu);
	}

	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACutscenePlayer::StaticClass(), foundPlayer);

	cutscenePlayer = Cast<ACutscenePlayer>(foundPlayer[0]);
	foundPlayer[0] = nullptr;
}

void UGoBackWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (cutscenePlayer.IsValid())
	{
		if (!cutscenePlayer->ReachedCreditsBillboard())
		{
			Super::NativeTick(MyGeometry, InDeltaTime);

			// Set camera location and rotation to the set camera credits location and rotation
			cutscenePlayer->MoveToCreditsCamera(InDeltaTime);

			// Hide the back button if it's visible
			if (BackButton->IsVisible()) BackButton->SetVisibility(ESlateVisibility::Hidden);
		}

		else
		{
			if (!BackButton->IsVisible())
			{
				Super::NativeTick(MyGeometry, InDeltaTime);

				if (AGoedWareCutsceneGM* cutsceneGM = Cast<AGoedWareCutsceneGM>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					cutsceneGM->CreateCreditsWidgetComponent();
				}

				// Show the back button if it's not visible and set the camera to be at the credits transform
				cutscenePlayer->SetCreditsCameraTransform();
				BackButton->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UGoBackWidget::GoBackToMainMenu()
{
	if (AGoedWareCutsceneGM* cutsceneGM = Cast<AGoedWareCutsceneGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		cutsceneGM->HideGoBackWidget();
		cutsceneGM->ShowMainMenu();

		if (BackButton->OnHovered.IsBound())
		{
			BackButton->OnHovered.RemoveDynamic(this, &UGoBackWidget::MakeBackNormalButtonInvisible);
		}

		if (cutscenePlayer.IsValid()) cutscenePlayer = nullptr;

		BackButton->OnClicked.RemoveDynamic(this, &UGoBackWidget::GoBackToMainMenu);
		BackButton->OnPressed.RemoveDynamic(this, &UGoBackWidget::GoBackToMainMenu);
	}
}

void UGoBackWidget::MakeBackNormalButtonInvisible()
{
	FButtonStyle backButtonStyle = BackButton->GetStyle();
	FSlateBrush brushNormal, brushHovered;

	brushNormal.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	brushHovered.TintColor = FSlateColor(FLinearColor(0.445f, 0.41f, 0.234f, 1.0f));

	backButtonStyle.SetNormal(brushNormal);
	backButtonStyle.SetHovered(brushHovered);

	BackButton->SetStyle(backButtonStyle);

	if (BackButton->OnHovered.IsBound())
	{
		BackButton->OnHovered.RemoveDynamic(this, &UGoBackWidget::MakeBackNormalButtonInvisible);
	}
}