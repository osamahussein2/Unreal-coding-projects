// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/MainMenuActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager/WidgetManager.h"
#include "Widget/MainMenuWidget.h"

// Sets default values
AMainMenuActor::AMainMenuActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMainMenuActor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController) return;

	AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
	if (!widgetManager) return;
	
	widgetManager->EnableMouseCursor();
	widgetManager->PushWidget(widgetManager->GetMainMenuClass());
}

// Called every frame
void AMainMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

