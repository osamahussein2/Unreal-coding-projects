// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseGameWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class EHGAMEJAM_API UPauseGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPauseGameWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void ResumeGame();

	UFUNCTION()
	void QuitToMainMenu();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PauseMenuCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SemiTransparentImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PauseMenuTitleText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ResumeButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton = nullptr;
};
