// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/TopBun.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Objects/Bun.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "People/Customer.h"

ATopBun::ATopBun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATopBun::BeginPlay()
{
	Super::BeginPlay();

	bunMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &ATopBun::OnTopBunBeginOverlap);
}

void ATopBun::Tick(float DeltaTime)
{
	switch (bunState)
	{
	case BunState::EMovingSideways:
	{
		Super::Tick(DeltaTime);

		FVector bunLocation = GetActorLocation();
		bunLocation = FVector(0.0f, movePos, 0.0f);

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

		AddActorWorldOffset(bunLocation);

		break;
	}

	case BunState::EDroppingBun:
	{
		Super::Tick(DeltaTime);

		FVector bunLocation = GetActorLocation();
		const FVector& bunFallLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 1000.0f);

		bunLocation += (bunFallLocation - GetActorLocation()).GetSafeNormal() * dropSpeed * DeltaTime;

		SetActorLocation(bunLocation);

		break;
	}

	default:
		break;
	}
}

void ATopBun::OnTopBunBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bunState != BunState::EBunDropped)
	{
		bunState = BunState::EBunDropped;

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
					if (distance <= maxDistanceToBottomBun)
					{
						player->IncrementFoodsDropped();

						// If the correct food name is dropped at an assigned index from the customer's meal index
						if (player->GetFoodsDropped() <= customers[0]->GetFoodOrders().Num() - 1)
						{
							// Grant player full score
							if (customers[0]->ContainsFoodElement(player->GetFoodsDropped(), foodName))
								gameMode->playerScore += static_cast<int>((1.0f - (distance / maxDistanceToBottomBun)) * 100.0f);
						}

						gameMode->InitializeInstructionWidget("Press SPACE to deliver this meal to the customer");

						if (gameMode->GetMobileWidget())
						{
							gameMode->GetMobileWidget()->ShowInteractionButton();
							gameMode->GetMobileWidget()->SetInteractionText("Deliver meal");
						}

						player->SetPlayerStateToCarryingFood();
					}

					else
					{
						gameMode->playerScore += 0;
						Destroy();

						gameMode->InitializePickFoodWidget();
						gameMode->InitializePickFoodMobileWidget();
						player->SetPlayerStateToCooking();
					}
				}
			}

			players[0] = nullptr;
		}
	}
}

void ATopBun::DropTopBun()
{
	bunState = BunState::EDroppingBun;
}

bool ATopBun::IfTopBunHasNotDroppedYet() const
{
	return bunState == BunState::EMovingSideways;
}