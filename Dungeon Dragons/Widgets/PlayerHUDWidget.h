// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UCanvasPanel;
class UImage;
class UProgressBar;
class UOverlay;
class UTextBlock;
class UHowToUnlockDoorWidget;
class UCollapsibleInventoryWidget;

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	void SetHealthBar(float healthPercentage_);
	void SetPlayerStats(float dexterity_, float strength_, float intelligence_);

	void UpdateDieImage();

	void SpawnUnlockDoorWidget();
	void DestroyUnlockDoorWidget();

	void PopInventoryUp();

private:
	//void SpawnInventoryWidget();

	UFUNCTION()
	void MoveInventoryOnScreen();

	UFUNCTION()
	void StartHidingInventory();

	UFUNCTION()
	void MoveInventoryOffScreen();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PlayerHUDCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* MinimapImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* CharacterPortrait = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* DiceRoll = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* DexterityBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* StrengthBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* IntelligenceBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* StatsOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackOverlayImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* HealthText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DexterityText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* StrengthText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* IntelligenceText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* HealthPercentageText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* DexterityPercentageText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* StrengthPercentageText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* IntelligencePercentageText = nullptr;

	UPROPERTY(EditAnywhere, Category = "Dice")
	TArray<UTexture2D*> dieTextures;

	// Unlock door widget
	UPROPERTY(EditDefaultsOnly, Category = "Unlock Door Widget")
	TSubclassOf<UHowToUnlockDoorWidget> unlockDoorWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Unlock Door Widget")
	FKey PressToDestroyUnlockDoorWidget = EKeys::SpaceBar;

	UPROPERTY()
	UHowToUnlockDoorWidget* unlockDoorWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Unlock Door Widget")
	FVector2D unlockDoorWidgetPosition = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Unlock Door Widget")
	FVector2D unlockDoorWidgetSize = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Unlock Door Widget")
	FVector2D unlockDoorWidgetAlignment = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Unlock Door Widget")
	bool unlockDoorWidgetAutoSize = true;

	// Collapsible Inventory widget
	/*UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget")
	TSubclassOf<UCollapsibleInventoryWidget> inventoryWidgetClass;*/

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCollapsibleInventoryWidget* inventoryWidget = nullptr;

	/*UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget")
	FVector2D inventoryWidgetPosition = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget")
	FVector2D inventoryWidgetSize = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget")
	FVector2D inventoryWidgetAlignment = FVector2D(0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget")
	bool inventoryWidgetAutoSize = true;*/

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget")
	float stoppingPositionY = -200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget", meta = (ClampMin = "0.1"))
	float inventoryMoveSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Widget")
	float timeToHideInventory = 2.0f;

	float inventoryPos = 0.0f;
	float initialInventoryPos = 0.0f;

	FTimerHandle showInventoryHandle{}, hideInventoryHandle{};

	bool isInventoryPoppingUp = false;
};
