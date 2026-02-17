// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UCanvasPanel;
class UButton;

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void PauseGame();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PlayerHUDCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* PauseGameButton = nullptr;
};
