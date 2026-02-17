// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MM_QuitConfirmationWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class UButton;

/**
 * MM stands for Main Menu
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UMM_QuitConfirmationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMM_QuitConfirmationWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void GoBackToMainMenu();

	UFUNCTION()
	void QuitGame();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* MM_QuitConfirmationCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* MM_QuitConfirmationText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* YesButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* NoButton = nullptr;
};
