// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPauseMenuWidget(const FObjectInitializer& ObjectInitializer);

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
