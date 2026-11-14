// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DetectionOverlay.generated.h"

class UProgressBar;
class AGuard;

/**
 * 
 */
UCLASS()
class SHROOMJAM2026_API UDetectionOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDetectionOverlay(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	AGuard* GetGuard() const { return guard; }
	void SetGuard(AGuard* guard_) { guard = guard_; }

	UProgressBar* GetDetectionBar() const { return DetectionBar; }

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UProgressBar> DetectionBar = nullptr;

	UPROPERTY()
	TObjectPtr<AGuard> guard = nullptr;
};
