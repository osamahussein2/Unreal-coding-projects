// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class BRACKEYSGAMEJAM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void PlayGame();

	UFUNCTION()
	void GoToOptionsMenu();

	UFUNCTION()
	void GoToCreditsMenu();

	UFUNCTION()
	void QuitGame();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MainMenuCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* BlackImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MainMenuTitleText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* OptionsButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreditsButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton = nullptr;
};
