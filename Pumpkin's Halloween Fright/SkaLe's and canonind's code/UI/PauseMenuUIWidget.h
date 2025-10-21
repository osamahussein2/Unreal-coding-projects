// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "PauseMenuUIWidget.generated.h"

class UOptionsWidget;

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UPauseMenuUIWidget : public UBaseUIWidget
{
	GENERATED_BODY()

public:
	UPauseMenuUIWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void OnResumeClicked();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void OnRestartClicked();
	
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void OnSettingsClicked();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void OnMainMenuClicked();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void OnQuitClicked();

	/* Blueprint implementable events */
	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void OnMenuOpened();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void OnMenuClosed();

protected:
	virtual void NativeConstruct() override;
	virtual void OnWidgetShown_Implementation() override;
	virtual void OnWidgetHidden_Implementation() override;

private:
	void SetupInputBindings();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
	FName MenuLevelName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
	TSubclassOf<UOptionsWidget> OptionsWidgetClass;
};
