// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoBackWidget.generated.h"

class UCanvasPanel;
class UButton;

/**
 * 
 */
UCLASS()
class GOEDWARECUTSCENE2JAM_API UGoBackWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGoBackWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void GoBackToMainMenu();

	UFUNCTION()
	void MakeBackNormalButtonInvisible();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UCanvasPanel* GoBackCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* BackButton;

	UPROPERTY()
	TWeakObjectPtr<class ACutscenePlayer> cutscenePlayer = nullptr;
};
