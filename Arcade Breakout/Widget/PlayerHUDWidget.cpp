// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PlayerHUDWidget.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Guard.h"
#include "EngineUtils.h"
#include "Widget/DetectionOverlay.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"

UPlayerHUDWidget::UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (playerCharacter.IsValid())
	{
		playerCharacter->GetOnCoinCollected().AddUniqueDynamic(this, &UPlayerHUDWidget::UpdateCoinCollectedText);

		totalCoins = playerCharacter->GetTotalCoins();

		UpdateCoinCollectedText(playerCharacter->GetCoinsAmount());
	}

	for (TActorIterator<AGuard> guardIt(GetWorld()); guardIt; ++guardIt)
	{
		if (!guardIt || !guardIt->IsValidLowLevel() || guardIt->IsHidden()) continue;

		guardIt->GetOnStartDetection().AddUniqueDynamic(this, &UPlayerHUDWidget::SpawnDetectionBar);

		if (guardIt->GetDetectionValue() > 0.0f && guardIt->GetDetectionValue() < 1.0f)
		{
			SpawnDetectionBar(*guardIt);
		}

		guardIt->GetOnDetection().AddUniqueDynamic(this, &UPlayerHUDWidget::UpdateDetectionBar);
		guardIt->GetOnDetectedAnimation().AddUniqueDynamic(this, &UPlayerHUDWidget::AnimateDetectionBar);
		guardIt->GetOnStopDetection().AddUniqueDynamic(this, &UPlayerHUDWidget::DestroyDetectionBar);
	}
}

void UPlayerHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();

	for (TActorIterator<AGuard> guardIt(GetWorld()); guardIt; ++guardIt)
	{
		if (!guardIt || !guardIt->IsValidLowLevel() || guardIt->IsHidden()) continue;

		if (guardIt->GetOnStartDetection().IsBound())
		{
			guardIt->GetOnStartDetection().RemoveDynamic(this, &UPlayerHUDWidget::SpawnDetectionBar);
		}

		if (guardIt->GetOnDetection().IsBound())
		{
			guardIt->GetOnDetection().RemoveDynamic(this, &UPlayerHUDWidget::UpdateDetectionBar);
		}

		if (guardIt->GetOnDetectedAnimation().IsBound())
		{
			guardIt->GetOnDetectedAnimation().RemoveDynamic(this, &UPlayerHUDWidget::AnimateDetectionBar);
		}

		if (guardIt->GetOnStopDetection().IsBound())
		{
			guardIt->GetOnStopDetection().RemoveDynamic(this, &UPlayerHUDWidget::DestroyDetectionBar);
		}
	}

	for (int i = 0; i < detectionOverlays.Num(); ++i)
	{
		if (!detectionOverlays[i]) continue;

		detectionOverlays[i]->RemoveFromParent();
		detectionOverlays[i] = nullptr;
	}

	if (!detectionOverlays.IsEmpty()) detectionOverlays.Empty();

	if (playerCharacter.IsValid())
	{
		if (playerCharacter->GetOnCoinCollected().IsBound())
		{
			playerCharacter->GetOnCoinCollected().RemoveDynamic(this, &UPlayerHUDWidget::UpdateCoinCollectedText);
		}
	}

	if (TicketAmountText)
	{
		if (CoinOverlay) CoinOverlay->RemoveChild(TicketAmountText);
		TicketAmountText->RemoveFromParent();
		TicketAmountText = nullptr;
	}

	if (CoinOverlay)
	{
		CoinOverlay->RemoveFromParent();
		CoinOverlay = nullptr;
	}

	if (PlayerHUDCanvas)
	{
		PlayerHUDCanvas->RemoveFromParent();
		PlayerHUDCanvas = nullptr;
	}
}

void UPlayerHUDWidget::UpdateCoinCollectedText(int coinsCollected_)
{
	if (!TicketAmountText) return;

	TicketAmountText->SetText(FText::FromString(FString::FromInt(coinsCollected_) + "/" + FString::FromInt(totalCoins)));
}

void UPlayerHUDWidget::SpawnDetectionBar(AGuard* guard_)
{
	if (!IsValid(detectionOverlayClass) || !guard_) return;

	detectionOverlays.Add(WidgetTree->ConstructWidget<UDetectionOverlay>(detectionOverlayClass));

	const int lastIndex = detectionOverlays.Num() - 1;

	detectionOverlays[lastIndex]->SetGuard(guard_);
	if (detectionOverlays[lastIndex]->GetDetectionBar()) detectionOverlays[lastIndex]->GetDetectionBar()->SetPercent(0.0f);

	if (PlayerHUDCanvas) PlayerHUDCanvas->AddChildToCanvas(detectionOverlays[lastIndex]);

	if (UCanvasPanelSlot* canvasPanelSlot = Cast<UCanvasPanelSlot>(detectionOverlays[lastIndex]->Slot))
	{
		canvasPanelSlot->SetAnchors(detectionOverlayAnchor);
		canvasPanelSlot->SetPosition(detectionOverlayPosition);
		canvasPanelSlot->SetSize(detectionOverlaySize);
		canvasPanelSlot->SetAlignment(detectionOverlayAlignment);
	}
}

void UPlayerHUDWidget::UpdateDetectionBar(AGuard* guard_, float detectionValue_)
{
	for (UDetectionOverlay* detectionOverlay : detectionOverlays)
	{
		if (detectionOverlay->GetGuard() != guard_ || !detectionOverlay->GetDetectionBar() ||
			!playerCharacter.IsValid()) continue;

		detectionOverlay->GetDetectionBar()->SetPercent(detectionValue_);

		// Get the direction vector in world space (Position X = Forward, Position Y = Right)
		const FVector& WorldDelta = guard_->GetActorLocation() - playerCharacter->GetActorLocation();

		// Rotate world delta into starting local space (so UI up is always Forward)
		const FVector& LocalDelta = playerCharacter->GetActorRotation().UnrotateVector(WorldDelta);

		/* Convert 3D world space to 2D UI space by mapping world space forward (X) to UI space up (-Y) then world space right (Y) to
		UI space right (X), and scaling world units to UI pixel units by multiplying it with the local delta vector */
		FVector2D UIOffset = FVector2D(LocalDelta.Y, -LocalDelta.X);

		// Clamp to circular radius so icons stay inside the ring
		if (UIOffset.Size() > detectionOverlayRadius)
		{
			UIOffset = UIOffset.GetSafeNormal() * detectionOverlayRadius;
		}

		if (!detectionOverlay->Slot) return;

		if (UCanvasPanelSlot* canvasPanelSlot = Cast<UCanvasPanelSlot>(detectionOverlay->Slot))
		{
			// Apply final position of detection bar
			canvasPanelSlot->SetPosition(detectionOverlayPosition + UIOffset);

			// Apply final angle rotation of detection bar
			detectionOverlay->SetRenderTransformAngle(FMath::RadiansToDegrees(FMath::Atan2(LocalDelta.Y, LocalDelta.X)));
		}

		if (detectionValue_ >= 1.0f)
		{
			if (CoinOverlay)
			{
				if (TicketAmountText) CoinOverlay->RemoveChild(TicketAmountText);

				CoinOverlay->RemoveFromParent();
				CoinOverlay = nullptr;
			}
		}
	}
}

void UPlayerHUDWidget::AnimateDetectionBar(AGuard* guard_)
{
	for (int i = 0; i < detectionOverlays.Num(); ++i)
	{
		if (!detectionOverlays[i] || detectionOverlays[i]->GetGuard() != guard_) continue;

		if (scaleIncreasing)
		{
			detectionBarScale.X += detectionScaleSpeed * GetWorld()->GetDeltaSeconds();
			detectionBarScale.Y += detectionScaleSpeed * GetWorld()->GetDeltaSeconds();

			detectionBarScale.X = FMath::Clamp(detectionBarScale.X, minimumDetectionBarScale.X, maximumDetectionBarScale.Y);
			detectionBarScale.Y = FMath::Clamp(detectionBarScale.Y, minimumDetectionBarScale.X, maximumDetectionBarScale.Y);

			// Apply final scale of detection bar whether it's increasing or decreasing
			detectionOverlays[i]->SetRenderScale(detectionBarScale);

			if (detectionBarScale.X >= maximumDetectionBarScale.X && detectionBarScale.Y >= maximumDetectionBarScale.Y) scaleIncreasing = false;
		}

		else
		{
			detectionBarScale.X -= detectionScaleSpeed * GetWorld()->GetDeltaSeconds();
			detectionBarScale.Y -= detectionScaleSpeed * GetWorld()->GetDeltaSeconds();

			detectionBarScale.X = FMath::Clamp(detectionBarScale.X, minimumDetectionBarScale.X, maximumDetectionBarScale.Y);
			detectionBarScale.Y = FMath::Clamp(detectionBarScale.Y, minimumDetectionBarScale.X, maximumDetectionBarScale.Y);

			// Apply final scale of detection bar whether it's increasing or decreasing
			detectionOverlays[i]->SetRenderScale(detectionBarScale);

			if (detectionBarScale.X <= minimumDetectionBarScale.X && detectionBarScale.Y <= minimumDetectionBarScale.Y)
			{
				guard_->MoveCameraToGuardThatSpottedPlayer();
			}
		}
	}
}

void UPlayerHUDWidget::DestroyDetectionBar(AGuard* guard_)
{
	for (int i = 0; i < detectionOverlays.Num(); ++i)
	{
		if (!detectionOverlays[i] || detectionOverlays[i]->GetGuard() != guard_) continue;

		detectionOverlays[i]->RemoveFromParent();
		detectionOverlays[i] = nullptr;

		detectionOverlays.RemoveAt(i);
	}
}
