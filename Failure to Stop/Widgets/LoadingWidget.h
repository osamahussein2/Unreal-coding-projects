// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;

UENUM()
enum class LoadToNextState : uint8
{
	None,
	Play,
	BackToMain
};

/**
 * 
 */
UCLASS()
class GOEDWARECUTSCENE2JAM_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULoadingWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Transition to different widgets from game mode class functions
	LoadToNextState GetLoadingToGameState() const { return loadToNextState; }
	void SetLoadingToGameState(LoadToNextState value_);

private:
	void GameLoaded();

	// Only for setting the randomized loading time depending on what the next game state is retrieved from the game mode
	void SetRandomizedLoadingTime();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* GameLoadingCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* LoadingImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* LoadingText;

	UPROPERTY()
	UMaterialInstanceDynamic* materialInst;

	float loadTimer = 0.0f;
	float randomizedLoadingTime = 0.0f;

	bool hasLoaded = false;

	UPROPERTY(EditAnywhere, Category = "Loading Times")
	float minTimeToLoadGame = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Loading Times")
	float maxTimeToLoadGame = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Loading Times")
	float minTimeToLoadMainMenu = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Loading Times")
	float maxTimeToLoadMainMenu = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Loading Times")
	float maxLoadedTime = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = "Loading State")
	LoadToNextState loadToNextState = LoadToNextState::None;
};
