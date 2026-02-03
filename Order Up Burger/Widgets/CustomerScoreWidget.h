// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomerScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UCustomerScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCustomerScoreWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void ReEnablePlayerInput();

private:
	UPROPERTY(EditAnywhere, Category = "UI elements", meta = (BindWidget))
	class UTextBlock* customerScoreText;

	UPROPERTY(EditAnywhere, Category = "UI elements", meta = (BindWidget))
	class UImage* customerImage;

	UPROPERTY(EditAnywhere, Category = "UI elements")
	TArray<class UTexture2D*> customerImages;

	TArray<class ACustomer*> customers;

	UPROPERTY(EditAnywhere, Category = "Wait Time")
	float timeToReEnablePlayerInput = 2.0f;

	FTimerHandle timerHandle;
};
