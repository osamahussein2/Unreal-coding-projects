// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Layout/Anchors.h"
#include "PlayerHUDWidget.generated.h"

class UCanvasPanel;
class UOverlay;
class UTextBlock;
class UDetectionOverlay;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class SHROOMJAM2026_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void UpdateCoinCollectedText(int coinsCollected_);

	UFUNCTION()
	void SpawnDetectionBar(class AGuard* guard_);

	UFUNCTION()
	void UpdateDetectionBar(class AGuard* guard_, float detectionValue_);

	UFUNCTION()
	void AnimateDetectionBar(class AGuard* guard_);

	UFUNCTION()
	void DestroyDetectionBar(class AGuard* guard_);

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> PlayerHUDCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UOverlay> CoinOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> TicketAmountText = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UDetectionOverlay>> detectionOverlays = TArray<TObjectPtr<UDetectionOverlay>>();

	UPROPERTY(EditAnywhere, Category = "Detection Overlay")
	TSubclassOf<UDetectionOverlay> detectionOverlayClass = TSubclassOf<UDetectionOverlay>();

	UPROPERTY(EditAnywhere, Category = "Detection Overlay")
	FAnchors detectionOverlayAnchor = FAnchors();

	UPROPERTY(EditAnywhere, Category = "Detection Overlay")
	FVector2D detectionOverlayPosition = FVector2D(0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Detection Overlay")
	FVector2D detectionOverlaySize = FVector2D(0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Detection Overlay")
	FVector2D detectionOverlayAlignment = FVector2D(0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "Detection Overlay")
	float detectionOverlayRadius = 200.f;

	TWeakObjectPtr<APlayerCharacter> playerCharacter = nullptr;

	int totalCoins = 0;

	FVector2D detectionBarScale = FVector2D(1.0f, 1.0f);

	bool scaleIncreasing = true;

	UPROPERTY(EditAnywhere, Category = "Detection Settings")
	float detectionScaleSpeed = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Detection Settings")
	FVector2D minimumDetectionBarScale = FVector2D(0.5f, 0.5f);

	UPROPERTY(EditAnywhere, Category = "Detection Settings")
	FVector2D maximumDetectionBarScale = FVector2D(1.5f, 1.5f);
};
