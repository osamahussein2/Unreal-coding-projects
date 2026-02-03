// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickFoodWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UPickFoodWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPickFoodWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

	void FindPlayerToSpawnBurger();
	void FindPlayerToSpawnTopping(FString toppingName_);
	void FindPlayerToSpawnTopBun();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BurgerImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* TomatoImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* CheeseImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* TopBunImage;
};
