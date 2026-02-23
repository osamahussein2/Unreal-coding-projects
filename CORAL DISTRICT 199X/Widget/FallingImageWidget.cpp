// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/FallingImageWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/BrackeysGM.h"

UFallingImageWidget::UFallingImageWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UFallingImageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (fallingImage && fallingImage->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		fallingImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		currentIndex = gameMode->GetRewardNumber();
	}
}

void UFallingImageWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	// Only update the falling image if the game isn't paused
	if (!UGameplayStatics::IsGamePaused(GetWorld()) && fallSpeeds.Num() >= currentIndex + 1 && fallSpeeds[currentIndex])
	{
		Super::NativeTick(Geometry, DeltaTime);

		fallingImagePositionY = fallSpeeds[currentIndex] * DeltaTime;
	}
}

void UFallingImageWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (fallingImage)
	{
		fallingImage->RemoveFromParent();
		fallingImage = nullptr;
	}
}