// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/NerveSequenceWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/GoedWareCommunityGM.h"

UNerveSequenceWidget::UNerveSequenceWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UNerveSequenceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NerveSequenceCanvas && NerveSequenceCanvas->GetVisibility() != ESlateVisibility::HitTestInvisible)
	{
		NerveSequenceCanvas->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (NerveSequenceOverlay && NerveSequenceOverlay->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		NerveSequenceOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (BlackImage && BlackImage->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		BlackImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (NerveImage && NerveImage->GetVisibility() != ESlateVisibility::Hidden)
	{
		NerveImage->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CurrentSequenceNumberText && CurrentSequenceNumberText->GetVisibility() != ESlateVisibility::Hidden)
	{
		CurrentSequenceNumberText->SetVisibility(ESlateVisibility::Hidden);
	}

	GetWorld()->GetTimerManager().SetTimer(nerveTimerHandle, this, &UNerveSequenceWidget::ShowNerveImage, 
		timeToUpdateNerveImage, false);
}

void UNerveSequenceWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}

void UNerveSequenceWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (NerveImage)
	{
		NerveImage->RemoveFromParent();
		NerveImage = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (NerveSequenceOverlay)
	{
		NerveSequenceOverlay->RemoveFromParent();
		NerveSequenceOverlay = nullptr;
	}

	if (NerveSequenceCanvas)
	{
		NerveSequenceCanvas->RemoveFromParent();
		NerveSequenceCanvas = nullptr;
	}
}

void UNerveSequenceWidget::ShowNerveImage()
{
	if (NerveImage && CurrentSequenceNumberText)
	{
		if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			switch (gameMode->GetCurrentCompletedSequences())
			{
			case 0: // Blue nerves

				if (gameMode->GetBlueNerveTextureByIndex(gameMode->GetRandomNerveIndex(gameMode->GetCurrentPlayedSequences())))
				{
					NerveImage->SetBrushFromTexture(gameMode->GetBlueNerveTextureByIndex(gameMode->GetRandomNerveIndex(
						gameMode->GetCurrentPlayedSequences())));

					gameMode->IncrementPlayedSequences();
					CurrentSequenceNumberText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPlayedSequences())));
				}

				break;

			case 1: // Orange nerves

				if (gameMode->GetOrangeNerveTextureByIndex(gameMode->GetRandomNerveIndex(gameMode->GetCurrentPlayedSequences())))
				{
					NerveImage->SetBrushFromTexture(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetRandomNerveIndex(
						gameMode->GetCurrentPlayedSequences())));

					gameMode->IncrementPlayedSequences();
					CurrentSequenceNumberText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPlayedSequences())));
				}

				break;

			case 2: // Purple nerves

				if (gameMode->GetPurpleNerveTextureByIndex(gameMode->GetRandomNerveIndex(gameMode->GetCurrentPlayedSequences())))
				{
					NerveImage->SetBrushFromTexture(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetRandomNerveIndex(
						gameMode->GetCurrentPlayedSequences())));

					gameMode->IncrementPlayedSequences();
					CurrentSequenceNumberText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPlayedSequences())));
				}

				break;

			case 3: // Yellow nerves

				if (gameMode->GetYellowNerveTextureByIndex(gameMode->GetRandomNerveIndex(gameMode->GetCurrentPlayedSequences())))
				{
					NerveImage->SetBrushFromTexture(gameMode->GetYellowNerveTextureByIndex(gameMode->GetRandomNerveIndex(
						gameMode->GetCurrentPlayedSequences())));

					gameMode->IncrementPlayedSequences();
					CurrentSequenceNumberText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPlayedSequences())));
				}

				break;
			}
		}

		if (!NerveImage->IsVisible()) NerveImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (!CurrentSequenceNumberText->IsVisible()) CurrentSequenceNumberText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// Clear the active nerve timer handle once the nerve image is shown
		if (GetWorld()->GetTimerManager().IsTimerActive(nerveTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(nerveTimerHandle);
		}

		GetWorld()->GetTimerManager().SetTimer(nerveTimerHandle, this, &UNerveSequenceWidget::UpdateNerveImageSequence,
			timeToUpdateNerveImage, true);
	}
}

void UNerveSequenceWidget::UpdateNerveImageSequence()
{
	if (NerveImage)
	{
		if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			switch (gameMode->GetCurrentCompletedSequences())
			{
			case 0: // Blue nerves
				if (gameMode->GetBlueNerveTextureByIndex(gameMode->GetRandomNerveIndex(gameMode->GetCurrentPlayedSequences())))
				{
					NerveImage->SetBrushFromTexture(gameMode->GetBlueNerveTextureByIndex(gameMode->GetRandomNerveIndex(
						gameMode->GetCurrentPlayedSequences())));

					gameMode->IncrementPlayedSequences();
					CurrentSequenceNumberText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPlayedSequences())));
				}

				break;

			case 1: // Orange nerves

				if (gameMode->GetOrangeNerveTextureByIndex(gameMode->GetRandomNerveIndex(gameMode->GetCurrentPlayedSequences())))
				{
					NerveImage->SetBrushFromTexture(gameMode->GetOrangeNerveTextureByIndex(gameMode->GetRandomNerveIndex(
						gameMode->GetCurrentPlayedSequences())));

					gameMode->IncrementPlayedSequences();
					CurrentSequenceNumberText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPlayedSequences())));
				}

				break;

			case 2: // Purple nerves

				if (gameMode->GetPurpleNerveTextureByIndex(gameMode->GetRandomNerveIndex(gameMode->GetCurrentPlayedSequences())))
				{
					NerveImage->SetBrushFromTexture(gameMode->GetPurpleNerveTextureByIndex(gameMode->GetRandomNerveIndex(
						gameMode->GetCurrentPlayedSequences())));

					gameMode->IncrementPlayedSequences();
					CurrentSequenceNumberText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPlayedSequences())));
				}

				break;

			case 3: // Yellow nerves

				if (gameMode->GetYellowNerveTextureByIndex(gameMode->GetRandomNerveIndex(gameMode->GetCurrentPlayedSequences())))
				{
					NerveImage->SetBrushFromTexture(gameMode->GetYellowNerveTextureByIndex(gameMode->GetRandomNerveIndex(
						gameMode->GetCurrentPlayedSequences())));

					gameMode->IncrementPlayedSequences();
					CurrentSequenceNumberText->SetText(FText::FromString(FString::FromInt(gameMode->GetCurrentPlayedSequences())));
				}

				break;
			}

			if (gameMode->GetCurrentPlayedSequences() >= gameMode->GetMaxPlayedSequences())
			{
				// Clear the active nerve timer handle once all sequences are completed
				if (GetWorld()->GetTimerManager().IsTimerActive(nerveTimerHandle))
				{
					GetWorld()->GetTimerManager().ClearTimer(nerveTimerHandle);
				}

				GetWorld()->GetTimerManager().SetTimer(nerveTimerHandle, this, &UNerveSequenceWidget::ShowPickNervesWidget,
					timeToUpdateNerveImage, false);
			}
		}
	}
}

void UNerveSequenceWidget::ShowPickNervesWidget()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// Clear the active nerve timer handle once all sequences are completed
		if (GetWorld()->GetTimerManager().IsTimerActive(nerveTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(nerveTimerHandle);
		}

		gameMode->PopWidget(gameMode->GetNerveSequenceClass());

		switch (gameMode->GetCurrentCompletedSequences())
		{
		case 0:
			gameMode->PushWidget(gameMode->GetPlayerHUDClass());
			gameMode->PushWidget(gameMode->GetPickThreeNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::EPickingThree);
			break;

		case 1:
			gameMode->PushWidget(gameMode->GetPlayerHUDClass());
			gameMode->PushWidget(gameMode->GetPickFourNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::EPickingFour);
			break;

		case 2:
			gameMode->PushWidget(gameMode->GetPlayerHUDClass());
			gameMode->PushWidget(gameMode->GetPickFourNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::EPickingFour);
			break;

		case 3:
			gameMode->PushWidget(gameMode->GetPlayerHUDClass());
			gameMode->PushWidget(gameMode->GetPickFiveNervesClass());
			gameMode->SetPickNerveState(EPickNerveState::EPickingFive);
			break;
		}
	}
}
