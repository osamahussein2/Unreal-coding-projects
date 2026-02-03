// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CurrentDayWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Player/PlayerCharacter.h"

UCurrentDayWidget::UCurrentDayWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCurrentDayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Just in case, the game isn't paused, pause the game using UGameplayStatics
	if (!UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (CurrentDayText) CurrentDayText->SetText(FText::FromString(TEXT("Day ") + FString::FromInt(gameMode->GetCurrentDay())));
	}
}

void UCurrentDayWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	// Make the current day text pop up more overtime
	if (!alphaStateChanged)
	{
		alpha += alphaFadeSpeed * DeltaTime;
		if (CurrentDayText) CurrentDayText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, alpha));

		if (alpha >= 1.0f) alphaStateChanged = true;
	}

	else
	{
		currentDayTimer += DeltaTime; // Increase the game over timer after the text completely faded

		// Once game over timer exceeds its max timer, make the current day text fade out overtime
		if (currentDayTimer >= maxCurrentDayTimer)
		{
			alpha -= alphaFadeSpeed * DeltaTime;
			if (CurrentDayText) CurrentDayText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, alpha));

			if (alpha <= 0.0f)
			{
				// Destroy current day widget afterwards
				if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					// Unpause the game using UGameplayStatics
					UGameplayStatics::SetGamePaused(GetWorld(), false);

					if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
					{
						if (APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
						{
							// Enable player input again
							player->EnableInput(playerController);
						}
					}

					gameMode->InitializeMobileWidget();
					gameMode->DestroyCurrentDayWidget();
				}
			}
		}
	}
}

void UCurrentDayWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (CurrentDayText)
	{
		CurrentDayText->RemoveFromParent();
		CurrentDayText = nullptr;
	}

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (CurrentDayCanvas)
	{
		CurrentDayCanvas->RemoveFromParent();
		CurrentDayCanvas = nullptr;
	}

	alpha = 0.0f;
	currentDayTimer = 0.0f;

	alphaStateChanged = false;
}
