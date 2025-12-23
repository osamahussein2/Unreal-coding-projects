// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/SoccerBall.h"
#include "Goalie/Goalkeeper.h"

APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Modify capsule component parameters
	GetCapsuleComponent()->SetEnableGravity(true);

	// Modify character movement component parameters
	GetCharacterMovement()->GravityScale = 1.0f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	seconds -= DeltaTime; // Decrement seconds by delta time

	// If the seconds reaches 0 with at least a minute to spare, decrease the minutes counter and reset seconds countdown
	if (seconds <= 0.0f && minutes > 0)
	{
		--minutes;
		seconds = 60.0f;
	}

	// Show the game over screen after timer expired
	else if (seconds <= 0.0f && minutes <= 0)
	{
		EnableMouseCursor();
		HidePlayerHUD();

		// Set the player's score and player's currency only if it's NOT 0
		if (playerInformations[currentGameIndex].playerScore != 0)
		{
			SetPlayerScore(playerInformations[currentGameIndex].playerScore);
			
			if (static_cast<int&>(lastEXP) != currentEXP) static_cast<int&>(lastEXP) = currentEXP;
			if (static_cast<int&>(lastLevel) != currentLevel) static_cast<int&>(lastLevel) = currentLevel;
			if (static_cast<int&>(lastUpgradePoint) != upgradePoint) static_cast<int&>(lastUpgradePoint) = upgradePoint;
		}

		// Otherwise current player score is 0, just remove it at the current index
		else if (playerInformations.Num() <= maximumScoreIndex && playerInformations[currentGameIndex].playerScore <= 0)
		{
			if (playerInformations.IsValidIndex(currentGameIndex)) playerInformations.RemoveAt(currentGameIndex);
			if (currentGameIndex > 0) --currentGameIndex; // Also decrease current game index by 1 if necessary
		}

		ShowGameOver();
	}

	// Update timer text in HUD
	if (playerHUD) playerHUD->SetTimeText(minutes, seconds);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the local player enhanced input subsystem
	UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(GetController())->GetLocalPlayer());

	// Add the input mapping context
	enhancedInputSubsystem->AddMappingContext(inputMapping, 0);

	// Get the EnhancedInputComponent
	UEnhancedInputComponent* playerEnhancedInputcomponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerDataAsset && PlayerDataAsset->IA_Pause)
	{
		playerEnhancedInputcomponent->BindAction(PlayerDataAsset->IA_Pause, ETriggerEvent::Triggered, this,
			&APlayerCharacter::PauseGame);
	}

	if (PlayerDataAsset && PlayerDataAsset->IA_Shoot)
	{
		playerEnhancedInputcomponent->BindAction(PlayerDataAsset->IA_Shoot, ETriggerEvent::Triggered, this,
			&APlayerCharacter::ShootBall);
	}
}

void APlayerCharacter::PauseGame()
{
	EnableMouseCursor();
	HidePlayerHUD();
	ShowPauseMenu();
}

void APlayerCharacter::ShootBall()
{
	// Check if soccer ball is valid and also check if the player has shot the ball yet
	if (soccerBall && !soccerBall->GetPlayerShotBall())
	{
		// Start shooting if a soccer ball is found in the level
		soccerBall->SetPlayerShotBall(true);
		soccerBall->SetShotIndicatorLocation();
		soccerBall->DestroyShotIndicator();
	}
}

int APlayerCharacter::GetPlayerScores(int& index_) const
{
	return playerInformations[index_].playerScore;
}

FText APlayerCharacter::GetPlayerNames(int& index_) const
{
	return playerInformations[index_].playerName;
}

bool APlayerCharacter::GetNamesReceived(int& index_) const
{
	return !playerInformations[index_].playerName.IsEmpty();
}

void APlayerCharacter::SetPlayerName(FText playerName_, int& index_)
{
	playerInformations[index_].playerName = playerName_;
}

void APlayerCharacter::SetPlayerScore(int& playerScore_)
{
	// Check if we're still below or equal to the maximum score index to update the current game index score
	if (playerInformations.Num() <= maximumScoreIndex)
	{
		playerInformations[currentGameIndex].playerScore = playerScore_;
	}

	// Change the player score too if we went past the maximum score index and the player score is greater than the lowest score
	else if (playerInformations.Num() > maximumScoreIndex && playerScore_ > playerInformations.Last().playerScore)
	{
		playerInformations[currentGameIndex].playerName = FText::FromString("");
		playerInformations[currentGameIndex].playerScore = playerScore_;
	}

	// Otherwise we went past the maximum score index but the player score is less than or equal to the lowest score
	else if (playerInformations.Num() > maximumScoreIndex && playerScore_ <= playerInformations.Last().playerScore)
	{
		playerInformations[currentGameIndex].playerScore += 0; // Don't increase the player's score
	}

	// Call sort player information array to start sorting from highest score to lowest score
	playerInformations.Sort();
}

void APlayerCharacter::IncrementPlayerScore()
{
	if (!playerInformations.IsEmpty())
	{
		// Increase player's score by 1
		playerInformations[currentGameIndex].playerScore += 1;

		// Increase current EXP only if the player's level isn't at the hardcoded maximum value yet
		if (currentLevel < 9) currentEXP += 1 * EXPMultiplier;

		// Make sure the player can level up after current EXP exceeds max EXP and that the player hasn't reached max level yet
		if (currentEXP >= maxEXP && currentLevel < 9)
		{
			// Give the player the correct current EXP amount when leveling up
			EXPToGiveBeforeLevelingUp = currentEXP - maxEXP;
			static_cast<int&>(currentEXP) = EXPToGiveBeforeLevelingUp;

			++currentLevel; // Progress to the next level
			++upgradePoint; // Also give the player an upgrade point to be able to upgrade abilities

			ChangeEXPValuesDependingOnCurrentLevel(); // Make sure that maxEXP values are changed after leveling up
		}

		// Update player's score HUD
		if (playerHUD)
		{
			playerHUD->SetScoreText(playerInformations[currentGameIndex].playerScore);
			playerHUD->SetPlayerCurrentLevelText(currentLevel);
			playerHUD->SetProgressBarEXP((float&)currentEXP, (float&)maxEXP);
			if (currentLevel < 9) playerHUD->SetProgressToNextLevelText(currentEXP, maxEXP);
		}
	}
}

int APlayerCharacter::GetPlayerLeaderboardStatsSize() const
{
	return playerInformations.Num();
}

/*void APlayerCharacter::SortPlayerScoresArray()
{
	// Sort in descending order
	Algo::Sort(playerScores, [this](const int& A, const int& B)
		{
			return A > B; // Return true if A should come before B (for descending)
		});
}*/

void APlayerCharacter::AddPlayerInformation()
{
	// Add player scores to the final element in the array if player scores array is less than or equal to maximum score index value
	if (playerInformations.Num() <= maximumScoreIndex && !playerInformations.IsEmpty())
	{
		playerInformations.Emplace(FText::FromString(""), 0);

		++currentGameIndex; // Increase current game index by 1
	}

	else if (playerInformations.IsEmpty())
	{
		playerInformations.Emplace(FText::FromString(""), 0);
	}
}

void APlayerCharacter::DeleteCurrentPlayerInformation()
{
	if (!playerInformations.IsEmpty() && playerInformations.Num() <= maximumScoreIndex)
	{
		// Delete player's current information
		if (playerInformations.IsValidIndex(currentGameIndex)) playerInformations.RemoveAt(currentGameIndex);
		if (currentGameIndex > 0) --currentGameIndex; // Decrease current game index by 1
	}
}

void APlayerCharacter::ResetCountingEXPStats()
{
	// Reset player's EXP and upgrade point back to what it was before starting the game if necessary
	if (static_cast<int&>(currentEXP) != lastEXP) static_cast<int&>(currentEXP) = lastEXP;
	if (static_cast<int&>(currentLevel) != lastLevel) static_cast<int&>(currentLevel) = lastLevel;
	if (static_cast<int&>(upgradePoint) != lastUpgradePoint) static_cast<int&>(upgradePoint) = lastUpgradePoint;
}

void APlayerCharacter::DeleteAllLeaderboardInformation()
{
	if (!playerInformations.IsEmpty()) playerInformations.Empty();

	currentGameIndex = 0;
}

void APlayerCharacter::SetPlayerViewTarget()
{
	// Find a camera actor placed in the level
	TArray<AActor*> FoundCameraActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACineCameraActor::StaticClass(), FoundCameraActor);

	cineCameraActor = Cast<ACineCameraActor>(FoundCameraActor[0]);

	if (cineCameraActor)
	{
		// Set player's view target to the camera found inside the level
		Cast<APlayerController>(GetController())->SetViewTargetWithBlend(cineCameraActor);

		FoundCameraActor[0] = nullptr;
		cineCameraActor = nullptr;
	}
}

void APlayerCharacter::EnableMouseCursor()
{
	// Exclusive to PC
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false); // Don't hide the cursor during capture

	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Don't worry about mouse lock

	if (Cast<APlayerController>(GetController()))
	{
		Cast<APlayerController>(GetController())->SetInputMode(InputMode);

		if (Cast<APlayerController>(GetController())->bShowMouseCursor != true) 
			Cast<APlayerController>(GetController())->bShowMouseCursor = true;
	}
}

void APlayerCharacter::DisableMouseCursor()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

	if (Cast<APlayerController>(GetController()))
	{
		Cast<APlayerController>(GetController())->SetInputMode(GameInputMode);

		// Make sure the mouse cursor is turned off when resuming the game
		if (Cast<APlayerController>(GetController())->bShowMouseCursor != false) 
			Cast<APlayerController>(GetController())->bShowMouseCursor = false;
	}
}

void APlayerCharacter::ResetGame()
{
	// Reset timers
	if (minutes != 1) minutes = 1;
	if (seconds != 1.0f) seconds = 1.0f;

	// Reset soccer ball as well if it's valid
	if (soccerBall) soccerBall->ResetSoccerBall();

	// Find a goalkeeper in the level
	TArray<AActor*> foundGoalkeeper;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoalkeeper::StaticClass(), foundGoalkeeper);

	// Reset goalkeeper values if necessary
	if (Cast<AGoalkeeper>(foundGoalkeeper[0]))
	{
		Cast<AGoalkeeper>(foundGoalkeeper[0])->ResetGoalkeeper();
		foundGoalkeeper[0] = nullptr;
	}

	// Reset player's EXP stats and upgrade points if necessary
	if (static_cast<int&>(lastEXP) != currentEXP) static_cast<int&>(lastEXP) = currentEXP;
	if (static_cast<int&>(lastLevel) != currentLevel) static_cast<int&>(lastLevel) = currentLevel;
	if (static_cast<int&>(lastUpgradePoint) != upgradePoint) static_cast<int&>(lastUpgradePoint) = upgradePoint;

	// Also change EXP values based on current level as well if needed
	ChangeEXPValuesDependingOnCurrentLevel();
}

void APlayerCharacter::FindSoccerBall()
{
	TArray<AActor*> foundSoccerBall;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoccerBall::StaticClass(), foundSoccerBall);

	if (!soccerBall) soccerBall = Cast<ASoccerBall>(foundSoccerBall[0]);

	foundSoccerBall[0] = nullptr;
}

void APlayerCharacter::InvalidateSoccerBall()
{
	if (soccerBall)
	{
		soccerBall->DestroyShotIndicator();
		soccerBall = nullptr;
	}
}

void APlayerCharacter::DecreaseUpgradePoint(int value_)
{
	upgradePoint -= value_;
}

void APlayerCharacter::UpgradeBallSpeed()
{
	++ballSpeedUpgradeIndex;

	// Increase the ball speed depending on current ball speed upgrade index
	switch (ballSpeedUpgradeIndex)
	{
	case 2:
		if (soccerBall) soccerBall->IncreaseBallSpeed(5.0f);
		break;

	case 3:
		if (soccerBall) soccerBall->IncreaseBallSpeed(10.0f);
		break;

	case 4:
		if (soccerBall) soccerBall->IncreaseBallSpeed(15.0f);
		break;

	default:
		break;
	}
}

void APlayerCharacter::UpgradeGoalkeeperSpeed()
{
	++goalkeeperSpeedUpgradeIndex;

	TArray<AActor*> foundGoalkeeper;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoalkeeper::StaticClass(), foundGoalkeeper);

	// Decrease goalkeeper speed depending on current ball speed upgrade index
	switch (goalkeeperSpeedUpgradeIndex)
	{
	case 2:
		if (Cast<AGoalkeeper>(foundGoalkeeper[0])) Cast<AGoalkeeper>(foundGoalkeeper[0])->DecreaseGoalkeeperSpeed(0.1f);
		if (foundGoalkeeper[0]) foundGoalkeeper[0] = nullptr;
		break;

	case 3:
		if (Cast<AGoalkeeper>(foundGoalkeeper[0])) Cast<AGoalkeeper>(foundGoalkeeper[0])->DecreaseGoalkeeperSpeed(0.1f);
		if (foundGoalkeeper[0]) foundGoalkeeper[0] = nullptr;
		break;

	case 4:
		if (Cast<AGoalkeeper>(foundGoalkeeper[0])) Cast<AGoalkeeper>(foundGoalkeeper[0])->DecreaseGoalkeeperSpeed(0.2f);
		if (foundGoalkeeper[0]) foundGoalkeeper[0] = nullptr;
		break;

	default:
		if (foundGoalkeeper[0]) foundGoalkeeper[0] = nullptr;
		break;
	}
}

void APlayerCharacter::UpgradeEXPMultiplier()
{
	++EXPMultiplierUpgradeIndex;

	// Increase EXP multiplier depending on EXP multiplier upgrade index
	switch (EXPMultiplierUpgradeIndex)
	{
	case 2:
		EXPMultiplier = 2;
		break;

	case 3:
		EXPMultiplier = 3;
		break;

	default:
		break;
	}
}

void APlayerCharacter::IncrementCurrentDifficultyIndex()
{
	++currentDifficultyIndex;
}

void APlayerCharacter::DecrementCurrentDifficultyIndex()
{
	--currentDifficultyIndex;
}

void APlayerCharacter::UpdateVariablesBasedOnCurrentDifficulty()
{
	switch (currentDifficultyIndex)
	{
	case 0: // Easy
		if (animateLeftRotation != FRotator(0.0f, -90.0f, 0.0f)) animateLeftRotation = FRotator(0.0f, -90.0f, 0.0f);
		if (animateRightRotation != FRotator(0.0f, 90.0f, 0.0f)) animateRightRotation = FRotator(0.0f, 90.0f, 0.0f);

		if (timeToFlipRotation != 0.5f) timeToFlipRotation = 0.5f;
		break;

	case 1: // Normal
		if (animateLeftRotation != FRotator(0.0f, -152.5f, 0.0f)) animateLeftRotation = FRotator(0.0f, -152.5f, 0.0f);
		if (animateRightRotation != FRotator(0.0f, 152.5f, 0.0f)) animateRightRotation = FRotator(0.0f, 152.5f, 0.0f);

		if (timeToFlipRotation != 0.4f) timeToFlipRotation = 0.4f;
		break;

	case 2: // Hard
		if (animateLeftRotation != FRotator(0.0f, -215.0f, 0.0f)) animateLeftRotation = FRotator(0.0f, -215.0f, 0.0f);
		if (animateRightRotation != FRotator(0.0f, 215.0f, 0.0f)) animateRightRotation = FRotator(0.0f, 215.0f, 0.0f);

		if (timeToFlipRotation != 0.3f) timeToFlipRotation = 0.3f;
		break;

	default:
		break;
	}
}

void APlayerCharacter::ChangeEXPValuesDependingOnCurrentLevel()
{
	switch (currentLevel)
	{
	case 1:
		if (maxEXP != 5) maxEXP = 5;
		break;

	case 2:
		if (maxEXP != 10) maxEXP = 10;
		break;

	case 3:
		if (maxEXP != 15) maxEXP = 15;
		break;

	case 4:
		if (maxEXP != 20) maxEXP = 20;
		break;

	case 5:
		if (maxEXP != 30) maxEXP = 30;
		break;

	case 6:
		if (maxEXP != 40) maxEXP = 40;
		break;

	case 7:
		if (maxEXP != 50) maxEXP = 50;
		break;

	case 8:
		if (maxEXP != 75) maxEXP = 75;
		break;

	case 9:
		/* Let's ensure when the player reached max level, player's current and max EXP's are maxed out to give the illusion
		that the player has all the upgrade points possible to upgrade all abilities */
		if (maxEXP != 1) maxEXP = 1;
		if (currentEXP != 1) currentEXP = 1;

		if (playerHUD) playerHUD->DestroyEXPRelatedUI(); // Destroy any UI related to player's EXP and level
		break;

	default:
		break;
	}
}

// Player widgets functions
void APlayerCharacter::ShowMainMenu(bool worryAboutMouseCursor)
{
	if (worryAboutMouseCursor) EnableMouseCursor();

	if (mainMenuClass)
	{
		UMainMenuWidget* mainMenuWidget = CreateWidget<UMainMenuWidget>(Cast<APlayerController>(GetController()), mainMenuClass);

		if (mainMenuWidget)
		{
			mainMenuWidget->AddToViewport();
			mainMenuWidget = nullptr;
		}
	}
}

void APlayerCharacter::ShowLeaderboardMenu()
{
	if (leaderboardClass)
	{
		ULeaderboardWidget* mainMenuWidget = CreateWidget<ULeaderboardWidget>(Cast<APlayerController>(GetController()), leaderboardClass);

		if (mainMenuWidget)
		{
			mainMenuWidget->AddToViewport();
			mainMenuWidget = nullptr;
		}
	}
}

void APlayerCharacter::ShowPauseMenu()
{
	if (pauseMenuClass)
	{
		UPauseMenuWidget* pauseMenuWidget = CreateWidget<UPauseMenuWidget>(Cast<APlayerController>(GetController()), pauseMenuClass);

		if (pauseMenuWidget)
		{
			pauseMenuWidget->AddToViewport();
			pauseMenuWidget = nullptr;
		}
	}
}

void APlayerCharacter::ShowPlayerHUD()
{
	if (playerHUDClass)
	{
		playerHUD = CreateWidget<UPlayerHUDWidget>(Cast<APlayerController>(GetController()), playerHUDClass);

		if (playerHUD)
		{
			playerHUD->AddToViewport();
			playerHUD->SetTimeText(minutes, seconds);
			playerHUD->SetScoreText(playerInformations[currentGameIndex].playerScore);
			playerHUD->SetPlayerCurrentLevelText(currentLevel);
			playerHUD->SetProgressBarEXP((float&)currentEXP, (float&)maxEXP);
			playerHUD->SetProgressToNextLevelText(currentEXP, maxEXP);

			// If player reaches the hardcoded max level, don't show progress to next level text
			if (currentLevel >= 9) playerHUD->DestroyEXPRelatedUI();
		}
	}
}

void APlayerCharacter::HidePlayerHUD()
{
	if (playerHUD)
	{
		playerHUD->RemoveFromParent();
		playerHUD = nullptr;
	}
}

void APlayerCharacter::ShowGameOver()
{
	if (gameOverClass)
	{
		UGameOverWidget* gameOverWidget = CreateWidget<UGameOverWidget>(Cast<APlayerController>(GetController()), gameOverClass);

		if (gameOverWidget)
		{
			gameOverWidget->AddToViewport();
			gameOverWidget = nullptr;
		}
	}
}

void APlayerCharacter::ShowUpgradeMenu()
{
	if (upgradeMenuClass)
	{
		UUpgradeMenuWidget* upgradeMenuWidget = CreateWidget<UUpgradeMenuWidget>(Cast<APlayerController>(GetController()), upgradeMenuClass);

		if (upgradeMenuWidget)
		{
			upgradeMenuWidget->AddToViewport();
			upgradeMenuWidget = nullptr;
		}
	}
}

void APlayerCharacter::ShowOptionsMenu()
{
	if (optionsMenuClass)
	{
		UOptionsMenuWidget* optionsMenuWidget = CreateWidget<UOptionsMenuWidget>(Cast<APlayerController>(GetController()), optionsMenuClass);

		if (optionsMenuWidget)
		{
			optionsMenuWidget->AddToViewport();
			optionsMenuWidget = nullptr;
		}
	}
}

void APlayerCharacter::ShowCreditsMenu()
{
	if (creditsMenuClass)
	{
		UCreditsMenuWidget* creditsMenuWidget = CreateWidget<UCreditsMenuWidget>(Cast<APlayerController>(GetController()), creditsMenuClass);

		if (creditsMenuWidget)
		{
			creditsMenuWidget->AddToViewport();
			creditsMenuWidget = nullptr;
		}
	}
}
