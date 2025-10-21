// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "OffscreenRenderWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(UI), meta=(BlueprintSpawnableComponent))
class GDTV_HALLOWEEN2025_API UOffscreenRenderWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UOffscreenRenderWidgetComponent();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetCustomRenderTarget(UTextureRenderTarget2D* InRenderTarget);

protected:
	virtual void InitWidget() override;

private:
	UPROPERTY(Transient)
	UTextureRenderTarget2D* ExternalRenderTarget = nullptr;
};
