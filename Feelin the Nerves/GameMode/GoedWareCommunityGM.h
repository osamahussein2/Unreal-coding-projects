// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

// Widget include header files
#include "Widget/MainMenuWidget.h"
#include "Widget/PauseMenuWidget.h"
#include "Widget/MM_QuitConfirmationWidget.h"
#include "Widget/PM_QuitConfirmationWidget.h"
#include "Widget/PlayerHUDWidget.h"
#include "Widget/NerveSequenceWidget.h"
#include "Widget/PickThreeNervesWidget.h"
#include "Widget/PickFourNervesWidget.h"
#include "Widget/PickFiveNervesWidget.h"
#include "Widget/SequenceStatsWidget.h"
#include "Widget/CompletionWidget.h"
#include "Widget/CM_QuitConfirmationWidget.h"
#include "Widget/CreditsWidget.h"
#include "Widget/SaveGameWidget.h"
#include "Widget/PM_LoadGameWidget.h"
#include "Widget/MM_LoadGameWidget.h"

#include "Objects/BodyActor.h"
//#include "Objects/NerveActor.h"

// Save game include header
#include "SaveGame/PlayerSaveGame.h"

// Enum include headers
#include "Enums/PickNerveState.h"

#include "GoedWareCommunityGM.generated.h"

/**
 * 
 */
UCLASS()
class GOEDWARECOMMUNITYJAM_API AGoedWareCommunityGM : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGoedWareCommunityGM();

	void PushWidget(TSubclassOf<UUserWidget> widgetClass_);
	void PopWidget(TSubclassOf<UUserWidget> widgetClass_);

	// Widget subclasses getter functions (preventing anyone from modifying the widget subclasses)
	TSubclassOf<UMainMenuWidget> GetMainMenuClass() const;
	TSubclassOf<UPauseMenuWidget> GetPauseMenuClass() const;
	TSubclassOf<UMM_QuitConfirmationWidget> GetMM_QuitConfirmationClass() const;
	TSubclassOf<UPM_QuitConfirmationWidget> GetPM_QuitConfirmationClass() const;
	TSubclassOf<UCM_QuitConfirmationWidget> GetCM_QuitConfirmationClass() const;
	TSubclassOf<UPlayerHUDWidget> GetPlayerHUDClass() const;
	TSubclassOf<UNerveSequenceWidget> GetNerveSequenceClass() const;
	TSubclassOf<UPickThreeNervesWidget> GetPickThreeNervesClass() const;
	TSubclassOf<UPickFourNervesWidget> GetPickFourNervesClass() const;
	TSubclassOf<UPickFiveNervesWidget> GetPickFiveNervesClass() const;
	TSubclassOf<USequenceStatsWidget> GetSequenceStatsClass() const;
	TSubclassOf<UCompletionWidget> GetCompletionClass() const;
	TSubclassOf<UCreditsWidget> GetCreditsClass() const;
	TSubclassOf<USaveGameWidget> GetSaveGameClass() const;
	TSubclassOf<UPM_LoadGameWidget> GetPM_LoadGameClass() const;
	TSubclassOf<UMM_LoadGameWidget> GetMM_LoadGameClass() const;

	// Body actor spawn and destroy functions
	void SpawnBodyActor();
	void DestroyBodyActor();

	ABodyActor* GetBodyActor() const;

	// Body nerve spawn function
	//void SpawnNerveActors();
	//void DestroyNerveActors();

	void UpdateBodySprite();

	// Mouse cursor functions
	void EnableMouseCursor();
	void DisableMouseCursor();

	// Correct sequences functions
	int GetCurrentPlayedSequences() const;
	int GetMaxPlayedSequences() const;

	int GetCurrentPickedSequences() const;

	int GetCurrentCorrectSequences() const;
	int GetMaxCorrectSequences() const;

	int GetCurrentCompletedSequences() const;
	int GetMaxCompletedSequences() const;

	// Random nerves index getter functions
	TArray<int> GetRandomNerves() const;
	int GetRandomNervesSize() const;

	int GetRandomNerveIndex(int value_) const;
	int GetReferencedRandomNerveIndex(int& value_) const;
	int FindRandomNerveAtReferencedIndex(int& value_) const;

	int GetBlueNervesTextureNumber(int value_) const;
	int GetOrangeNervesTextureNumber(int value_) const;
	int GetPurpleNervesTextureNumber(int value_) const;
	int GetYellowNervesTextureNumber(int value_) const;

	void AddRandomNerves();

	void IncrementPickedSequences();
	void IncrementPlayedSequences();
	void IncrementCorrectSequences();
	void IncrementCompletedSequences();

	void UpdateValuesBasedOnCompletedSequences();

	void ResetCurrentSequenceStats(bool resetArray_ = true);
	void ResetGame();

	// Nerve textures array functions
	int GetBlueNerveTexturesSize() const;
	int GetOrangeNerveTexturesSize() const;
	int GetPurpleNerveTexturesSize() const;
	int GetYellowNerveTexturesSize() const;

	UTexture2D* GetBlueNerveTextureByIndex(int value_) const;
	UTexture2D* GetOrangeNerveTextureByIndex(int value_) const;
	UTexture2D* GetPurpleNerveTextureByIndex(int value_) const;
	UTexture2D* GetYellowNerveTextureByIndex(int value_) const;

	// Nerve state function
	EPickNerveState GetPickNerveState() const;
	void SetPickNerveState(EPickNerveState value_);

	// Save game function for loading data
	void LoadGame(UPlayerSaveGame* playerSaveGame_);
	
protected:
	virtual void BeginPlay() override;

private:
	// Widget array holding UUserWidget pointers
	UPROPERTY(VisibleAnywhere, Category = "Widgets")
	TArray<UUserWidget*> cachedWidgets;

	// Widget subclasses
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> mainMenuClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPauseMenuWidget> pauseMenuClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMM_QuitConfirmationWidget> mm_QuitConfirmationClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPM_QuitConfirmationWidget> pm_QuitConfirmationClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UCM_QuitConfirmationWidget> cm_QuitConfirmationClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPlayerHUDWidget> playerHUDClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UNerveSequenceWidget> nerveSequenceClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPickThreeNervesWidget> pickThreeNervesClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPickFourNervesWidget> pickFourNervesClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPickFiveNervesWidget> pickFiveNervesClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<USequenceStatsWidget> sequenceStatsClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UCompletionWidget> completionClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UCreditsWidget> creditsClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<USaveGameWidget> saveGameClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UMM_LoadGameWidget> mm_LoadGameClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UPM_LoadGameWidget> pm_LoadGameClass;

	// Body/nerve actor subclass
	UPROPERTY(EditAnywhere, Category = "Body Actor Parameters")
	TSubclassOf<ABodyActor> bodyActorClass;

	/*UPROPERTY(EditAnywhere, Category = "Body Actor Parameters")
	TSubclassOf<ANerveActor> nerveActorClass;*/

	UPROPERTY(EditAnywhere, Category = "Body Actor Parameters")
	FVector bodyActorSpawnLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Body Actor Parameters")
	FRotator bodyActorSpawnRotation = FRotator::ZeroRotator;

	// Body actor
	UPROPERTY()
	ABodyActor* bodyActor;

	// Array of nerve actors
	/*UPROPERTY(VisibleAnywhere)
	TArray<ANerveActor*> nerveActors;*/

	// Correct sequences
	int currentPlayedSequences = 0;
	int maxPlayedSequences = 3;

	int currentPickedSequences = 0;

	int currentCorrectSequences = 0;
	int maxCorrectSequences = 3;

	int currentCompletedSequences = 0;
	int maxCompletedSequences = 4;

	// Pick a random index for nerves
	UPROPERTY(VisibleAnywhere, Category = "Sequences")
	TArray<int> randomNerves;

	// Nerve textures
	UPROPERTY(EditAnywhere, Category = "Nerve Textures")
	TArray<UTexture2D*> blueNerves;

	UPROPERTY(EditAnywhere, Category = "Nerve Textures")
	TArray<UTexture2D*> orangeNerves;

	UPROPERTY(EditAnywhere, Category = "Nerve Textures")
	TArray<UTexture2D*> purpleNerves;

	UPROPERTY(EditAnywhere, Category = "Nerve Textures")
	TArray<UTexture2D*> yellowNerves;

	// Picking nerve state
	EPickNerveState pickNerveState = EPickNerveState::ENone;

	int nerveIndex = 0;

	// Music
	UPROPERTY(EditAnywhere, Category = "Music")
	USoundBase* gameMusic;

	// Scene camera
	/*UPROPERTY()
	class ACineCameraActor* sceneCamera = nullptr;*/
};
