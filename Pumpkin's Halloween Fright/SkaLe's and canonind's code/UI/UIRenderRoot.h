// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIRenderRoot.generated.h"

class UCanvasPanel;
class UUserWidget;

/**
 * Root widget that acts as a container for other UI widgets when rendering UI to a render target
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UUIRenderRoot : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI|RenderRoot")
	UCanvasPanelSlot* AddWidgetToRoot(UUserWidget* WidgetToAdd, int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "UI|RenderRoot")
	void RemoveWidgetFromRoot(UUserWidget* WidgetToRemove);


protected:
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
	UCanvasPanel* RootCanvas = nullptr;
};
