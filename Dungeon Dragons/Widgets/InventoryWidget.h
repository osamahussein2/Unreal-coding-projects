// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UCanvasPanel;
class UOverlay;
class UImage;
class UTextBlock;
class UButton;
class UProgressBar;

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void UpdateInventory(int inventoryTextIndex_, const int& collectedItems_);

	void UnbindDelegates();

	UFUNCTION()
	void HoveredOnSmallHealthPotion();

	UFUNCTION()
	void UseSmallHealthPotion();

	UFUNCTION()
	void HoveredOnMediumHealthPotion();

	UFUNCTION()
	void UseMediumHealthPotion();

	UFUNCTION()
	void HoveredOnLargeHealthPotion();

	UFUNCTION()
	void UseLargeHealthPotion();

	UFUNCTION()
	void GoBackToGame();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* InventoryCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* InventoryOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	// Inventory slot images
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* InventorySlot1 = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* InventorySlot2 = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* InventorySlot3 = nullptr;

	// Text for how many items the player has for that inventory slot
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InventoryText1 = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InventoryText2 = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InventoryText3 = nullptr;

	// Slot selected image
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SelectedSlotImage = nullptr;

	// Back button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton = nullptr;

	// Health bar
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	// Selected option (int)
	int selectedSlot = 1;
};
