// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void PlayGame();

	UFUNCTION()
	void GoToLoadGame();

	UFUNCTION()
	void GoToCreditsMenu();

	UFUNCTION()
	void QuitGame();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MainMenuCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MainMenuTitleText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PlayButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* LoadButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CreditsButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton = nullptr;
};
