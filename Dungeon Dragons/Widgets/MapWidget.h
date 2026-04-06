// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"

class UCanvasPanel;
class UImage;
class UButton;
class AMapSceneCapture2D;

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMapWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	UFUNCTION()
	void GoBack();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MapCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* MapImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton = nullptr;

	UPROPERTY()
	AMapSceneCapture2D* mapSceneCapture = nullptr;
};
