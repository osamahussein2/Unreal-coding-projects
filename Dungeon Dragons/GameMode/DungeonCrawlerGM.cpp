// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/DungeonCrawlerGM.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Player/PlayerCharacter.h"

ADungeonCrawlerGM::ADungeonCrawlerGM()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
}

void ADungeonCrawlerGM::BeginPlay()
{
	Super::BeginPlay();

	if (!playerCharacterClass) return;

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	APawn* playerCharacter = GetWorld()->SpawnActor<APawn>(playerCharacterClass, FVector::ZeroVector,
		FRotator::ZeroRotator, FActorSpawnParameters());

	if (!playerCharacter) return;

	playerController->Possess(playerCharacter);
	playerCharacter->EnableInput(playerController);

	EnableMouseCursor();
	PushWidget(mainMenuClass);

	if (gameMusic) UGameplayStatics::PlaySound2D(GetWorld(), gameMusic);
}

void ADungeonCrawlerGM::PushWidget(TSubclassOf<UUserWidget> widgetClass_)
{
	// Only if the widget class if valid
	if (widgetClass_)
	{
		// Add whatever widget subclass is passed in into the cached widgets array
		cachedWidgets.Add(CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), widgetClass_));

		// Set the last index to be the last added element in the cached widgets array
		const int& lastIndex = cachedWidgets.Num() - 1;

		// If the cached widget at the last pushed index is valid, add it to viewport
		if (cachedWidgets[lastIndex]) cachedWidgets[lastIndex]->AddToViewport();

		// Otherwise, just remove it from the array as we don't want the array to hold an invalid widget
		else cachedWidgets.RemoveAt(lastIndex);
	}
}

void ADungeonCrawlerGM::PopWidget(TSubclassOf<UUserWidget> widgetClass_)
{
	for (int i = 0; i < cachedWidgets.Num(); ++i)
	{
		if (widgetClass_ && cachedWidgets[i]->GetClass() == widgetClass_)
		{
			cachedWidgets[i]->RemoveFromParent();
			cachedWidgets[i] = nullptr;

			cachedWidgets.RemoveAt(i);
		}

		else continue;
	}
}

bool ADungeonCrawlerGM::DoesWidgetContainPlayerHUD()
{
	for (int i = 0; i < cachedWidgets.Num(); ++i)
	{
		if (playerHUDClass && cachedWidgets[i]->GetClass() == playerHUDClass)
		{
			return true;
		}

		else continue;
	}

	return false;
}

void ADungeonCrawlerGM::UpdatePlayerHealth(const float& healthPercentage)
{
	for (int i = 0; i < cachedWidgets.Num(); ++i)
	{
		if (playerHUDClass && cachedWidgets[i]->GetClass() == playerHUDClass)
		{
			UPlayerHUDWidget* playerHUDWidget = Cast<UPlayerHUDWidget>(cachedWidgets[i]);

			playerHUDWidget->SetHealthBar(healthPercentage);
		}

		else continue;
	}
}

void ADungeonCrawlerGM::UpdatePlayerStats(const float& dexterity, const float& strength, const float& intelligence)
{
	for (int i = 0; i < cachedWidgets.Num(); ++i)
	{
		if (playerHUDClass && cachedWidgets[i]->GetClass() == playerHUDClass)
		{
			UPlayerHUDWidget* playerHUDWidget = Cast<UPlayerHUDWidget>(cachedWidgets[i]);

			playerHUDWidget->SetPlayerStats(dexterity, strength, intelligence);
		}

		else continue;
	}
}

void ADungeonCrawlerGM::SpawnUnlockDoorWidgetOnHUD()
{
	for (int i = 0; i < cachedWidgets.Num(); ++i)
	{
		if (playerHUDClass && cachedWidgets[i]->GetClass() == playerHUDClass)
		{
			UPlayerHUDWidget* playerHUDWidget = Cast<UPlayerHUDWidget>(cachedWidgets[i]);

			playerHUDWidget->SpawnUnlockDoorWidget();
		}

		else continue;
	}
}

void ADungeonCrawlerGM::DestroyUnlockDoorWidgetOnHUD()
{
	for (int i = 0; i < cachedWidgets.Num(); ++i)
	{
		if (playerHUDClass && cachedWidgets[i]->GetClass() == playerHUDClass)
		{
			UPlayerHUDWidget* playerHUDWidget = Cast<UPlayerHUDWidget>(cachedWidgets[i]);

			playerHUDWidget->DestroyUnlockDoorWidget();
		}

		else continue;
	}
}

void ADungeonCrawlerGM::UpdateDiceRoll()
{
	for (int i = 0; i < cachedWidgets.Num(); ++i)
	{
		if (playerHUDClass && cachedWidgets[i]->GetClass() == playerHUDClass)
		{
			UPlayerHUDWidget* playerHUDWidget = Cast<UPlayerHUDWidget>(cachedWidgets[i]);

			playerHUDWidget->UpdateDieImage();
		}

		else continue;
	}
}

void ADungeonCrawlerGM::SpawnInventoryWidgetOnHUD()
{
	for (int i = 0; i < cachedWidgets.Num(); ++i)
	{
		if (playerHUDClass && cachedWidgets[i]->GetClass() == playerHUDClass)
		{
			UPlayerHUDWidget* playerHUDWidget = Cast<UPlayerHUDWidget>(cachedWidgets[i]);

			playerHUDWidget->PopInventoryUp();
		}

		else continue;
	}
}

void ADungeonCrawlerGM::EnableMouseCursor()
{
	// Exclusive to PC
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false); // Don't hide the cursor during capture
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetInputMode(InputMode);

	if (playerController->bShowMouseCursor != true) playerController->bShowMouseCursor = true;
}

void ADungeonCrawlerGM::DisableMouseCursor()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetInputMode(GameInputMode);

	// Make sure the mouse cursor is turned off
	if (playerController->bShowMouseCursor != false) playerController->bShowMouseCursor = false;
}

TSubclassOf<UMainMenuWidget> ADungeonCrawlerGM::GetMainMenuClass() const
{
	return mainMenuClass;
}

TSubclassOf<UPlayerHUDWidget> ADungeonCrawlerGM::GetPlayerHUDClass() const
{
	return playerHUDClass;
}

TSubclassOf<UPauseMenuWidget> ADungeonCrawlerGM::GetPauseMenuClass() const
{
	return pauseMenuClass;
}

TSubclassOf<UMapWidget> ADungeonCrawlerGM::GetMapClass() const
{
	return mapClass;
}

TSubclassOf<UGameOverWidget> ADungeonCrawlerGM::GetGameOverClass() const
{
	return gameOverClass;
}

TSubclassOf<UInventoryWidget> ADungeonCrawlerGM::GetInventoryClass() const
{
	return inventoryClass;
}

TSubclassOf<ULevelCompletionWidget> ADungeonCrawlerGM::GetLevelCompletionClass() const
{
	return levelCompletionClass;
}

void ADungeonCrawlerGM::LoadPlayingLevel()
{
	// Stream the game level
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnGameLevelLoaded");

	UGameplayStatics::LoadStreamLevel(GetWorld(), levelName, true, true, LatentInfo);
}

void ADungeonCrawlerGM::UnloadPlayingLevel()
{
	UGameplayStatics::UnloadStreamLevel(GetWorld(), levelName, FLatentActionInfo(), true);
}

void ADungeonCrawlerGM::OnGameLevelLoaded()
{
	AActor* PlayerStart = nullptr;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() && It->GetLevel()->bIsVisible)
		{
			PlayerStart = *It;
			break;
		}
	}

	if (!PlayerStart) // Don't try to spawn player character
	{
#ifdef UE_EDITOR
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
			TEXT("No valid PlayerStart found in visible levels."));
#endif

		return;
	}

	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		player->SetActorLocation(PlayerStart->GetActorLocation());
		player->SetActorRotation(PlayerStart->GetActorRotation());
		if (player->GetController()) player->GetController()->SetControlRotation(PlayerStart->GetActorRotation());
	}

	PlayerStart->Destroy();
}