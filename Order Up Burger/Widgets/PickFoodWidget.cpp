// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/PickFoodWidget.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

UPickFoodWidget::UPickFoodWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPickFoodWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BurgerImage && !BurgerImage->IsVisible()) BurgerImage->SetVisibility(ESlateVisibility::Visible);
	if (TomatoImage && !TomatoImage->IsVisible()) TomatoImage->SetVisibility(ESlateVisibility::Visible);
	if (CheeseImage && !CheeseImage->IsVisible()) CheeseImage->SetVisibility(ESlateVisibility::Visible);
	if (TopBunImage && !TopBunImage->IsVisible()) TopBunImage->SetVisibility(ESlateVisibility::Visible);
}

void UPickFoodWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	// If the burger key was pressed, spawn the burger only if player is found in the level
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustPressed(EKeys::One))
	{
		Super::NativeTick(Geometry, DeltaTime);

		FindPlayerToSpawnBurger();
	}

	// If the tomato key was pressed, spawn the tomato only if player is found in the level
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustPressed(EKeys::Two))
	{
		Super::NativeTick(Geometry, DeltaTime);

		FindPlayerToSpawnTopping("Tomato");
	}

	// If the cheese key was pressed, spawn the cheese only if player is found in the level
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustPressed(EKeys::Three))
	{
		Super::NativeTick(Geometry, DeltaTime);

		FindPlayerToSpawnTopping("Cheese");
	}

	// If the top bun key was pressed, spawn the top bun only if player is found in the level
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustPressed(EKeys::Four))
	{
		Super::NativeTick(Geometry, DeltaTime);

		FindPlayerToSpawnTopBun();
	}
}

void UPickFoodWidget::FindPlayerToSpawnBurger()
{
	// Start by looking for the player in the level
	TArray<AActor*> player;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), player);

	// Try casting the player character into the found actor
	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(player[0]);
	if (!playerCharacter.IsValid()) return;

	// Drop the burger on the player's side
	playerCharacter->StartDroppingBurger();
	playerCharacter = nullptr;
}

void UPickFoodWidget::FindPlayerToSpawnTopping(FString toppingName_)
{
	// Start by looking for the player in the level
	TArray<AActor*> player;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), player);

	// Try casting the player character into the found actor
	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(player[0]);
	if (!playerCharacter.IsValid()) return;

	// Drop the topping's name on the player's side
	playerCharacter->StartDroppingTopping(toppingName_);
	playerCharacter = nullptr;
}

void UPickFoodWidget::FindPlayerToSpawnTopBun()
{
	// Start by looking for the player in the level
	TArray<AActor*> player;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), player);

	// Try casting the player character into the found actor
	TWeakObjectPtr<APlayerCharacter> playerCharacter = Cast<APlayerCharacter>(player[0]);
	if (!playerCharacter.IsValid()) return;

	// Drop the top bun on the player's side
	playerCharacter->StartDroppingTopBun();
	playerCharacter = nullptr;
}
