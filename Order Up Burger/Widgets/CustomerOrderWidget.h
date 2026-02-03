// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomerOrderWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UCustomerOrderWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCustomerOrderWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	void UpdateCustomerOrderList();

	void HideOrderOverlay();
	void ShowOrderOverlay();

	bool GetHasFinishedUpdatingOrder() const { return hasFinishedUpdatingOrder; }

	void SetHasFinishedUpdatingOrderToFalse() { hasFinishedUpdatingOrder = false; }

private:
	void SpawnBottomBunOnOverlay();
	void SpawnFoodImageOnOverlay();

private:
	UPROPERTY(EditAnywhere, Category = "Updating Order Parameters")
	float updateOrderTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Updating Order Parameters")
	TArray<class UTexture2D*> imageTextures;

	FTimerHandle timerHandle;

	TArray<class UImage*> mealImages;

	TArray<class ACustomer*> customers;

	// UI components
	UPROPERTY(EditAnywhere, Category = "UI Objects", meta = (BindWidget))
	class UOverlay* orderOverlay = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI Objects", meta = (BindWidget))
	class UImage* BlackImage = nullptr;

	// Image parameter variables
	UPROPERTY(EditAnywhere, Category = "Image Parameters")
	FDeprecateSlateVector2D imageSize = FDeprecateSlateVector2D(100.0f, 100.0f);

	UPROPERTY(EditAnywhere, Category = "Image Parameters")
	FMargin startingMargin = FMargin(5.0f, 5.0f, 5.0f, 5.0f);

	UPROPERTY(EditAnywhere, Category = "Image Parameters")
	float marginTopOffset = 150.0f;

	// Variables for updating customer order
	int currentOrderIndex = 0;

	bool hasFinishedUpdatingOrder = false;
};
