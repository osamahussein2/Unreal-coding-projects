// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Widgets/LoadingWidget.h"
#include "Widgets/GoBackWidget.h"
#include "GoedWareCutsceneGM.generated.h"

DECLARE_DELEGATE(FOnWatchingCutscene);

/**
 * 
 */
UCLASS()
class GOEDWARECUTSCENE2JAM_API AGoedWareCutsceneGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGoedWareCutsceneGM();

	void SetIsCutscenePlaying(bool value_);

	// Mouse cursor functions
	void EnableMouseCursor();
	void DisableMouseCursor();

	// Main menu widget functions
	void ShowMainMenu();
	void HideMainMenu();

	// Cutscene finished widget functions
	void ShowCutsceneFinished();
	void HideCutsceneFinished();

	// Loading widget functions
	void ShowLoadingWidget(LoadToNextState value_);
	void HideLoadingWidget();

	// Go back widget functions
	void ShowGoBackWidget();
	void HideGoBackWidget();

	void CreateCreditsWidgetComponent();

	int GetLevelSequenceIndex() const { return levelSequenceIndex; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FOnWatchingCutscene watchCutscene;

	// Cutscene related functions
	UFUNCTION()
	void PlayCutscene();

	UFUNCTION()
	void CheckForMoreCutscenes();

	void TransitionToNextGameState();

	// Snow related functions
	void SpawnSnowEffect();
	void DestroySnowEffect();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> playerPawnClass;

	// Cutscene
	UPROPERTY(EditAnywhere)
	TArray<class ULevelSequence*> LevelSequences;

	UPROPERTY()
	int levelSequenceIndex = 0;

	// Main menu widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UMainMenuWidget> mainMenuWidgetClass;

	class UMainMenuWidget* mainMenuWidget;

	// Cutscene finished widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCutsceneFinishedWidget> cutsceneFinishedWidgetClass;

	class UCutsceneFinishedWidget* cutsceneFinishedWidget;

	// Loading widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoadingWidget> loadingWidgetClass;
	
	ULoadingWidget* loadingWidget;

	// Go Back widget
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGoBackWidget> goBackWidgetClass;

	UGoBackWidget* goBackWidget;

	// Snow
	UPROPERTY(EditAnywhere, Category = "Game Weather")
	class UNiagaraSystem* snowEffectNS;

	UPROPERTY(EditAnywhere, Category = "Game Weather")
	FVector snowSpawnLocation = FVector::ZeroVector;

	UPROPERTY()
	class ANiagaraActor* snowEffectActor = nullptr;

	UPROPERTY()
	class ACreditsBillboard* creditsBillboard = nullptr;

	UPROPERTY()
	TWeakObjectPtr<class ACutscenePlayer> cutscenePlayer;
};
