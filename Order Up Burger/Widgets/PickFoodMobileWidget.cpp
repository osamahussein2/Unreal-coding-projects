// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/PickFoodMobileWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "GameMode/PortfolioBuilders_GM.h"

UPickFoodMobileWidget::UPickFoodMobileWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPickFoodMobileWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BurgerButton && !BurgerButton->IsVisible()) BurgerButton->SetVisibility(ESlateVisibility::Visible);
	if (TomatoButton && !TomatoButton->IsVisible()) TomatoButton->SetVisibility(ESlateVisibility::Visible);
	if (CheeseButton && !CheeseButton->IsVisible()) CheeseButton->SetVisibility(ESlateVisibility::Visible);
	if (TopBunButton && !TopBunButton->IsVisible()) TopBunButton->SetVisibility(ESlateVisibility::Visible);
	if (GoBackButton && !GoBackButton->IsVisible()) GoBackButton->SetVisibility(ESlateVisibility::Visible);

	if (BurgerButton) BurgerButton->OnPressed.AddDynamic(this, &UPickFoodMobileWidget::DropBurger);
	if (TomatoButton) TomatoButton->OnPressed.AddDynamic(this, &UPickFoodMobileWidget::DropTomato);
	if (CheeseButton) CheeseButton->OnPressed.AddDynamic(this, &UPickFoodMobileWidget::DropCheese);
	if (TopBunButton) TopBunButton->OnPressed.AddDynamic(this, &UPickFoodMobileWidget::DropTopBun);
	if (GoBackButton) GoBackButton->OnPressed.AddDynamic(this, &UPickFoodMobileWidget::GoBack);
}

void UPickFoodMobileWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPickFoodMobileWidget::DropBurger()
{
	if (BurgerButton->OnPressed.IsBound()) BurgerButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropBurger);
	if (TomatoButton->OnPressed.IsBound()) TomatoButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropTomato);
	if (CheeseButton->OnPressed.IsBound()) CheeseButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropCheese);
	if (TopBunButton->OnPressed.IsBound()) TopBunButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropTopBun);
	if (GoBackButton->OnPressed.IsBound()) GoBackButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::GoBack);

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		// Drop the burger on the player's side
		playerCharacter->StartDroppingBurger();
	}
}

void UPickFoodMobileWidget::DropTomato()
{
	if (BurgerButton->OnPressed.IsBound()) BurgerButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropBurger);
	if (TomatoButton->OnPressed.IsBound()) TomatoButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropTomato);
	if (CheeseButton->OnPressed.IsBound()) CheeseButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropCheese);
	if (TopBunButton->OnPressed.IsBound()) TopBunButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropTopBun);
	if (GoBackButton->OnPressed.IsBound()) GoBackButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::GoBack);

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		FString tomato = "Tomato";

		// Drop the tomato on the player's side
		playerCharacter->StartDroppingTopping(tomato);
	}
}

void UPickFoodMobileWidget::DropCheese()
{
	if (BurgerButton->OnPressed.IsBound()) BurgerButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropBurger);
	if (TomatoButton->OnPressed.IsBound()) TomatoButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropTomato);
	if (CheeseButton->OnPressed.IsBound()) CheeseButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropCheese);
	if (TopBunButton->OnPressed.IsBound()) TopBunButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropTopBun);
	if (GoBackButton->OnPressed.IsBound()) GoBackButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::GoBack);

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		FString cheese = "Cheese";

		// Drop the cheese on the player's side
		playerCharacter->StartDroppingTopping(cheese);
	}
}

void UPickFoodMobileWidget::DropTopBun()
{
	if (BurgerButton->OnPressed.IsBound()) BurgerButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropBurger);
	if (TomatoButton->OnPressed.IsBound()) TomatoButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropTomato);
	if (CheeseButton->OnPressed.IsBound()) CheeseButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropCheese);
	if (TopBunButton->OnPressed.IsBound()) TopBunButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropTopBun);
	if (GoBackButton->OnPressed.IsBound()) GoBackButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::GoBack);

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		// Drop the top bun on the player's side
		playerCharacter->StartDroppingTopBun();
	}
}

void UPickFoodMobileWidget::GoBack()
{
	if (BurgerButton->OnPressed.IsBound()) BurgerButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropBurger);
	if (TomatoButton->OnPressed.IsBound()) TomatoButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropTomato);
	if (CheeseButton->OnPressed.IsBound()) CheeseButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropCheese);
	if (TopBunButton->OnPressed.IsBound()) TopBunButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::DropTopBun);
	if (GoBackButton->OnPressed.IsBound()) GoBackButton->OnPressed.RemoveDynamic(this, &UPickFoodMobileWidget::GoBack);

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		playerCharacter->StartOrEndCooking();
	}
}
