// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompletionWidget.generated.h"

class UCanvasPanel;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API UCompletionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCompletionWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& Geometry, float DeltaTime);

	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void GoToMainMenu();

	void RemoveBindedButtonDelegates();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCanvasPanel* CompletionCanvas = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* CompletionText = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* RestartButton = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton = nullptr;
};
