// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/LoadingWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/GoedWareCutsceneGM.h"

ULoadingWidget::ULoadingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ULoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Make sure the world object paused game if it isn't already
	if (!UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), true);

	// Set the loading text to say "LOADING" to give the illusion that there is a loading screen
	if (LoadingText) LoadingText->SetText(FText::FromString("LOADING..."));

	// Make sure material instance dynamic hasn't been allocated yet and that loading image is a valid object
	if (!materialInst && LoadingImage)
	{
		// Create a material instance dynamic if the loading image uses an actual material
		materialInst = UMaterialInstanceDynamic::Create(Cast<UMaterialInterface>(LoadingImage->GetBrush().GetResourceObject()), this);
	}

	// Material instance dynamic isn't valid, don't set brush of the loading image
	if (!materialInst) return;

	FSlateBrush brush;
	brush.SetResourceObject(materialInst);

	// Update the loading image to use the brush that includes the material instance dynamic
	LoadingImage->SetBrush(brush);
}

void ULoadingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Increment load time to update loading logic
	loadTimer += InDeltaTime;

	// Call the game loaded function
	GameLoaded();
}

void ULoadingWidget::GameLoaded()
{
	// If the load timer exceeds the randomized loading time along with has loaded being false
	if (loadTimer >= randomizedLoadingTime && !hasLoaded)
	{
		// Don't update the circular motion anymore once it has loaded
		if (materialInst) materialInst->SetScalarParameterValue(TEXT("MoveSpeed"), 0.0f);

		// Update the loading text to say "LOADED" to give the illusion that it's loaded
		if (LoadingText) LoadingText->SetText(FText::FromString("LOADED"));

		// Reset loading variables except for has loaded boolean (should be set to true to remove this widget eventually)
		loadTimer = 0.0f;
		hasLoaded = true;
	}

	// If the load timer exceeds the max loaded time along with has loaded being true
	else if (loadTimer >= maxLoadedTime && hasLoaded)
	{
		if (AGoedWareCutsceneGM* cutsceneGM = Cast<AGoedWareCutsceneGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// Hide the loading widget and transition to the next game state
			cutsceneGM->HideLoadingWidget();
			UGameplayStatics::SetGamePaused(GetWorld(), false); // Unpause the game
		}

		// Reset loading variables
		loadTimer = 0.0f;
		randomizedLoadingTime = 0.0f;
		hasLoaded = false;
	}
}

void ULoadingWidget::SetLoadingToGameState(LoadToNextState value_)
{
	loadToNextState = value_;

	// Call set randomized loading time upon setting load to next state value
	SetRandomizedLoadingTime();
}

void ULoadingWidget::SetRandomizedLoadingTime()
{
	// Randomize the loading time by set minimum and maximum values depending on load to next state value
	switch (loadToNextState)
	{
	case LoadToNextState::Play:
		randomizedLoadingTime = FMath::RandRange(minTimeToLoadGame, maxTimeToLoadGame);
		break;

	case LoadToNextState::BackToMain:
		randomizedLoadingTime = FMath::RandRange(minTimeToLoadMainMenu, maxTimeToLoadMainMenu);
		break;

	default:
		break;
	}
}