// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/Burger.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Objects/Bun.h"
#include "Player/PlayerCharacter.h"
#include "People/Customer.h"

// Sets default values
ABurger::ABurger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	foodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	foodMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABurger::BeginPlay()
{
	Super::BeginPlay();
	
	foodMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABurger::OnFoodBeginOverlap);
}

// Called every frame
void ABurger::Tick(float DeltaTime)
{
	switch (foodState)
	{
	case FoodState::EMovingSideways:
	{
		Super::Tick(DeltaTime);

		FVector burgerLocation = GetActorLocation();
		burgerLocation = FVector(0.0f, movePos, 0.0f);

		if (!reverseDirection)
		{
			movePos += DeltaTime;

			if (movePos >= maxRightPos) reverseDirection = true;
		}

		else
		{
			movePos -= DeltaTime;

			if (movePos <= -maxRightPos) reverseDirection = false;
		}

		AddActorWorldOffset(burgerLocation);

		break;
	}

	case FoodState::EDroppingFood:
	{
		Super::Tick(DeltaTime);

		FVector burgerLocation = GetActorLocation();
		const FVector& burgerFallLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 1000.0f);

		burgerLocation += (burgerFallLocation - GetActorLocation()).GetSafeNormal() * dropSpeed * DeltaTime;

		SetActorLocation(burgerLocation);

		break;
	}

	default:
		break;
	}
}

void ABurger::DropFood()
{
	foodState = FoodState::EDroppingFood;
}

void ABurger::OnFoodBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (foodState != FoodState::EToppingDropped)
	{
		foodState = FoodState::EToppingDropped;

		TArray<AActor*> buns;
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ABun::StaticClass(), FName("BottomBun"), buns);

		TArray<AActor*> customerActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomer::StaticClass(), customerActors);

		TArray<ACustomer*> customers;

		TArray<AActor*> players;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), players);

		for (AActor* customer : customerActors)
		{
			if (Cast<ACustomer>(customer)->GetCustomerStatus() == CustomerStatus::EMadeOrder)
			{
				customers.Add(Cast<ACustomer>(customer));
			}

			customer = nullptr;
		}

		if (!buns.IsEmpty() && buns[0])
		{
			float distance = FMath::Abs(GetActorLocation().Y - buns[0]->GetActorLocation().Y);

			//UE_LOG(LogTemp, Warning, TEXT("%f"), distance);

			if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				if (APlayerCharacter* player = Cast<APlayerCharacter>(players[0]))
				{
					gameMode->InitializePickFoodWidget();
					gameMode->InitializePickFoodMobileWidget();

					if (distance <= maxDistanceToBottomBun)
					{
						player->IncrementFoodsDropped();

						// If the correct food name is dropped at an assigned index from the customer's meal index
						if (player->GetFoodsDropped() < customers[0]->GetFoodOrders().Num() - 1)
						{
							// Grant player full score
							if (customers[0]->ContainsFoodElement(player->GetFoodsDropped(), foodName))
								gameMode->playerScore += static_cast<int>((1.0f - (distance / maxDistanceToBottomBun)) * 100.0f);
						}

						// Otherwise the player dropped the food name at -1 than its customer's meal index
						else if (player->GetFoodsDropped() != 0 && player->GetFoodsDropped() < 
							customers[0]->GetFoodOrders().Num() - 1 && 
							customers[0]->ContainsFoodElement(player->GetFoodsDropped() - 1, foodName))
						{
							// Grant player a half of a full score
							gameMode->playerScore += static_cast<int>((1.0f - (distance / maxDistanceToBottomBun)) * 50.0f);
						}

						// Otherwise the player dropped the food name at -2 than its customer's meal index
						else if (player->GetFoodsDropped() != 0 && player->GetFoodsDropped() <
							customers[0]->GetFoodOrders().Num() - 1 &&
							customers[0]->ContainsFoodElement(player->GetFoodsDropped() - 2, foodName))
						{
							// Grant player a quarter percent of a full score
							gameMode->playerScore += static_cast<int>((1.0f - (distance / maxDistanceToBottomBun)) * 25.0f);
						}

						// Otherwise the player dropped foods more than they should have for the customers order
						else if (player->GetFoodsDropped() >= customers[0]->GetFoodOrders().Num() - 1)
						{
							// Take a player's score away by whatever the score loss value is
							gameMode->playerScore -= scoreLoss;
							if (gameMode->playerScore <= 0.0f) gameMode->playerScore = 0.0f;
						}


						else
						{
							// Grant player a 10% of a full score
							gameMode->playerScore += static_cast<int>((1.0f - (distance / maxDistanceToBottomBun)) * 10.0f);
						}
					}

					else
					{
						gameMode->playerScore += 0;
						Destroy();
					}
				}
			}

			Cast<APlayerCharacter>(players[0])->SetPlayerStateToCooking();
			players[0] = nullptr;
		}
	}
}

bool ABurger::IfFoodHasNotDroppedYet() const
{
	return foodState == FoodState::EMovingSideways;
}
