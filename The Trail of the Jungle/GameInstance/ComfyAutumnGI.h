// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ComfyAutumnGI.generated.h"

class UUserWidget;

UENUM(BlueprintType)
enum class ETutorialState : uint8
{
    NotGiven,
    Introduction,
    PhotoGallery,
    SignChanged,
    Completed
};

/**
 * 
 */
UCLASS()
class COMFYAUTUMNJAM_API UComfyAutumnGI : public UGameInstance
{
	GENERATED_BODY()
	
public:
    UComfyAutumnGI(const FObjectInitializer& ObjectInitializer);

    ETutorialState GetTutorialState() const { return tutorialState; }

    void PopupTutorial();

private:
    virtual void Init() override;
    virtual void FinishDestroy() override;

    UFUNCTION()
    virtual void BeginLoadingScreen(const FString& MapName);

    UFUNCTION()
    virtual void EndLoadingScreen(UWorld* InLoadedWorld);

    UFUNCTION(BlueprintCallable)
    void FinishIntroductionTutorial();

    UFUNCTION(BlueprintCallable)
    void SetTutorialStateToSignChanged();

    UFUNCTION(BlueprintCallable)
    void SetTutorialStateCompleted();

private:
    UPROPERTY(EditAnywhere, Category = "Loading Settings")
    TSubclassOf<UUserWidget> loadingWidgetClass = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Tutorial Settings", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    ETutorialState tutorialState = ETutorialState::NotGiven;
};
