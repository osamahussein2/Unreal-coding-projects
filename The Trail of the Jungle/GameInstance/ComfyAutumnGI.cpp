// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstance/ComfyAutumnGI.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

UComfyAutumnGI::UComfyAutumnGI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UComfyAutumnGI::Init()
{
    Super::Init();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UComfyAutumnGI::BeginLoadingScreen);
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UComfyAutumnGI::EndLoadingScreen);
}

void UComfyAutumnGI::FinishDestroy()
{
    Super::FinishDestroy();
}

void UComfyAutumnGI::BeginLoadingScreen(const FString& MapName)
{
    if (!loadingWidgetClass || !GetMoviePlayer()) return;

    UUserWidget* loadingWidget = CreateWidget<UUserWidget>(this, loadingWidgetClass);
    if (!loadingWidget) return;

    if (!IsRunningDedicatedServer())
    {
        FLoadingScreenAttributes LoadingScreen{};
        LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
        LoadingScreen.WidgetLoadingScreen = loadingWidget->TakeWidget();
        
        GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
    }
}

void UComfyAutumnGI::EndLoadingScreen(UWorld* InLoadedWorld)
{
}

void UComfyAutumnGI::PopupTutorial()
{
    if (tutorialState != ETutorialState::NotGiven) return;

    tutorialState = ETutorialState::Introduction;
}

void UComfyAutumnGI::FinishIntroductionTutorial()
{
    if (tutorialState != ETutorialState::Introduction) return;

    tutorialState = ETutorialState::PhotoGallery;
}

void UComfyAutumnGI::SetTutorialStateToSignChanged()
{
    if (tutorialState != ETutorialState::PhotoGallery) return;

    tutorialState = ETutorialState::SignChanged;
}

void UComfyAutumnGI::SetTutorialStateCompleted()
{
    if (tutorialState != ETutorialState::SignChanged) return;

    tutorialState = ETutorialState::Completed;
}
