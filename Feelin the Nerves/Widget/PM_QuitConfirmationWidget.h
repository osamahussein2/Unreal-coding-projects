// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PM_QuitConfirmationWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;
class UButton;

/**
 * PM stands for Pause Menu
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UPM_QuitConfirmationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPM_QuitConfirmationWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void GoBackToPauseMenu();

	UFUNCTION()
	void GoBackToMainMenu();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* PM_QuitConfirmationCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* SemiTransparentImage = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PM_QuitConfirmationText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* YesButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* NoButton = nullptr;
};
