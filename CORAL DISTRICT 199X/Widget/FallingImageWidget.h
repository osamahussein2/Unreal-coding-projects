// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FallingImageWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API UFallingImageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFallingImageWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

	float GetFallingImagePositionY() const { return fallingImagePositionY; }

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* fallingImage = nullptr;

	float fallingImagePositionY = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	TArray<float> fallSpeeds;

	int currentIndex = 0;
};
