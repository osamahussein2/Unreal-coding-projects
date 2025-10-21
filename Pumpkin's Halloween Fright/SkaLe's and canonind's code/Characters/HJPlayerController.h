// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "HJPlayerController.generated.h"

class AUIManager;
class UInputAction;
class UInputMappingContext;
class UWidgetInteractionComponent;
class AHJPlayerCharacter;
class AHJGameModeBase;

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API AHJPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void TogglePauseMenu();

	UFUNCTION(BlueprintPure, Category = "UI")
	AUIManager* GetUIManager() const;


	virtual void OnPossess(APawn* InPawn) override;

protected:
	AHJGameModeBase* GetCachedGameMode();
protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* UIInputMappingContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction;

	UPROPERTY()
	TObjectPtr<AHJGameModeBase> CachedGameMode;

};
