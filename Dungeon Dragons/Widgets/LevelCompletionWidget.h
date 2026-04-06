// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelCompletionWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class DUNGEONCRAWLER2026_API ULevelCompletionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	ULevelCompletionWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void QuitToMainMenu();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* LevelCompletedCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* LevelCompletedTitleText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RestartButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton = nullptr;
};
