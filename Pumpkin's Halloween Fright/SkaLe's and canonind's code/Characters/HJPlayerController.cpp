// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HJPlayerController.h"
#include "UI/UIManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/WidgetInteractionComponent.h"
#include "Characters/HJPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Core/HJGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "TVCapture/TVRoomGameMode.h"

void AHJPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(UIInputMappingContext, 1);

}

void AHJPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (PauseAction)
		{
			EnhancedInput->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AHJPlayerController::TogglePauseMenu);
		}
	}
}

void AHJPlayerController::TogglePauseMenu()
{
	if (!GetCachedGameMode()) return;

	CachedGameMode->TogglePause();
}

AUIManager* AHJPlayerController::GetUIManager() const
{
	return Cast<AUIManager>(GetHUD());
}

void AHJPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

AHJGameModeBase* AHJPlayerController::GetCachedGameMode()
{
	if (!CachedGameMode)
	{
		AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
		CachedGameMode = Cast<AHJGameModeBase>(GameModeBase);
	}
	if (!CachedGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("AHJPlayerController: Unable to retrieve game mode"));
	}
	return CachedGameMode;
}
