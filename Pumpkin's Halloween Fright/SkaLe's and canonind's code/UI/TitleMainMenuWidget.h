// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "TitleMainMenuWidget.generated.h"

class UOptionsWidget;
class UCreditsWidget;

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UTitleMainMenuWidget : public UBaseUIWidget
{
	GENERATED_BODY()
	
public:
	UTitleMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void OnPlayClicked();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void OnOptionsClicked();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void OnCreditsClicked();

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void OnQuitClicked();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
	TSubclassOf<UOptionsWidget> OptionsWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
	TSubclassOf<UCreditsWidget> CreditsWidgetClass;
};
