// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CustomerInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "People/Customer.h"
#include "Objects/Bun.h"
#include "Objects/Burger.h"

// Sets default values for this component's properties
UCustomerInteractionComponent::UCustomerInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCustomerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCustomerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCustomerInteractionComponent::InteractWithCustomer(bool& receivedOrder_, int& customerNumber_, bool& overlayVisibility_,
	int& foodsDropped_, EPlayerState& playerState_)
{
	TArray<AActor*> customers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomer::StaticClass(), customers);

	for (AActor* customerActor : customers)
	{
		ACustomer* customer = Cast<ACustomer>(customerActor);
		if (!customer) return;

		// If the customer is requesting an order (meaning they haven't made an order yet)
		if (customer->GetTriggerOverlapped() && customer->GetCustomerStatus() == CustomerStatus::ERequestingOrder && 
			!receivedOrder_)
		{
			customer->TakeOrder();

			++customerNumber_;

			if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				gameMode->DestroyInstructionWidget();
				if (gameMode->GetMobileWidget()) gameMode->GetMobileWidget()->HideInteractionButton();

				gameMode->InvalidateTouchInterface();

				gameMode->InitializeCustomerOrderWidget();
			}

			receivedOrder_ = true;
			overlayVisibility_ = true;

			customerActor = nullptr;
			customer = nullptr;

			if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				player->DestroyCustomerInteractionComponent();
			}
		}

		// Otherwise, the customer has made an order
		else if (customer->GetTriggerOverlapped() && customer->GetCustomerStatus() == CustomerStatus::EMadeOrder && 
			receivedOrder_ && playerState_ == EPlayerState::ECarryingFood)
		{
			if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				playerState_ = EPlayerState::EFreeRoam; // Go back to free roam state now that the food is destroyed

				gameMode->DestroyInstructionWidget();
				if (gameMode->GetMobileWidget()) gameMode->GetMobileWidget()->HideInteractionButton();

				gameMode->InvalidateTouchInterface();

				gameMode->InitializeCustomerScoreWidget();

				customer->Destroy();
				DestroyFood();

				receivedOrder_ = false;
				foodsDropped_ = 0;

				customerActor = nullptr;
				customer = nullptr;

				if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
				{
					player->DestroyCustomerInteractionComponent();
				}
			}
		}
	}
}

void UCustomerInteractionComponent::DestroyFood()
{
	if (!GetOwner()) return;

	TArray<AActor*> buns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABun::StaticClass(), buns);

	TArray<AActor*> foods;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABurger::StaticClass(), foods);

	for (AActor* bun : buns)
	{
		if (bun->IsAttachedTo(GetOwner())) bun->Destroy();
		bun = nullptr;
	}

	for (AActor* food : foods)
	{
		if (food->IsAttachedTo(GetOwner())) food->Destroy();
		food = nullptr;
	}
}

