// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/RhythmMinigameMobileWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameMode/BrackeysGM.h"
#include "Components/OverlaySlot.h"
#include "Blueprint/WidgetTree.h"
#include "Widget/FallingImageWidget.h"
#include "Player/PlayerCharacter.h"

URhythmMinigameMobileWidget::URhythmMinigameMobileWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URhythmMinigameMobileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RhythmMinigameCanvas && RhythmMinigameCanvas->GetVisibility() != ESlateVisibility::Visible)
	{
		RhythmMinigameCanvas->SetVisibility(ESlateVisibility::Visible);
	}

	if (RhythmMinigameOverlay && RhythmMinigameOverlay->GetVisibility() != ESlateVisibility::Visible)
	{
		RhythmMinigameOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (BlackImage && BlackImage->GetVisibility() != ESlateVisibility::Visible)
	{
		BlackImage->SetVisibility(ESlateVisibility::Visible);
	}

	if (AButton && AButton->GetVisibility() != ESlateVisibility::Visible)
	{
		AButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (SButton && SButton->GetVisibility() != ESlateVisibility::Visible)
	{
		SButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (DButton && DButton->GetVisibility() != ESlateVisibility::Visible)
	{
		DButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (FButton && FButton->GetVisibility() != ESlateVisibility::Visible)
	{
		FButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayerText && PlayerText->GetVisibility() != ESlateVisibility::Visible)
	{
		PlayerText->SetVisibility(ESlateVisibility::Visible);
	}

	if (EnemyText && EnemyText->GetVisibility() != ESlateVisibility::Visible)
	{
		EnemyText->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayerScoreText && PlayerScoreText->GetVisibility() != ESlateVisibility::Visible)
	{
		PlayerScoreText->SetVisibility(ESlateVisibility::Visible);
	}

	if (EnemyScoreText && EnemyScoreText->GetVisibility() != ESlateVisibility::Visible)
	{
		EnemyScoreText->SetVisibility(ESlateVisibility::Visible);
	}

	if (PauseGameButton && PauseGameButton->GetVisibility() != ESlateVisibility::Visible)
	{
		PauseGameButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (PlayerScoreText)
	{
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			currentSFXVolume = gameMode->GetSFXVolume();
			PlayerScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetPlayerScore())));
		}
	}

	if (EnemyScoreText)
	{
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->PlayBattleMusic();
			EnemyScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetEnemyScore())));
		}
	}

	if (AButton) AButton->OnPressed.AddUniqueDynamic(this, &URhythmMinigameMobileWidget::PressOnFallingImageA);
	if (SButton) SButton->OnPressed.AddUniqueDynamic(this, &URhythmMinigameMobileWidget::PressOnFallingImageS);
	if (DButton) DButton->OnPressed.AddUniqueDynamic(this, &URhythmMinigameMobileWidget::PressOnFallingImageD);
	if (FButton) FButton->OnPressed.AddUniqueDynamic(this, &URhythmMinigameMobileWidget::PressOnFallingImageF);
	if (PauseGameButton) PauseGameButton->OnPressed.AddUniqueDynamic(this, &URhythmMinigameMobileWidget::PauseGame);

	const int& randomReadyIndex = FMath::RandRange(0, readySounds.Num() - 1);
	UGameplayStatics::PlaySound2D(GetWorld(), readySounds[randomReadyIndex], currentSFXVolume);

	FTimerHandle enemyDanceHandler;
	GetWorld()->GetTimerManager().SetTimer(enemyDanceHandler, this, &URhythmMinigameMobileWidget::StartDancing, timeToStartDancing, false);
}

void URhythmMinigameMobileWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	// If the game isn't paused, update the rhythm minigame
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		spawnTimer += DeltaTime;

		// Show the rhythm minigame canvas if it's not visible already
		if (RhythmMinigameCanvas && RhythmMinigameCanvas->GetVisibility() != ESlateVisibility::Visible)
		{
			if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				if (currentMusicVolume != gameMode->GetMusicVolume()) currentMusicVolume = gameMode->GetMusicVolume();
				if (currentSFXVolume != gameMode->GetSFXVolume()) currentSFXVolume = gameMode->GetSFXVolume();
			}

			RhythmMinigameCanvas->SetVisibility(ESlateVisibility::Visible);
		}

		// Spawn falling images for either A, S, D or F using a randomized index
		if (spawnTimer >= maxSpawnTime && fallingImagesSpawned < maxFallingImagesSpawned)
		{
			const int& randomizedIndex = FMath::RandRange(0, 3);

			switch (randomizedIndex)
			{
			case 0:
				SpawnFallingImagesForA();
				++fallingImagesSpawned;
				break;

			case 1:
				SpawnFallingImagesForS();
				++fallingImagesSpawned;
				break;

			case 2:
				SpawnFallingImagesForD();
				++fallingImagesSpawned;
				break;

			case 3:
				SpawnFallingImagesForF();
				++fallingImagesSpawned;
				break;

			default:
				break;
			}

			if (fallingImagesSpawned <= 1)
			{
				const int& randomStartIndex = FMath::RandRange(0, startSounds.Num() - 1);
				UGameplayStatics::PlaySound2D(GetWorld(), startSounds[randomStartIndex], currentSFXVolume);
			}

			spawnTimer = 0.0f; // Restart spawn timer
		}

		UpdateFallingImagesForA();
		UpdateFallingImagesForS();
		UpdateFallingImagesForD();
		UpdateFallingImagesForF();

		// Remove rhythm minigame widget after falling images spawned value reaches its max spawn value to stop minigame
		if (fallingImagesSpawned >= maxFallingImagesSpawned &&
			imagesForA.IsEmpty() && imagesForS.IsEmpty() &&
			imagesForD.IsEmpty() && imagesForF.IsEmpty())
		{
			minigameFinishedTimer += DeltaTime;

			if (minigameFinishedTimer >= timeToEndMinigame) EndMinigame();
		}
	}

	else
	{
		// Collapse the rhythm minigame canvas whenever the game is paused
		if (RhythmMinigameCanvas && RhythmMinigameCanvas->GetVisibility() != ESlateVisibility::Collapsed)
		{
			RhythmMinigameCanvas->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URhythmMinigameMobileWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (GetWorld()->GetTimerManager().IsTimerActive(enemyIncreaseScoreHandler))
	{
		GetWorld()->GetTimerManager().ClearTimer(enemyIncreaseScoreHandler);
	}

	if (AButton && AButton->OnPressed.IsBound()) AButton->OnPressed.RemoveDynamic(this, &URhythmMinigameMobileWidget::PressOnFallingImageA);
	if (SButton && SButton->OnPressed.IsBound()) SButton->OnPressed.RemoveDynamic(this, &URhythmMinigameMobileWidget::PressOnFallingImageS);
	if (DButton && DButton->OnPressed.IsBound()) DButton->OnPressed.RemoveDynamic(this, &URhythmMinigameMobileWidget::PressOnFallingImageD);
	if (FButton && FButton->OnPressed.IsBound()) FButton->OnPressed.RemoveDynamic(this, &URhythmMinigameMobileWidget::PressOnFallingImageF);
	if (PauseGameButton && PauseGameButton->OnPressed.IsBound()) PauseGameButton->OnPressed.RemoveDynamic(this, &URhythmMinigameMobileWidget::PauseGame);

	minigameFinishedTimer = 0.0f;

	for (UFallingImageWidget* imageA : imagesForA)
	{
		imageA->RemoveFromParent();
		imageA = nullptr;
	}

	for (UFallingImageWidget* imageS : imagesForS)
	{
		imageS->RemoveFromParent();
		imageS = nullptr;
	}

	for (UFallingImageWidget* imageD : imagesForD)
	{
		imageD->RemoveFromParent();
		imageD = nullptr;
	}

	for (UFallingImageWidget* imageF : imagesForF)
	{
		imageF->RemoveFromParent();
		imageF = nullptr;
	}

	if (!imagesForA.IsEmpty()) imagesForA.Empty();
	if (!imagesForS.IsEmpty()) imagesForS.Empty();
	if (!imagesForD.IsEmpty()) imagesForD.Empty();
	if (!imagesForF.IsEmpty()) imagesForF.Empty();

	for (USoundBase* noteSound : noteSounds) noteSound = nullptr;

	if (!noteSounds.IsEmpty()) noteSounds.Empty();

	if (FButton)
	{
		FButton->RemoveFromParent();
		FButton = nullptr;
	}

	if (DButton)
	{
		DButton->RemoveFromParent();
		DButton = nullptr;
	}

	if (SButton)
	{
		SButton->RemoveFromParent();
		SButton = nullptr;
	}

	if (AButton)
	{
		AButton->RemoveFromParent();
		AButton = nullptr;
	}

	if (PlayerText)
	{
		PlayerText->RemoveFromParent();
		PlayerText = nullptr;
	}

	if (EnemyText)
	{
		EnemyText->RemoveFromParent();
		EnemyText = nullptr;
	}

	if (PlayerScoreText)
	{
		PlayerScoreText->RemoveFromParent();
		PlayerScoreText = nullptr;
	}

	if (EnemyScoreText)
	{
		EnemyScoreText->RemoveFromParent();
		EnemyScoreText = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (RhythmMinigameOverlay)
	{
		RhythmMinigameOverlay->RemoveFromParent();
		RhythmMinigameOverlay = nullptr;
	}

	if (RhythmMinigameCanvas)
	{
		RhythmMinigameCanvas->RemoveFromParent();
		RhythmMinigameCanvas = nullptr;
	}
}

void URhythmMinigameMobileWidget::SpawnFallingImagesForA()
{
	if (fallingImageClass) imagesForA.Add(WidgetTree->ConstructWidget<UFallingImageWidget>(fallingImageClass));

	for (int i = imagesForA.Num() - 1; i < imagesForA.Num(); ++i)
	{
		imagesForA[i]->SetVisibility(ESlateVisibility::HitTestInvisible);

		RhythmMinigameOverlay->AddChildToOverlay(imagesForA[i]);

		if (UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(imagesForA[i]->Slot))
		{
			overlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
			overlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);

			overlaySlot->SetPadding(FMargin(imageAPadding.Left, imageAPadding.Top,
				imageAPadding.Right, imageAPadding.Bottom));
		}
	}
}

void URhythmMinigameMobileWidget::SpawnFallingImagesForS()
{
	if (fallingImageClass) imagesForS.Add(WidgetTree->ConstructWidget<UFallingImageWidget>(fallingImageClass));

	for (int i = imagesForS.Num() - 1; i < imagesForS.Num(); ++i)
	{
		imagesForS[i]->SetVisibility(ESlateVisibility::HitTestInvisible);

		RhythmMinigameOverlay->AddChildToOverlay(imagesForS[i]);

		if (UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(imagesForS[i]->Slot))
		{
			overlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
			overlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);

			overlaySlot->SetPadding(FMargin(imageSPadding.Left, imageSPadding.Top,
				imageSPadding.Right, imageSPadding.Bottom));
		}
	}
}

void URhythmMinigameMobileWidget::SpawnFallingImagesForD()
{
	if (fallingImageClass) imagesForD.Add(WidgetTree->ConstructWidget<UFallingImageWidget>(fallingImageClass));

	for (int i = imagesForD.Num() - 1; i < imagesForD.Num(); ++i)
	{
		imagesForD[i]->SetVisibility(ESlateVisibility::HitTestInvisible);

		RhythmMinigameOverlay->AddChildToOverlay(imagesForD[i]);

		if (UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(imagesForD[i]->Slot))
		{
			overlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
			overlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);

			overlaySlot->SetPadding(FMargin(imageDPadding.Left, imageDPadding.Top,
				imageDPadding.Right, imageDPadding.Bottom));
		}
	}
}

void URhythmMinigameMobileWidget::SpawnFallingImagesForF()
{
	if (fallingImageClass) imagesForF.Add(WidgetTree->ConstructWidget<UFallingImageWidget>(fallingImageClass));

	for (int i = imagesForF.Num() - 1; i < imagesForF.Num(); ++i)
	{
		imagesForF[i]->SetVisibility(ESlateVisibility::HitTestInvisible);

		RhythmMinigameOverlay->AddChildToOverlay(imagesForF[i]);

		if (UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(imagesForF[i]->Slot))
		{
			overlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
			overlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);

			overlaySlot->SetPadding(FMargin(imageFPadding.Left, imageFPadding.Top,
				imageFPadding.Right, imageFPadding.Bottom));
		}
	}
}

void URhythmMinigameMobileWidget::UpdateFallingImagesForA()
{
	for (int i = 0; i < imagesForA.Num(); ++i)
	{
		if (UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(imagesForA[i]->Slot))
		{
			overlaySlot->SetPadding(overlaySlot->GetPadding() + FMargin(0.0f, imagesForA[i]->GetFallingImagePositionY(), 0.0f, 0.0f));

			if (overlaySlot->GetPadding().Top >= maxYThreshold)
			{
				DetermineEnemyScore();

				PlayPlayerMissedNoteDance();
				PlayMissNoteSound();

				imagesForA[i]->RemoveFromParent();
				imagesForA[i] = nullptr;

				imagesForA.RemoveAt(i);

				//UE_LOG(LogTemp, Warning, TEXT("Falling image is destroyed!"));
			}

			//UE_LOG(LogTemp, Warning, TEXT("%f"), overlaySlot->GetPadding().Top);
		}
	}
}

void URhythmMinigameMobileWidget::UpdateFallingImagesForS()
{
	for (int i = 0; i < imagesForS.Num(); ++i)
	{
		if (UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(imagesForS[i]->Slot))
		{
			overlaySlot->SetPadding(overlaySlot->GetPadding() + FMargin(0.0f, imagesForS[i]->GetFallingImagePositionY(), 0.0f, 0.0f));

			if (overlaySlot->GetPadding().Top >= maxYThreshold)
			{
				DetermineEnemyScore();

				PlayPlayerMissedNoteDance();
				PlayMissNoteSound();

				imagesForS[i]->RemoveFromParent();
				imagesForS[i] = nullptr;

				imagesForS.RemoveAt(i);

				//UE_LOG(LogTemp, Warning, TEXT("Falling image is destroyed!"));
			}

			//UE_LOG(LogTemp, Warning, TEXT("%f"), overlaySlot->GetPadding().Top);
		}
	}
}

void URhythmMinigameMobileWidget::UpdateFallingImagesForD()
{
	for (int i = 0; i < imagesForD.Num(); ++i)
	{
		if (UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(imagesForD[i]->Slot))
		{
			overlaySlot->SetPadding(overlaySlot->GetPadding() + FMargin(0.0f, imagesForD[i]->GetFallingImagePositionY(), 0.0f, 0.0f));

			if (overlaySlot->GetPadding().Top >= maxYThreshold)
			{
				DetermineEnemyScore();

				PlayPlayerMissedNoteDance();
				PlayMissNoteSound();

				imagesForD[i]->RemoveFromParent();
				imagesForD[i] = nullptr;

				imagesForD.RemoveAt(i);

				//UE_LOG(LogTemp, Warning, TEXT("Falling image is destroyed!"));
			}

			//UE_LOG(LogTemp, Warning, TEXT("%f"), overlaySlot->GetPadding().Top);
		}
	}
}

void URhythmMinigameMobileWidget::UpdateFallingImagesForF()
{
	for (int i = 0; i < imagesForF.Num(); ++i)
	{
		if (UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(imagesForF[i]->Slot))
		{
			overlaySlot->SetPadding(overlaySlot->GetPadding() + FMargin(0.0f, imagesForF[i]->GetFallingImagePositionY(), 0.0f, 0.0f));

			if (overlaySlot->GetPadding().Top >= maxYThreshold)
			{
				DetermineEnemyScore();

				PlayPlayerMissedNoteDance();
				PlayMissNoteSound();

				imagesForF[i]->RemoveFromParent();
				imagesForF[i] = nullptr;

				imagesForF.RemoveAt(i);

				//UE_LOG(LogTemp, Warning, TEXT("Falling image is destroyed!"));
			}

			//UE_LOG(LogTemp, Warning, TEXT("%f"), overlaySlot->GetPadding().Top);
		}
	}
}

void URhythmMinigameMobileWidget::PressOnFallingImageA()
{
	for (int i = 0; i < imagesForA.Num(); ++i)
	{
		if (UOverlaySlot* overlayImageASlot = Cast<UOverlaySlot>(imagesForA[i]->Slot))
		{
			if (UOverlaySlot* overlayTextASlot = Cast<UOverlaySlot>(AButton->Slot))
			{
				const float& distance = FMath::Abs(overlayTextASlot->GetPadding().Top - overlayImageASlot->GetPadding().Top);

				if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					// If the falling images for A is slightly above or at the A text, increment the score
					if (distance > maxDistanceToTexts - distanceOffset && distance <= maxDistanceToTexts)
					{
						const float& actualDistance = ((distance - (maxDistanceToTexts - distanceOffset)) / 100.0f);

						int scoreFromDistance = static_cast<int>(actualDistance * maxScoreToIncrement);

						// If it reaches within the distance offset for perfect score zone, award the max score
						if (distance > maxDistanceToTexts - distanceOffsetForPerfectScore)
						{
							scoreFromDistance = maxScoreToIncrement;
						}

						gameMode->IncrementPlayerScore(scoreFromDistance);

						if (PlayerScoreText)
						{
							PlayerScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetPlayerScore())));
							//PlayerScoreText->SetText(FText::FromString(FString::FromInt(scoreFromDistance)));
						}

						PlayHitNoteSound();

						GetWorld()->GetTimerManager().SetTimer(enemyIncreaseScoreHandler, this,
							&URhythmMinigameMobileWidget::DetermineEnemyScore, timeToUpdateEnemyScore, false);

						imagesForA[i]->RemoveFromParent();
						imagesForA[i] = nullptr;

						imagesForA.RemoveAt(i);
					}

					// If the falling images for A is at the A text or slightly below it, increment the score
					else if (distance >= maxDistanceToTexts && distance < maxDistanceToTexts + distanceOffset)
					{
						const float& actualDistance = (((maxDistanceToTexts + distanceOffset) - distance) / 100.0f);

						int scoreFromDistance = static_cast<int>(actualDistance * maxScoreToIncrement);

						// If it reaches within the distance offset for perfect score zone, award the max score
						if (distance < maxDistanceToTexts + distanceOffsetForPerfectScore)
						{
							scoreFromDistance = maxScoreToIncrement;
						}

						gameMode->IncrementPlayerScore(scoreFromDistance);

						if (PlayerScoreText)
						{
							PlayerScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetPlayerScore())));
							//PlayerScoreText->SetText(FText::FromString(FString::FromInt(scoreFromDistance)));
						}

						PlayHitNoteSound();

						GetWorld()->GetTimerManager().SetTimer(enemyIncreaseScoreHandler, this,
							&URhythmMinigameMobileWidget::DetermineEnemyScore, timeToUpdateEnemyScore, false);

						imagesForA[i]->RemoveFromParent();
						imagesForA[i] = nullptr;

						imagesForA.RemoveAt(i);
					}
				}

				UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), distance);
			}
		}
	}
}

void URhythmMinigameMobileWidget::PressOnFallingImageS()
{
	for (int i = 0; i < imagesForS.Num(); ++i)
	{
		if (UOverlaySlot* overlayImageSSlot = Cast<UOverlaySlot>(imagesForS[i]->Slot))
		{
			if (UOverlaySlot* overlayTextSSlot = Cast<UOverlaySlot>(SButton->Slot))
			{
				const float& distance = FMath::Abs(overlayTextSSlot->GetPadding().Top - overlayImageSSlot->GetPadding().Top);

				if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					// If the falling images for S is slightly above or at the S text, increment the score
					if (distance > maxDistanceToTexts - distanceOffset && distance <= maxDistanceToTexts)
					{
						const float& actualDistance = ((distance - (maxDistanceToTexts - distanceOffset)) / 100.0f);

						int scoreFromDistance = static_cast<int>(actualDistance * maxScoreToIncrement);

						// If it reaches within the distance offset for perfect score zone, award the max score
						if (distance > maxDistanceToTexts - distanceOffsetForPerfectScore)
						{
							scoreFromDistance = maxScoreToIncrement;
						}

						gameMode->IncrementPlayerScore(scoreFromDistance);

						if (PlayerScoreText)
						{
							PlayerScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetPlayerScore())));
							//PlayerScoreText->SetText(FText::FromString(FString::FromInt(scoreFromDistance)));
						}

						PlayHitNoteSound();

						GetWorld()->GetTimerManager().SetTimer(enemyIncreaseScoreHandler, this,
							&URhythmMinigameMobileWidget::DetermineEnemyScore, timeToUpdateEnemyScore, false);

						imagesForS[i]->RemoveFromParent();
						imagesForS[i] = nullptr;

						imagesForS.RemoveAt(i);
					}

					// If the falling images for S is at the S text or slightly below it, increment the score
					else if (distance >= maxDistanceToTexts && distance < maxDistanceToTexts + distanceOffset)
					{
						const float& actualDistance = (((maxDistanceToTexts + distanceOffset) - distance) / 100.0f);

						int scoreFromDistance = static_cast<int>(actualDistance * maxScoreToIncrement);

						// If it reaches within the distance offset for perfect score zone, award the max score
						if (distance < maxDistanceToTexts + distanceOffsetForPerfectScore)
						{
							scoreFromDistance = maxScoreToIncrement;
						}

						gameMode->IncrementPlayerScore(scoreFromDistance);

						if (PlayerScoreText)
						{
							PlayerScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetPlayerScore())));
							//PlayerScoreText->SetText(FText::FromString(FString::FromInt(scoreFromDistance)));
						}

						PlayHitNoteSound();

						GetWorld()->GetTimerManager().SetTimer(enemyIncreaseScoreHandler, this,
							&URhythmMinigameMobileWidget::DetermineEnemyScore, timeToUpdateEnemyScore, false);

						imagesForS[i]->RemoveFromParent();
						imagesForS[i] = nullptr;

						imagesForS.RemoveAt(i);
					}
				}

				//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), distance);
			}
		}
	}
}

void URhythmMinigameMobileWidget::PressOnFallingImageD()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed D"));

	for (int i = 0; i < imagesForD.Num(); ++i)
	{
		if (UOverlaySlot* overlayImageDSlot = Cast<UOverlaySlot>(imagesForD[i]->Slot))
		{
			if (UOverlaySlot* overlayTextDSlot = Cast<UOverlaySlot>(DButton->Slot))
			{
				const float& distance = FMath::Abs(overlayTextDSlot->GetPadding().Top - overlayImageDSlot->GetPadding().Top);

				if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					// If the falling images for D is slightly above or at the D text, increment the score
					if (distance > maxDistanceToTexts - distanceOffset && distance <= maxDistanceToTexts)
					{
						const float& actualDistance = ((distance - (maxDistanceToTexts - distanceOffset)) / 100.0f);

						int scoreFromDistance = static_cast<int>(actualDistance * maxScoreToIncrement);

						// If it reaches within the distance offset for perfect score zone, award the max score
						if (distance > maxDistanceToTexts - distanceOffsetForPerfectScore)
						{
							scoreFromDistance = maxScoreToIncrement;
						}

						gameMode->IncrementPlayerScore(scoreFromDistance);

						if (PlayerScoreText)
						{
							PlayerScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetPlayerScore())));
							//PlayerScoreText->SetText(FText::FromString(FString::FromInt(scoreFromDistance)));
						}

						PlayHitNoteSound();

						GetWorld()->GetTimerManager().SetTimer(enemyIncreaseScoreHandler, this,
							&URhythmMinigameMobileWidget::DetermineEnemyScore, timeToUpdateEnemyScore, false);

						imagesForD[i]->RemoveFromParent();
						imagesForD[i] = nullptr;

						imagesForD.RemoveAt(i);
					}

					// If the falling images for D is at the D text or slightly below it, increment the score
					else if (distance >= maxDistanceToTexts && distance < maxDistanceToTexts + distanceOffset)
					{
						const float& actualDistance = (((maxDistanceToTexts + distanceOffset) - distance) / 100.0f);

						int scoreFromDistance = static_cast<int>(actualDistance * maxScoreToIncrement);

						// If it reaches within the distance offset for perfect score zone, award the max score
						if (distance < maxDistanceToTexts + distanceOffsetForPerfectScore)
						{
							scoreFromDistance = maxScoreToIncrement;
						}

						gameMode->IncrementPlayerScore(scoreFromDistance);

						if (PlayerScoreText)
						{
							PlayerScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetPlayerScore())));
							//PlayerScoreText->SetText(FText::FromString(FString::FromInt(scoreFromDistance)));
						}

						PlayHitNoteSound();

						GetWorld()->GetTimerManager().SetTimer(enemyIncreaseScoreHandler, this,
							&URhythmMinigameMobileWidget::DetermineEnemyScore, timeToUpdateEnemyScore, false);

						imagesForD[i]->RemoveFromParent();
						imagesForD[i] = nullptr;

						imagesForD.RemoveAt(i);
					}
				}

				//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), distance);
			}
		}
	}
}

void URhythmMinigameMobileWidget::PressOnFallingImageF()
{
	for (int i = 0; i < imagesForF.Num(); ++i)
	{
		if (UOverlaySlot* overlayImageFSlot = Cast<UOverlaySlot>(imagesForF[i]->Slot))
		{
			if (UOverlaySlot* overlayTextFSlot = Cast<UOverlaySlot>(FButton->Slot))
			{
				const float& distance = FMath::Abs(overlayTextFSlot->GetPadding().Top - overlayImageFSlot->GetPadding().Top);

				if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					// If the falling images for F is slightly above or at the F text, increment the score
					if (distance > maxDistanceToTexts - distanceOffset && distance <= maxDistanceToTexts)
					{
						const float& actualDistance = ((distance - (maxDistanceToTexts - distanceOffset)) / 100.0f);

						int scoreFromDistance = static_cast<int>(actualDistance * maxScoreToIncrement);

						// If it reaches within the distance offset for perfect score zone, award the max score
						if (distance > maxDistanceToTexts - distanceOffsetForPerfectScore)
						{
							scoreFromDistance = maxScoreToIncrement;
						}

						gameMode->IncrementPlayerScore(scoreFromDistance);

						if (PlayerScoreText)
						{
							PlayerScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetPlayerScore())));
							//PlayerScoreText->SetText(FText::FromString(FString::FromInt(scoreFromDistance)));
						}

						PlayHitNoteSound();

						GetWorld()->GetTimerManager().SetTimer(enemyIncreaseScoreHandler, this,
							&URhythmMinigameMobileWidget::DetermineEnemyScore, timeToUpdateEnemyScore, false);

						imagesForF[i]->RemoveFromParent();
						imagesForF[i] = nullptr;

						imagesForF.RemoveAt(i);
					}

					// If the falling images for F is at the F text or slightly below it, increment the score
					else if (distance >= maxDistanceToTexts && distance < maxDistanceToTexts + distanceOffset)
					{
						const float& actualDistance = (((maxDistanceToTexts + distanceOffset) - distance) / 100.0f);

						int scoreFromDistance = static_cast<int>(actualDistance * maxScoreToIncrement);

						// If it reaches within the distance offset for perfect score zone, award the max score
						if (distance < maxDistanceToTexts + distanceOffsetForPerfectScore)
						{
							scoreFromDistance = maxScoreToIncrement;
						}

						gameMode->IncrementPlayerScore(scoreFromDistance);

						if (PlayerScoreText)
						{
							PlayerScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetPlayerScore())));
							//PlayerScoreText->SetText(FText::FromString(FString::FromInt(scoreFromDistance)));
						}

						PlayHitNoteSound();

						GetWorld()->GetTimerManager().SetTimer(enemyIncreaseScoreHandler, this,
							&URhythmMinigameMobileWidget::DetermineEnemyScore, timeToUpdateEnemyScore, false);

						imagesForF[i]->RemoveFromParent();
						imagesForF[i] = nullptr;

						imagesForF.RemoveAt(i);
					}
				}

				//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), distance);
			}
		}
	}
}

void URhythmMinigameMobileWidget::PauseGame()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->PushWidget(gameMode->GetPauseMenuClass());
		}
	}
}

void URhythmMinigameMobileWidget::DetermineEnemyScore()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		const int& scoreFromDistance = FMath::RandRange(0, maxScoreToIncrement);
		gameMode->IncrementEnemyScore(scoreFromDistance);

		if (EnemyScoreText)
		{
			EnemyScoreText->SetText(FText::FromString(FString::FromInt(gameMode->GetEnemyScore())));
			//EnemyScoreText->SetText(FText::FromString(FString::FromInt(scoreFromDistance)));
		}
	}
}

void URhythmMinigameMobileWidget::StartDancing()
{
	// Make the player dance
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		player->PlayDanceAnimation();
	}

	// Make the enemy dance too
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->MakeEnemyDance();
	}
}

void URhythmMinigameMobileWidget::PlayMissNoteSound()
{
	if (noteSounds.IsEmpty()) return;

	// Play miss note sound
	if (noteSounds[0])
	{
		UGameplayStatics::PlaySound2D(GetWorld(), noteSounds[0], currentSFXVolume);
	}
}

void URhythmMinigameMobileWidget::PlayHitNoteSound()
{
	if (noteSounds.IsEmpty() || noteSounds.Num() < 2) return;

	// Play hit note sound
	if (noteSounds[1])
	{
		UGameplayStatics::PlaySound2D(GetWorld(), noteSounds[1], currentSFXVolume);
	}
}

void URhythmMinigameMobileWidget::PlayPlayerMissedNoteDance()
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		player->PlayMissedNoteDanceAnimation();
	}
}

void URhythmMinigameMobileWidget::EndMinigame()
{
	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		gameMode->PopWidget(gameMode->GetRhythmMinigameMobileClass());
		gameMode->PushWidget(gameMode->GetFinalScoreClass());

		gameMode->StopEnemyFromDancing();

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->StopAllAnimations();

			// Disable the option for the player to pause the game
			player->SetPlayerActionToFinalScore();
		}
	}
}
