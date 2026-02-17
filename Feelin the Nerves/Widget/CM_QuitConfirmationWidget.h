// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CM_QuitConfirmationWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UCM_QuitConfirmationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCM_QuitConfirmationWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void GoBackToCompletionMenu();

	UFUNCTION()
	void GoBackToMainMenu();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* CM_QuitConfirmationCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CM_QuitConfirmationText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* YesButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* NoButton = nullptr;
};
