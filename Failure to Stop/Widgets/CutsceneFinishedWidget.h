// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CutsceneFinishedWidget.generated.h"

class UCanvasPanel;
class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class GOEDWARECUTSCENE2JAM_API UCutsceneFinishedWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCutsceneFinishedWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void GoBackToMainMenu();

	UFUNCTION()
	void MakeBackNormalButtonInvisible();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UCanvasPanel* CutsceneFinishedCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UImage* CutsceneFinishedImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* GoBackToMainMenuButton;
};
