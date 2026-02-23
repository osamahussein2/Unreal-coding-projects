// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RewardWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UOverlay;
class UButton;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API URewardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	URewardWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void SetRewardImageMaterial(int index_);

	UFUNCTION()
	void DestroyRewardWidget();

	void RemoveBindedDelegates();

	void DestroyInteractableFishTrigger();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* RewardCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* RewardOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RewardImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton = nullptr;

	UPROPERTY(EditAnywhere, Category = "Reward Materials")
	TArray<UMaterialInterface*> rewardMaterials;
};
