// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/CustomerScoreWidget.h"
#include "People/Customer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Engine/Texture2D.h"

UCustomerScoreWidget::UCustomerScoreWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCustomerScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			// Disable player input once this widget is constructed
			player->DisableInput(playerController);
		}
	}

	TArray<AActor*> customerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomer::StaticClass(), customerActors);

	if (customerActors.IsEmpty()) return;

	for (AActor* customerActor : customerActors)
	{
		// Only add the customers who made an order into the array
		if (Cast<ACustomer>(customerActor)->GetCustomerStatus() == CustomerStatus::EMadeOrder)
		{
			customers.Add(Cast<ACustomer>(customerActor));
		}

		customerActor = nullptr;
	}

	if (!customers[0]) return;

	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		// Divide the accumulated player score by the customer's food orders array (except for the bottom bun, that doesn't count towards the score) 
		gameMode->playerScore /= customers[0]->GetFoodOrders().Num() - 1;
		gameMode->AddCustomerInformation();

		// Print the score inside the text block
		if (customerScoreText)
		{
			customerScoreText->SetText(FText::FromString(TEXT("Customer Score: ") + FString::FromInt(gameMode->playerScore) + 
				TEXT("%")));
		}

		if (customerImage)
		{
			for (int i = 0; i < gameMode->GetCustomerClassLength(); ++i)
			{
				// Make sure the subclass of customer is equal to the game mode's customer class index to set customer image
				if (customers[0]->GetClass() == gameMode->GetCustomerClass(i))
				{
					// If the customer images index isn't valid but the image itself is, destroy that image
					if (!customerImages[i] && customerImage)
					{
						customerImage->RemoveFromParent();
						customerImage = nullptr;
						return;
					}

					customerImage->SetBrushFromTexture(customerImages[i]);
				}
			}
		}
	}

	// Wait for some time to re-enable player input
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UCustomerScoreWidget::ReEnablePlayerInput, timeToReEnablePlayerInput, false);
}

void UCustomerScoreWidget::ReEnablePlayerInput()
{
	// Clear the timer handle
	GetWorld()->GetTimerManager().ClearTimer(timerHandle);

	if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			if (APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
			{
				if (!gameMode->CompletedDay())
				{
					gameMode->SetTouchInterface();
					player->EnableInput(playerController); // Enable player input again
				}
			}
		}

		// Destroy customer score widget
		gameMode->DestroyCustomerScoreWidget();
	}
}
