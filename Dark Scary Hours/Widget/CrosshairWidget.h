// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class EHGAMEJAM_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCrosshairWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* crosshairImage = nullptr;
};
