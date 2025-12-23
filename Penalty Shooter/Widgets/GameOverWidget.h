// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCOREJAM37_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGameOverWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* GameOverCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* GameOverText;

	float alpha;

	float gameOverTimer;

	UPROPERTY(EditAnywhere)
	float maxGameOverTimer = 1.0f;

	UPROPERTY(EditAnywhere)
	float alphaFadeSpeed = 0.5f;
};
