// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/SequenceStatsWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/GoedWareCommunityGM.h"

USequenceStatsWidget::USequenceStatsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USequenceStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SequenceStatsCanvas && SequenceStatsCanvas->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		SequenceStatsCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (SequenceStatsOverlay && SequenceStatsOverlay->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		SequenceStatsOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (BlackImage && BlackImage->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		BlackImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (SequenceStatsText && SequenceStatsText->GetVisibility() != ESlateVisibility::Visible)
	{
		SequenceStatsText->SetVisibility(ESlateVisibility::Visible);
	}

	if (SequenceStatsText)
	{
		if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			SequenceStatsText->SetText(FText::FromString(TEXT("Correct sequences: ") + 
				FString::FromInt(gameMode->GetCurrentCorrectSequences()) + TEXT("/") + 
				FString::FromInt(gameMode->GetMaxCorrectSequences())));

			if (gameMode->GetCurrentCorrectSequences() >= gameMode->GetMaxCorrectSequences())
			{
				gameMode->IncrementCompletedSequences();
			}
		}
	}

	FTimerHandle statsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(statsTimerHandle, this, &USequenceStatsWidget::DestroySequenceStats, 
		timeToDestroySequenceStats, false);
}

void USequenceStatsWidget::NativeDestruct()
{
	Super::NativeDestruct();

	DestroyWidgetComponents();
}

void USequenceStatsWidget::DestroyWidgetComponents()
{
	if (SequenceStatsText)
	{
		SequenceStatsText->RemoveFromParent();
		SequenceStatsText = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (SequenceStatsOverlay)
	{
		SequenceStatsOverlay->RemoveFromParent();
		SequenceStatsOverlay = nullptr;
	}

	if (SequenceStatsCanvas)
	{
		SequenceStatsCanvas->RemoveFromParent();
		SequenceStatsCanvas = nullptr;
	}
}

void USequenceStatsWidget::DestroySequenceStats()
{
	DestroyWidgetComponents();

	// Change the body nerve sprite if the player got all the sequences right
	FTimerHandle spawnNervesTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(spawnNervesTimerHandle, this, &USequenceStatsWidget::SpawnBodyNerves,
		timeToSpawnBodyNerves, false);
}

void USequenceStatsWidget::SpawnBodyNerves()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// If the player hasn't completed all the sequences yet, show the nerve sequence again to complete another sequence
		if (gameMode->GetCurrentCompletedSequences() < gameMode->GetMaxCompletedSequences())
		{
			// Spawn the right body nerves if the player got all the sequences right
			if (gameMode->GetCurrentCorrectSequences() >= gameMode->GetMaxCorrectSequences())
			{
				UGameplayStatics::PlaySound2D(GetWorld(), sequenceSounds[0]); // Play sequence correct sound
				//gameMode->SpawnNerveActors();

				gameMode->UpdateBodySprite();

				// Update max played and max correct sequence values upon completing a sequence
				gameMode->UpdateValuesBasedOnCompletedSequences();
			}

			else
			{
				UGameplayStatics::PlaySound2D(GetWorld(), sequenceSounds[1]); // Play sequence wrong sound
			}

			// Reset current sequence stats to complete another sequence
			gameMode->ResetCurrentSequenceStats();

			FTimerHandle nerveSequenceTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(nerveSequenceTimerHandle, this, &USequenceStatsWidget::ShowNerveSequence,
				timeToShowNerveSequence, false);
		}

		else
		{
			UGameplayStatics::PlaySound2D(GetWorld(), sequenceSounds[0]); // Play sequence correct sound
			//gameMode->SpawnNerveActors();

			gameMode->UpdateBodySprite();

			FTimerHandle nerveSequenceTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(nerveSequenceTimerHandle, this, &USequenceStatsWidget::ShowCompletion,
				timeToShowNerveSequence, false);
		}
	}
}

void USequenceStatsWidget::ShowNerveSequence()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetSequenceStatsClass());
		gameMode->PushWidget(gameMode->GetNerveSequenceClass());
	}
}

void USequenceStatsWidget::ShowCompletion()
{
	if (AGoedWareCommunityGM* gameMode = Cast<AGoedWareCommunityGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->DestroyBodyActor();

		gameMode->PopWidget(gameMode->GetSequenceStatsClass());
		gameMode->PushWidget(gameMode->GetCompletionClass());
	}
}
