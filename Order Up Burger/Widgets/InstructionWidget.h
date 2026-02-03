// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InstructionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIOBUILDERS_API UInstructionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInstructionWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetInstructionText(FString& text_);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UTextBlock* instructionText;
};
