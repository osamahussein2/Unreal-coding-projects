// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "CameraSnapshot.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class COMFYAUTUMNJAM_API UCameraSnapshot : public USceneCaptureComponent2D
{
	GENERATED_BODY()
	
public:
	UCameraSnapshot(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void ReleaseRenderTarget();

	UFUNCTION(BlueprintPure)
	UTextureRenderTarget2D* GetTextureRenderTarget2D() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Render Target Settings")
	int renderTargetWidth = 124;

	UPROPERTY(EditDefaultsOnly, Category = "Render Target Settings")
	int renderTargetHeight = 124;
};
