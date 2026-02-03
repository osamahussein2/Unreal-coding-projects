// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DropFoodComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Objects/TopBun.h"
#include "Objects/Burger.h"

// Sets default values for this component's properties
UDropFoodComponent::UDropFoodComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UDropFoodComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UDropFoodComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDropFoodComponent::DropFood(EPlayerState& playerState_)
{
	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (playerState_ == EPlayerState::EDroppingFood)
		{
			gameMode->DestroyInstructionWidget();
			if (gameMode->GetMobileWidget()) gameMode->GetMobileWidget()->HideDropFoodButton();
		}
	}

	TArray<AActor*> anyDroppableFoods;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABurger::StaticClass(), anyDroppableFoods);

	if (!anyDroppableFoods.IsEmpty())
	{
		for (AActor* droppableFood : anyDroppableFoods)
		{
			if (ABurger* burger = Cast<ABurger>(droppableFood))
			{
				if (burger->IfFoodHasNotDroppedYet())
				{
					burger->DropFood();
				}
			}

			droppableFood = nullptr;
		}
	}

	TArray<AActor*> anyDroppableBuns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATopBun::StaticClass(), anyDroppableBuns);

	if (!anyDroppableBuns.IsEmpty())
	{
		for (AActor* droppableBun : anyDroppableBuns)
		{
			if (ATopBun* bun = Cast<ATopBun>(droppableBun))
			{
				if (bun->IfTopBunHasNotDroppedYet())
				{
					bun->DropTopBun();
				}
			}

			droppableBun = nullptr;
		}
	}

	if (APlayerCharacter* player = Cast<APlayerCharacter>(GetOwner()))
	{
		player->DestroyDropFoodComponent();
	}
}