// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/HJGameModeBase.h"
#include "TVRoomGameMode.generated.h"

class USceneCaptureComponent2D;
class UCameraComponent;
class AUIManager;
class UBaseUIWidget;
class ATVUIActor;
class ALightActorSystem;

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API ATVRoomGameMode : public AHJGameModeBase
{
	GENERATED_BODY()

	ATVRoomGameMode();
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void RestartGame() override;
	virtual void StartGame() override;
	virtual void EndGame() override;
	virtual void GoToMainMenu() override;

	virtual bool LoadNextLevel() override;
	virtual void ResetLevels(bool bBlock)override;

	virtual void PlayEndSequence() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void StartInsertCassetteSequence();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void StartPullOutCassetteSequence();

	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void StartEndGameSequence();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void OnCassetteInserted();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void OnTVCrashed();

	void OnCassettePulledOut();

private:
	UFUNCTION()
	void OnTVWorldLoaded();
	void SetCameraReferences();
	void SetViewTarget();
	void CopyCameraSettingsToSceneCapture();
	void AttachCaptureActorToCamera();
	void CacheTVActor();
	void DisplayMainMenu();
	void InitializeCharacter();
	void DestroyCharacter();
	bool SpawnPlayer();
	UFUNCTION()
	void UpdateCaptureDOF(float TargetLength);

	void OnTVTurnedOn();
	void OnTVTurnedOff();

	void ExecuteQuitGame();

	virtual void OnUIManagerReady(AUIManager* UIManager) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerSetup")
	TSubclassOf<ACharacter> PlayerCharacterClass;

	UPROPERTY(VisibleAnywhere, Category = "PlayerSetup")
	TObjectPtr<ACharacter> CachedPlayerCharacter;

	/* Time to wait to turn on the TV after game started*/
	UPROPERTY(VisibleAnywhere, Category = "Start")
	float TVTurnOnDelay = 2.f;

	/* Time to turn off the TV from the moment cassette started sequence */
	UPROPERTY(VisibleAnywhere, Category = "Start")
	float TVTurnOffDelay = 4.f; 

	/* Time to wait before starting insert sequence after the TV turned on */
	UPROPERTY(VisibleAnywhere, Category = "Start")
	float CassetteInsertDelay = 1.f;

	/* Time to wait before showing level after Play was clicked */
	UPROPERTY(VisibleAnywhere, Category = "Start")
	float StartFirstLevelDelay = 1.f;

	/* Time to wait before closing the game after the TV turned off */
	UPROPERTY(VisibleAnywhere, Category = "Start")
	float QuitGameDelay = 1.f; 

	UPROPERTY(VisibleAnywhere, Category = "Start")
	float SpawnDelay = 1.f; 

	UPROPERTY(VisibleAnywhere, Category = "Start")
	float StartNarrationDelay = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Materials")
	UMaterialInstanceDynamic* DynamicPostProcessMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterialInterface* BasePostProcessMaterial;

private:
	UPROPERTY()
	UCameraComponent* PlayerCameraComponent = nullptr;
	UPROPERTY()
	AActor* SceneCaptureActor = nullptr;

	UPROPERTY()
	ATVUIActor* TVActor = nullptr;

	UPROPERTY()
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponentCached= nullptr;


};
