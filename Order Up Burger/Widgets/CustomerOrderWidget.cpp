// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/CustomerOrderWidget.h"
#include "People/Customer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/Image.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/Texture2D.h"
#include "Player/PlayerCharacter.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "GameFramework/CharacterMovementComponent.h"

UCustomerOrderWidget::UCustomerOrderWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCustomerOrderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<AActor*> foundCustomers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomer::StaticClass(), foundCustomers);

	for (AActor* customerActors : foundCustomers)
	{
		// Only add the customers who made an order into the array
		if (Cast<ACustomer>(customerActors)->GetCustomerStatus() == CustomerStatus::EMadeOrder)
		{
			customers.Add(Cast<ACustomer>(customerActors));
		}

		customerActors = nullptr;
	}

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (!playerCharacter || !playerController) return;

	playerCharacter->DisableInput(playerController);

	playerCharacter = nullptr;
	playerController = nullptr;

	// If the order overlay isn't visible, make it visible
	if (orderOverlay && !orderOverlay->IsVisible()) orderOverlay->SetVisibility(ESlateVisibility::Visible);

	//UE_LOG(LogTemp, Warning, TEXT("%i"), customers.Num());

	UpdateCustomerOrderList();
}

void UCustomerOrderWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);
}

void UCustomerOrderWidget::UpdateCustomerOrderList()
{
	SpawnBottomBunOnOverlay();

	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UCustomerOrderWidget::SpawnFoodImageOnOverlay, updateOrderTime,
		true);
}

void UCustomerOrderWidget::HideOrderOverlay()
{
	if (orderOverlay && orderOverlay->IsVisible()) orderOverlay->SetVisibility(ESlateVisibility::Hidden);
}

void UCustomerOrderWidget::ShowOrderOverlay()
{
	if (orderOverlay && !orderOverlay->IsVisible()) orderOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UCustomerOrderWidget::SpawnBottomBunOnOverlay()
{
	if (customers.IsEmpty() || imageTextures.IsEmpty()) return;

	mealImages.Add(WidgetTree->ConstructWidget<UImage>(UImage::StaticClass()));

	FSlateBrush brush;
	brush.SetImageSize(imageSize);

	mealImages[currentOrderIndex]->SetBrush(brush);
	mealImages[currentOrderIndex]->SetRenderTransformAngle(-90.0f);
	
	mealImages[currentOrderIndex]->SetBrushFromTexture(imageTextures[0]); // Use bottom bun image

	orderOverlay->AddChildToOverlay(mealImages[currentOrderIndex]);

	if (UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(mealImages[currentOrderIndex]->Slot))
	{
		overlaySlot->SetPadding(FMargin(startingMargin.Left, startingMargin.Top + (currentOrderIndex * marginTopOffset),
			startingMargin.Right, startingMargin.Bottom));
	}

	++currentOrderIndex;
}

void UCustomerOrderWidget::SpawnFoodImageOnOverlay()
{
	if (customers.IsEmpty() || imageTextures.IsEmpty()) return;

	// If the current order index is greater than the last index of the customer's food orders array
	if (currentOrderIndex > customers[0]->GetFoodOrders().Num() - 1)
	{
		// Give instruction to player how to hide/show customer's order
		if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			gameMode->InitializeInstructionWidget("Press ENTER to hide/show customer's order");
			gameMode->InitializeMobileWidget();

			gameMode->SetTouchInterface();

			if (gameMode->GetMobileWidget())
			{
				gameMode->GetMobileWidget()->ShowDropFoodButton();
				gameMode->GetMobileWidget()->SetDropFoodText("Hide/Show\nOrder");
			}
		}

		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		APlayerController* playerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		if (!playerCharacter || !playerController) return;

		// Enable their input again but disable their movement until they hide the customer order overlay
		playerCharacter->GetCharacterMovement()->DisableMovement();
		playerCharacter->EnableInput(playerController);

		playerCharacter = nullptr;
		playerController = nullptr;

		hasFinishedUpdatingOrder = true;

		// Don't run this function anymore
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);

		return;
	}

	mealImages.Add(WidgetTree->ConstructWidget<UImage>(UImage::StaticClass()));

	FSlateBrush brush;
	brush.SetImageSize(imageSize);

	mealImages[currentOrderIndex]->SetBrush(brush);
	mealImages[currentOrderIndex]->SetRenderTransformAngle(-90.0f);

	if (customers[0]->GetFoodOrdersElement(currentOrderIndex) == customers[0]->GetBurger())
	{
		mealImages[currentOrderIndex]->SetBrushFromTexture(imageTextures[1]); // Use burger image
	}

	if (customers[0]->GetFoodOrdersElement(currentOrderIndex) == customers[0]->GetTomato())
	{
		mealImages[currentOrderIndex]->SetBrushFromTexture(imageTextures[2]); // Use tomato image
	}

	if (customers[0]->GetFoodOrdersElement(currentOrderIndex) == customers[0]->GetCheese())
	{
		mealImages[currentOrderIndex]->SetBrushFromTexture(imageTextures[3]); // Use cheese image
	}

	if (customers[0]->GetFoodOrdersElement(currentOrderIndex) == customers[0]->GetTopBun())
	{
		mealImages[currentOrderIndex]->SetBrushFromTexture(imageTextures[4]); // Use top bun image
	}

	orderOverlay->AddChildToOverlay(mealImages[currentOrderIndex]);

	if (UOverlaySlot* overlaySlot = Cast<UOverlaySlot>(mealImages[currentOrderIndex]->Slot))
	{
		overlaySlot->SetPadding(FMargin(startingMargin.Left, startingMargin.Top + (currentOrderIndex * marginTopOffset),
			startingMargin.Right, startingMargin.Bottom));

		/*UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f, %f"), overlaySlot->GetPadding().Left, overlaySlot->GetPadding().Top,
			overlaySlot->GetPadding().Right, overlaySlot->GetPadding().Bottom);*/
	}

	++currentOrderIndex;

	//UE_LOG(LogTemp, Warning, TEXT("%i"), customers[0]->GetMealIndex());
	//UE_LOG(LogTemp, Warning, TEXT("%i"), mealImages.Num());
}

void UCustomerOrderWidget::NativeDestruct()
{
	Super::NativeDestruct();

	for (UImage* mealImage : mealImages)
	{
		mealImage->RemoveFromParent();
		mealImage = nullptr;
	}

	for (ACustomer* customer : customers) customer = nullptr;

	mealImages.Empty();
	customers.Empty();

	if (BlackImage)
	{
		BlackImage->RemoveFromParent();
		BlackImage = nullptr;
	}

	if (orderOverlay)
	{
		orderOverlay->RemoveFromParent();
		orderOverlay = nullptr;
	}

	currentOrderIndex = 0;

	if (hasFinishedUpdatingOrder) hasFinishedUpdatingOrder = false;
}