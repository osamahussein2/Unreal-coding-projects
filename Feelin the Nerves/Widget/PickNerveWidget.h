// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PickNerveWidget.generated.h"

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UPickNerveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPickNerveWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PickNerveCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UOverlay* PickNerveOverlay = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;
};
