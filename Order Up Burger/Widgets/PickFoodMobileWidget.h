// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickFoodMobileWidget.generated.h"

class UCanvasPanel;
class UButton;

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UPickFoodMobileWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPickFoodMobileWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void DropBurger();

	UFUNCTION()
	void DropTomato();

	UFUNCTION()
	void DropCheese();

	UFUNCTION()
	void DropTopBun();

	UFUNCTION()
	void GoBack();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BurgerButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* TomatoButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CheeseButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* TopBunButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GoBackButton;
};
