// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGameOverWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& Geometry, float DeltaTime) override;

private:
	UFUNCTION()
	void StartRestartGameTimer();

	UFUNCTION()
	void QuitToMainMenu();

	void RemoveBindedButtonDelegates();

	void RestartGame();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* GameOverCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SemiTransparentImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* GameOverTitleText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RestartButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton = nullptr;

	bool restartPressed = false;

	float restartTimer = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Wait Times", meta = (ClampMin = "0.01"))
	float timeToRestartGame = 1.0f;
};
