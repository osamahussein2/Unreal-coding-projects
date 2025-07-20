// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameWinWidget.generated.h"

/**
 * 
 */
UCLASS()
class KENNEYJAM2025_API UGameWinWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void QuitToMainMenu();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* GameWinCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RestartButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitToMainMenuButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* backgroundImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* GameWinText;
};
