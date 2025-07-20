// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Engine/Level.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class KENNEYJAM2025_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void PlayLevel();

	UFUNCTION()
	void QuitGame();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MainMenuCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* playButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* quitButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* backgroundImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MainMenuText;
};
