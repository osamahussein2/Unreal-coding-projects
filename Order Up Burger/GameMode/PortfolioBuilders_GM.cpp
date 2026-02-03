// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/PortfolioBuilders_GM.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "Objects/Chair.h"
#include "People/Customer.h"
#include "Widgets/InstructionWidget.h"
#include "Widgets/PickFoodWidget.h"
#include "Widgets/CustomerScoreWidget.h"
#include "Widgets/CurrentDayWidget.h"
#include "Widgets/CurrentDayStatsWidget.h"
#include "Widgets/PauseMenuWidget.h"
#include "Widgets/PickFoodMobileWidget.h"
#include "Widgets/MainMenuWidget.h"
#include "Objects/Topping.h"
#include "Objects/Burger.h"
#include "Objects/Bun.h"
#include "Objects/TopBun.h"
#include "GameFramework/TouchInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

APortfolioBuilders_GM::APortfolioBuilders_GM()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultPawnClass = nullptr;
}

void APortfolioBuilders_GM::BeginPlay()
{
	if (!PlayerCharacterClass) return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return;

	AActor* PlayerStart = nullptr;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->GetLevel() != nullptr && It->GetLevel()->bIsVisible)
		{
			PlayerStart = *It;
			break;
		}
	}

	if (!PlayerStart)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnPlayer: No valid PlayerStart found in visible levels."));
		return;
	}

	FVector SpawnLocation = PlayerStart->GetActorLocation();
	FRotator SpawnRotation = PlayerStart->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACharacter* CachedPlayerCharacter = GetWorld()->SpawnActor<ACharacter>(PlayerCharacterClass, SpawnLocation, SpawnRotation, 
		SpawnParams);

	if (!CachedPlayerCharacter)	return;

	PlayerController->Possess(CachedPlayerCharacter);
	CachedPlayerCharacter->EnableInput(PlayerController);

	if (PlayerStart)
	{
		PlayerStart->Destroy();
		PlayerStart = nullptr;
	}

	// Find objects with the chair class
	TArray<AActor*> chairActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChair::StaticClass(), chairActors);

	for (AActor* chair : chairActors) chairs.Add(Cast<AChair>(chair));
	
	InitializeMainMenuWidget();
}

void APortfolioBuilders_GM::SpawnCustomer()
{
	customerIndex = FMath::RandRange(0, CustomerClass.Num() - 1);
	spawnPointIndex = FMath::RandRange(0, chairs.Num() - 1);

	// If the chairs or customer class arrays are empty or their elements are invalid, stop execution
	if (chairs.IsEmpty() || !chairs[spawnPointIndex] || CustomerClass.IsEmpty() || !CustomerClass[customerIndex]) return;

	// Find all customers if needed to check if there is a customer on the chair
	TArray<AActor*> customers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomer::StaticClass(), customers);

	// Don't spawn any customers if the customers spawned amount reaches the chairs array amount
	if (customersSpawned >= chairs.Num()) return;

	for (AActor* customer : customers)
	{
		// If one of the customers of a class has already spawned, re-execute this function
		if (customer->GetClass() == CustomerClass[customerIndex])
		{
			SpawnCustomer();
			return;
		}

		// If there is already a customer on one of the spawn points, re-execute this function
		if (FVector::Distance(customer->GetActorLocation(), chairs[spawnPointIndex]->GetActorLocation()) <= 50.0f)
		{
			SpawnCustomer();
			return;
		}
	}

	FVector SpawnLocation = chairs[spawnPointIndex]->GetActorLocation();
	FRotator SpawnRotation = chairs[spawnPointIndex]->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* customer = GetWorld()->SpawnActor<AActor>(CustomerClass[customerIndex], SpawnLocation, SpawnRotation, SpawnParams);
	customer = nullptr;

	++customersSpawned;
}

void APortfolioBuilders_GM::AddCustomerInformation()
{
	customerNumbers.Add(Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->GetCustomerNumber());
	customerScores.Add(playerScore);
}

void APortfolioBuilders_GM::AddAverageScoreInformation()
{
	averageScores.Add(averageScore);

	//UE_LOG(LogTemp, Warning, TEXT("Average score: %i"), averageScore)
}

void APortfolioBuilders_GM::RestartGame()
{
	// Empty all customer information array before resetting game
	if (!customerNumbers.IsEmpty()) customerNumbers.Empty();
	if (!customerScores.IsEmpty()) customerScores.Empty();
	if (!averageScores.IsEmpty()) averageScores.Empty();

	currentDay = 0;
	customersSpawned = 0;
}

void APortfolioBuilders_GM::InitializeInstructionWidget(FString text_)
{
	if (!instructionWidget && instructionWidgetClass)
	{
		instructionWidget = CreateWidget<UInstructionWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), 
			instructionWidgetClass);

		if (!instructionWidget) return;

		instructionWidget->SetInstructionText(text_);
		instructionWidget->AddToViewport();
	}
}

void APortfolioBuilders_GM::DestroyInstructionWidget()
{
	if (instructionWidget)
	{
		instructionWidget->RemoveFromParent();
		instructionWidget = nullptr;
	}
}

void APortfolioBuilders_GM::InitializePickFoodWidget()
{
	if (!pickFoodWidget && pickFoodWidgetClass)
	{
		pickFoodWidget = CreateWidget<UPickFoodWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			pickFoodWidgetClass);

		if (!pickFoodWidget) return;

		pickFoodWidget->AddToViewport();
	}
}

void APortfolioBuilders_GM::DestroyPickFoodWidget()
{
	if (pickFoodWidget)
	{
		pickFoodWidget->RemoveFromParent();
		pickFoodWidget = nullptr;
	}
}

void APortfolioBuilders_GM::InitializeCustomerOrderWidget()
{
	if (!customerOrderWidget && customerOrderWidgetClass)
	{
		customerOrderWidget = CreateWidget<UCustomerOrderWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			customerOrderWidgetClass);

		if (!customerOrderWidget) return;

		customerOrderWidget->AddToViewport();
	}
}

void APortfolioBuilders_GM::HideCustomerOrderOverlay()
{
	if (customerOrderWidget) customerOrderWidget->HideOrderOverlay();
}

void APortfolioBuilders_GM::ShowCustomerOrderOverlay()
{
	if (customerOrderWidget) customerOrderWidget->ShowOrderOverlay();
}

void APortfolioBuilders_GM::DestroyCustomerOrderWidget()
{
	if (customerOrderWidget)
	{
		customerOrderWidget->RemoveFromParent();
		customerOrderWidget = nullptr;
	}
}

void APortfolioBuilders_GM::InitializeCustomerScoreWidget()
{
	if (!customerScoreWidget && customerScoreWidgetClass)
	{
		customerScoreWidget = CreateWidget<UCustomerScoreWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			customerScoreWidgetClass);

		if (!customerScoreWidget) return;

		InvalidateTouchInterface();

		customerScoreWidget->AddToViewport();
	}
}

void APortfolioBuilders_GM::DestroyCustomerScoreWidget()
{
	if (customerScoreWidget)
	{
		IncrementAverageScore(playerScore);
		playerScore = 0;

		SetTouchInterface();

		customerScoreWidget->RemoveFromParent();
		customerScoreWidget = nullptr;
	}
}

void APortfolioBuilders_GM::InitializeMainMenuWidget()
{
	if (!mainMenuWidget && mainMenuWidgetClass)
	{
		EnableMouseCursor();

		mainMenuWidget = CreateWidget<UMainMenuWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			mainMenuWidgetClass);

		if (!mainMenuWidget) return;

		mainMenuWidget->AddToViewport();
	}
}

void APortfolioBuilders_GM::DestroyMainMenuWidget()
{
	if (mainMenuWidget)
	{
		mainMenuWidget->RemoveFromParent();
		mainMenuWidget = nullptr;
	}
}

void APortfolioBuilders_GM::InitializeCurrentDayWidget(bool incrementDay_)
{
	if (!currentDayWidget && currentDayWidgetClass)
	{
		DisableMouseCursor();

		if (customersSpawned != 0) customersSpawned = 0;
		if (averageScore != 0) averageScore = 0;

		// Empty all customer information array before starting a day
		if (!customerNumbers.IsEmpty()) customerNumbers.Empty();
		if (!customerScores.IsEmpty()) customerScores.Empty();

		if (incrementDay_) ++currentDay;

		for (int i = 0; i < currentDay; ++i) SpawnCustomer();

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

			player->ResetValues();
		}

		currentDayWidget = CreateWidget<UCurrentDayWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			currentDayWidgetClass);

		if (!currentDayWidget) return;

		currentDayWidget->AddToViewport();
	}
}

void APortfolioBuilders_GM::DestroyCurrentDayWidget()
{
	if (currentDayWidget)
	{
		SetTouchInterface();

		currentDayWidget->RemoveFromParent();
		currentDayWidget = nullptr;
	}
}

void APortfolioBuilders_GM::InitializeCurrentDayStatsWidget()
{
	if (!currentDayStatsWidget && currentDayStatsWidgetClass)
	{
		InvalidateTouchInterface();
		EnableMouseCursor();

		currentDayStatsWidget = CreateWidget<UCurrentDayStatsWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			currentDayStatsWidgetClass);

		if (!currentDayStatsWidget) return;

		currentDayStatsWidget->AddToViewport();
	}
}

void APortfolioBuilders_GM::DestroyCurrentDayStatsWidget()
{
	if (currentDayStatsWidget)
	{
		DisableMouseCursor();

		currentDayStatsWidget->RemoveFromParent();
		currentDayStatsWidget = nullptr;
	}
}

void APortfolioBuilders_GM::InitializePauseMenuWidget()
{
	if (!pauseMenuWidget && pauseMenuWidgetClass)
	{
		DestroyInstructionWidget();
		DestroyPickFoodWidget();
		HideCustomerOrderOverlay();
		DestroyMobileWidget();

		InvalidateTouchInterface();
		EnableMouseCursor();

		pauseMenuWidget = CreateWidget<UPauseMenuWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			pauseMenuWidgetClass);

		if (!pauseMenuWidget) return;

		pauseMenuWidget->AddToViewport();

		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

void APortfolioBuilders_GM::DestroyPauseMenuWidget()
{
	if (pauseMenuWidget)
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			switch (player->GetEPlayerState())
			{
			case EPlayerState::EFreeRoam:

				// If the player originally had the order overlay visible, show it after resuming
				if (player->GetToggleOverlayVisiblity()) ShowCustomerOrderOverlay();

				// If the customer order has finished updating, show the instruction to hide/show customer's order after resuming
				if (customerOrderWidget && customerOrderWidget->GetHasFinishedUpdatingOrder())
				{
					InitializeInstructionWidget("Press ENTER to hide/show customer's order");
					InitializeMobileWidget();

					if (mobileWidget)
					{
						mobileWidget->ShowDropFoodButton();
						mobileWidget->SetDropFoodText("Hide/Show\nOrder");
					}
				}

				// If player is near a customer and they haven't received an order from the customer yet
				if (player->GetCustomerInteraction() && !player->GetReceivedOrderFromCustomer())
				{
					// Show the instruction widget again with the corresponding text
					InitializeInstructionWidget("Press SPACE to take customer's order");
					InitializeMobileWidget();

					if (mobileWidget)
					{
						mobileWidget->ShowInteractionButton();
						mobileWidget->SetInteractionText("Interact");
					}
				}

				// If player is near the cooking area, print the corresponding instruction text on-screen
				if (player->GetCooking())
				{
					InitializeInstructionWidget("Press SPACE to make meal");
					InitializeMobileWidget();

					if (mobileWidget)
					{
						mobileWidget->ShowInteractionButton();
						mobileWidget->SetInteractionText("Make meal");
					}
				}

				break;

			case EPlayerState::ECooking:
				InitializePickFoodWidget(); // Make pick food widget pop up again
				InitializePickFoodMobileWidget();
				break;

			case EPlayerState::EDroppingFood:
				InitializeInstructionWidget("Press ENTER to drop food"); // Make instruction widget for dropping food pop up again
				InitializeMobileWidget();

				if (mobileWidget)
				{
					mobileWidget->ShowInteractionButton();
					mobileWidget->SetInteractionText("Drop Food");
				}

				break;

			case EPlayerState::ECarryingFood:

				// If the player is near a customer while receiving that customer's order
				if (player->GetCustomerInteraction() && player->GetReceivedOrderFromCustomer())
				{
					// Show the instruction widget to deliver order to customer
					InitializeInstructionWidget("Press SPACE to deliver order to customer");
					InitializeMobileWidget();

					if (mobileWidget)
					{
						mobileWidget->ShowInteractionButton();
						mobileWidget->SetInteractionText("Deliver Order");
					}
				}

				// If the player is still in the cooking area, reveal the instruction to leave the cooking area again
				if (player->GetCooking())
				{
					InitializeInstructionWidget("Press SPACE to deliver this meal to the customer");
					InitializeMobileWidget();

					if (mobileWidget)
					{
						mobileWidget->ShowInteractionButton();
						mobileWidget->SetInteractionText("Deliver Meal");
					}
				}

				break;
			}
		}

		UGameplayStatics::SetGamePaused(GetWorld(), false);

		SetTouchInterface();
		DisableMouseCursor();

		pauseMenuWidget->RemoveFromParent();
		pauseMenuWidget = nullptr;
	}
}

void APortfolioBuilders_GM::DestroyPauseMenuWidget(bool goingToMainMenu)
{
	if (pauseMenuWidget)
	{
		DestroySpawnedEntities();

		DestroyInstructionWidget();
		DestroyPickFoodWidget();
		DestroyPickFoodMobileWidget();
		DestroyMobileWidget();
		DestroyCustomerOrderWidget();

		pauseMenuWidget->RemoveFromParent();
		pauseMenuWidget = nullptr;
	}
}

void APortfolioBuilders_GM::InitializeMobileWidget()
{
	if (!mobileWidget && mobileWidgetClass)
	{
		mobileWidget = CreateWidget<UMobileWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), mobileWidgetClass);

		if (!mobileWidget) return;

		mobileWidget->AddToViewport();
	}
}

void APortfolioBuilders_GM::DestroyMobileWidget()
{
	if (mobileWidget)
	{
		mobileWidget->RemoveFromParent();
		mobileWidget = nullptr;
	}
}

void APortfolioBuilders_GM::InitializePickFoodMobileWidget()
{
	if (!pickFoodMobileWidget && pickFoodMobileWidgetClass)
	{
		pickFoodMobileWidget = CreateWidget<UPickFoodMobileWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			pickFoodMobileWidgetClass);

		if (!pickFoodMobileWidget) return;

		pickFoodMobileWidget->AddToViewport();
	}
}

void APortfolioBuilders_GM::DestroyPickFoodMobileWidget()
{
	if (pickFoodMobileWidget)
	{
		pickFoodMobileWidget->RemoveFromParent();
		pickFoodMobileWidget = nullptr;
	}
}

void APortfolioBuilders_GM::SpawnBottomBun()
{
	TArray<AActor*> buns;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ABun::StaticClass(), FName("BottomBun"), buns);

	if (buns.IsEmpty() && bunClass[0]) // If there are no bottom buns found, spawn one
	{
		FVector& spawnLocation = bunSpawnPoint;
		FRotator spawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABun* bottomBun = GetWorld()->SpawnActor<ABun>(bunClass[0], spawnLocation, spawnRotation, spawnParams);
		bottomBun = nullptr;
	}
}

void APortfolioBuilders_GM::SpawnTopBun()
{
	TArray<AActor*> topBuns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopBun::StaticClass(), topBuns);

	if (topBuns.IsEmpty() && bunClass[1]) // If there are no top buns found, spawn one
	{
		FVector& spawnLocation = foodSpawnPoint;
		FRotator spawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ABun* topBun = GetWorld()->SpawnActor<ABun>(bunClass[1], spawnLocation, spawnRotation, spawnParams);
		topBun = nullptr;
	}
}

void APortfolioBuilders_GM::SpawnBurger()
{
	FVector& spawnLocation = foodSpawnPoint;
	FRotator spawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ABurger* burger = GetWorld()->SpawnActor<ABurger>(burgerClass, spawnLocation, spawnRotation, spawnParams);
	burger = nullptr;
}

void APortfolioBuilders_GM::SpawnTopping(FString& toppingName_)
{
	if (toppingClass[toppingName_])
	{
		FVector& spawnLocation = foodSpawnPoint;
		FRotator spawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		ATopping* topping = GetWorld()->SpawnActor<ATopping>(toppingClass[toppingName_], spawnLocation, spawnRotation, spawnParams);
		topping = nullptr;
	}
}

bool APortfolioBuilders_GM::CompletedDay()
{
	TArray<AActor*> customerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomer::StaticClass(), customerActors);

	if (customerActors.IsEmpty())
	{
		GetWorld()->GetTimerManager().SetTimer(timerHandleToShowDayStats, [this]()
		{
			InitializeCurrentDayStatsWidget();

			GetWorld()->GetTimerManager().ClearTimer(timerHandleToShowDayStats);

		}, timeUntilShowDayStats, false);

		return true;
	}

	return false;
}

void APortfolioBuilders_GM::DestroySpawnedEntities()
{
	TArray<AActor*> customerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomer::StaticClass(), customerActors);

	TArray<AActor*> bunActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABun::StaticClass(), bunActors);

	TArray<AActor*> burgerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABurger::StaticClass(), burgerActors);

	for (AActor* customer : customerActors) customer->Destroy();
	for (AActor* bun : bunActors) bun->Destroy();
	for (AActor* burger : burgerActors) burger->Destroy();
}

/*void APortfolioBuilders_GM::WaitUntilToShowCurrentDayStats()
{
	InitializeCurrentDayStatsWidget();
}*/

void APortfolioBuilders_GM::EnableMouseCursor()
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

void APortfolioBuilders_GM::DisableMouseCursor()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away

	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!playerController) return;

	playerController->SetInputMode(GameInputMode);

	// Make sure the mouse cursor is turned off
	if (playerController->bShowMouseCursor != false) playerController->bShowMouseCursor = false;
}

void APortfolioBuilders_GM::SetTouchInterface()
{
	if (touchInterface)
	{
		if (APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			playerController->ActivateTouchInterface(touchInterface);
		}
	}
}

void APortfolioBuilders_GM::InvalidateTouchInterface()
{
	if (touchInterface)
	{
		if (APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			playerController->ActivateTouchInterface(nullptr);
		}
	}
}
