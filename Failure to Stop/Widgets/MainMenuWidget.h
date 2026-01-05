// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UCanvasPanel;
class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class GOEDWARECUTSCENE2JAM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void PlayGame();

	UFUNCTION()
	void MakePlayNormalButtonInvisible();

	UFUNCTION()
	void GoToCredits();

	UFUNCTION()
	void QuitGame();

private:
	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UCanvasPanel* MainMenuCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UImage* MainMenuImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* PlayButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* CreditsButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* QuitButton;

	UPROPERTY()
	TWeakObjectPtr<class ACutscenePlayer> cutscenePlayer = nullptr;
};
