// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUIWidget.generated.h"

class AUIManager;

/**
 * Base class for all UI widgets in the game
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UBaseUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetOwningManager(AUIManager* NewManager) { OwningManager = NewManager; }

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void TriggerHide();

	/* Lifecycle events. Override in BP or C++ */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
	void OnWidgetShown();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
	void OnWidgetHidden();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
	void OnWidgetInitialized();

	/* Play animations */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI|Animations")
	void PlayShowAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI|Animations")
	void PlayHideAnimation();

	/* Helper to get manager */
	UFUNCTION(BlueprintPure, Category = "UI")
	AUIManager* GetOwningManager() const { return OwningManager; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	class APlayerController* GetOwningPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	class AGameModeBase* GetOwiningGameMode() const;

 	//UFUNCTION(BlueprintCallable, Category = "UI")
 	//class APlayerState* GetOwningPlayerState() const;

	//UFUNCTION(BlueprintCallable, Category = "UI")
	//class APawn* GetOwningPlayerPawn() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category="UI")
	TObjectPtr<AUIManager> OwningManager = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Config")
	bool bPauseGameWhenShown = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Config")
	bool bShowMouseCursor = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Config")
	bool bShouldHideLowerWidget = true;

	/* Whether pause menu can be pushed on top of this widget*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Config")
	bool bAllowsPause = true;

	/* 
	* Should we hide this widget if pause input occurred. This has priority over bAllowsPause
	* Mostly used in menu widgets like options
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Config")
	bool bHideOnPauseToggle = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Config")
	bool bDisablePlayerInput = false;

	/* Animation names for BP animations */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Animations")
	FName ShowAnimationName = "ShowAnim";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Animations")
	FName HideAnimationName = "HideAnim";

	friend AUIManager;


};
