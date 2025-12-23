// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameOverWidget.h"
#include "Player/PlayerCharacter.h"

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), alpha(1.0f),
gameOverTimer(0.0f)
{
}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Just in case, the game isn't paused, pause the game using UGameplayStatics
	if (!UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UGameOverWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Make the game over text fade overtime
	if (alpha >= 0.0f)
	{
		alpha -= alphaFadeSpeed * InDeltaTime;
		if (GameOverText) GameOverText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, alpha));
	}

	else
	{
		gameOverTimer += InDeltaTime; // Increase the game over timer after the text completely faded

		// Once game over timer exceeds its max timer, take the player to the upgrade menu
		if (gameOverTimer >= maxGameOverTimer)
		{
			TArray<AActor*> foundPlayer;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

			TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

			foundPlayer[0] = nullptr;

			if (playerCharacter.IsValid())
			{
				RemoveFromParent();

				playerCharacter->ShowUpgradeMenu();
				playerCharacter = nullptr;
			}
		}
	}
}
