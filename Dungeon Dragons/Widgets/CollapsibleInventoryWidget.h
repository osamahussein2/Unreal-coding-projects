// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CollapsibleInventoryWidget.generated.h"

class UOverlay;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API UCollapsibleInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCollapsibleInventoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void UpdateInventorySlot();

private:
	//void DestroyInventoryText(int inventoryTextIndex_);
	void UpdateInventoryText(int inventoryTextIndex_, const int& collectedItems_);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* InventoryOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	// Inventory slot images
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* InventorySlot1 = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* InventorySlot2 = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* InventorySlot3 = nullptr;

	// Text for how many items the player has for that inventory slot
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InventoryText1 = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InventoryText2 = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* InventoryText3 = nullptr;
};
