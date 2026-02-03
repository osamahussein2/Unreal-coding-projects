// Fill out your copyright notice in the Description page of Project Settings.

#include "People/Customer.h"
#include "Components/BoxComponent.h"
#include "Player/PlayerCharacter.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/Bun.h"

// Sets default values
ACustomer::ACustomer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	customerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CustomerMesh"));
	customerMesh->SetupAttachment(RootComponent);

	customerMesh->SetWorldScale3D(FVector(0.5f));

	boxMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxMesh"));
	boxMesh->SetupAttachment(customerMesh);

	boxMesh->SetGenerateOverlapEvents(true);

	boxMesh->SetBoxExtent(FVector(100.f, 150.f, 150.f));
}

// Called when the game starts or when spawned
void ACustomer::BeginPlay()
{
	Super::BeginPlay();

	boxMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACustomer::OnComponentBeginOverlap);
	boxMesh->OnComponentEndOverlap.AddUniqueDynamic(this, &ACustomer::OnComponentEndOverlap);
}

// Called every frame
void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACustomer::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && !triggerOverlapped && 
		customerStatus == CustomerStatus::ERequestingOrder)
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			if (!player->GetReceivedOrderFromCustomer() && player->GetEPlayerState() != EPlayerState::ECarryingFood)
			{
				player->SpawnCustomerInteractionComponent();

				if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					gameMode->InitializeInstructionWidget("Press SPACE to take customer's order");
					gameMode->InitializeMobileWidget();

					if (gameMode->GetMobileWidget())
					{
						gameMode->GetMobileWidget()->ShowInteractionButton();
						gameMode->GetMobileWidget()->SetInteractionText("Interact");
					}

					triggerOverlapped = true;
				}
			}
		}
	}

	else if (OtherActor->IsA(APlayerCharacter::StaticClass()) && !triggerOverlapped &&
		customerStatus == CustomerStatus::EMadeOrder)
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			if (player->GetReceivedOrderFromCustomer() && player->GetEPlayerState() == EPlayerState::ECarryingFood)
			{
				player->SpawnCustomerInteractionComponent();

				if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					gameMode->InitializeInstructionWidget("Press SPACE to deliver order to customer");
					gameMode->InitializeMobileWidget();

					if (gameMode->GetMobileWidget())
					{
						gameMode->GetMobileWidget()->ShowInteractionButton();
						gameMode->GetMobileWidget()->SetInteractionText("Deliver");
					}
				}

				triggerOverlapped = true;
			}
		}
	}
}

void ACustomer::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && triggerOverlapped)
	{
		if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->DestroyInstructionWidget();
			
			if (gameMode->GetMobileWidget()) gameMode->GetMobileWidget()->HideInteractionButton();
		}

		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			// Destroy the customer interaction component when no longer overlapping with the customer trigger box
			player->DestroyCustomerInteractionComponent();
		}

		triggerOverlapped = false;
	}
}

void ACustomer::TakeOrder()
{
	customerStatus = CustomerStatus::EMadeOrder;

	randomizedMealIndex = FMath::RandRange(minMealIndex, maxMealIndex);

	// Give bottom bun order first
	foodOrders.Add(bottomBun);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *foodOrders[mealIndex]);

	++mealIndex;

	LoopThroughMealOrderIndex();
}

void ACustomer::LoopThroughMealOrderIndex()
{
	int randomizeOrder = FMath::RandRange(0, 2);
	int randomizeNumberOfBurgers = FMath::RandRange(1, 3);
	int randomizedIndexToIncludeBurger = 0;

	switch (randomizeNumberOfBurgers)
	{
	case 1:
		randomizedIndexToIncludeBurger = FMath::RandRange(1, randomizedMealIndex - 2);
		break;

	case 2:
		randomizedIndexToIncludeBurger = FMath::RandRange(1, randomizedMealIndex - 3);
		break;

	case 3:
		randomizedIndexToIncludeBurger = FMath::RandRange(1, randomizedMealIndex - 4);
		break;

	default:
		break;
	}

	int burgersAdded = 0;

	for (mealIndex = 1; mealIndex < randomizedMealIndex - 1; ++mealIndex)
	{
		randomizeOrder = FMath::RandRange(1, 2);

		// If the meal index is equal to the randomized index to include burger, set randomize order value to 0 (order burger)
		if (mealIndex == randomizedIndexToIncludeBurger)
		{
			++burgersAdded;
			randomizeOrder = 0;

			// If there is more than 1 burger that the customer wants in their order, randomize the index to include burger again
			if (burgersAdded < randomizeNumberOfBurgers)
			{
				switch (randomizeNumberOfBurgers)
				{
				case 1:
					break;

				case 2:
					randomizedIndexToIncludeBurger = FMath::RandRange(mealIndex + 1, randomizedMealIndex - 2);
					break;

				case 3:
					if (burgersAdded == 1) randomizedIndexToIncludeBurger = FMath::RandRange(mealIndex + 1, randomizedMealIndex - 3);
					else if (burgersAdded == 2) randomizedIndexToIncludeBurger = FMath::RandRange(mealIndex + 1, randomizedMealIndex - 2);
					break;

				default:
					break;
				}
			}
		}

		switch (randomizeOrder)
		{
		case 0:
			foodOrders.Add(burger);
			break;

		case 1:
			foodOrders.Add(tomato);
			break;

		case 2:
			foodOrders.Add(cheese);
			break;
		}

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *foodOrders[mealIndex]);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Burgers Added: %i/%i"), burgersAdded, randomizeNumberOfBurgers);

	// For last meal index
	foodOrders.Add(topBun);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *foodOrders[mealIndex]);

	//UE_LOG(LogTemp, Warning, TEXT("%i"), foodOrders.Num());
}