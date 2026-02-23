// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/RewardWidget.h"
#include "Widget/FinalScoreWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/BrackeysGM.h"
#include "Player/PlayerCharacter.h"
#include "Character/InteractableFish.h"

URewardWidget::URewardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URewardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RewardCanvas && RewardCanvas->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		RewardCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (RewardOverlay && RewardOverlay->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		RewardOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (BlackImage && BlackImage->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		BlackImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (RewardImage && RewardImage->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		RewardImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (BackButton->GetVisibility() != ESlateVisibility::Visible)
	{
		BackButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddUniqueDynamic(this, &URewardWidget::DestroyRewardWidget);
		BackButton->OnPressed.AddUniqueDynamic(this, &URewardWidget::DestroyRewardWidget);
	}

	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		SetRewardImageMaterial(gameMode->GetRewardNumber());
	}
}

void URewardWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (RewardCanvas)
	{
		RewardCanvas->RemoveFromParent();
		RewardCanvas = nullptr;
	}

	if (RewardOverlay)
	{
		RewardOverlay->RemoveFromParent();
		RewardOverlay = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (RewardImage)
	{
		RewardImage->RemoveFromParent();
		RewardImage = nullptr;
	}

	if (BackButton)
	{
		BackButton->RemoveFromParent();
		BackButton = nullptr;
	}
}

void URewardWidget::SetRewardImageMaterial(int index_)
{
	if (rewardMaterials.IsEmpty()) return;

	if (RewardImage && rewardMaterials[index_])
	{
		RewardImage->SetBrushFromMaterial(rewardMaterials[index_]);
	}
}

void URewardWidget::DestroyRewardWidget()
{
	RemoveBindedDelegates();

	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (!gameMode->GetIsForMobile()) gameMode->DisableMouseCursor();

		gameMode->PopWidget(gameMode->GetRewardClass());
		gameMode->PushWidget(gameMode->GetMobileClass());

		gameMode->DestroyReward();

		gameMode->ShowGameTouchInterface();

		gameMode->ResetScores();

		gameMode->EndRhythmMinigame();

		DestroyInteractableFishTrigger();

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->ResetPlayerBackToWorld();

			// Make the player be able to free roam again
			player->ResetPlayerActions();
		}
	}
}

void URewardWidget::RemoveBindedDelegates()
{
	if (BackButton)
	{
		BackButton->OnClicked.RemoveDynamic(this, &URewardWidget::DestroyRewardWidget);
		BackButton->OnPressed.RemoveDynamic(this, &URewardWidget::DestroyRewardWidget);
	}
}

void URewardWidget::DestroyInteractableFishTrigger()
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (Cast<AInteractableFish>(player->GetInteractableFish()))
		{
			// Destroy the box trigger from the interactable fish so the player can't interact with it anymore after completion
			Cast<AInteractableFish>(player->GetInteractableFish())->DestroyBoxTrigger();
			player->InvalidateFishActor();
		}
	}
}
