// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MapWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Player/PlayerCharacter.h"
#include "SceneCapture/MapSceneCapture2D.h"

UMapWidget::UMapWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	for (TActorIterator<AMapSceneCapture2D> sceneCapIt(GetWorld()); sceneCapIt; ++sceneCapIt)
	{
		mapSceneCapture = *sceneCapIt;
		break;
	}

	if (BackButton) BackButton->OnClicked.AddUniqueDynamic(this, &UMapWidget::GoBack);
}

void UMapWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), false);

	if (BackButton->OnClicked.IsBound()) BackButton->OnClicked.RemoveDynamic(this, &UMapWidget::GoBack);
	if (mapSceneCapture) mapSceneCapture = nullptr;
}

void UMapWidget::NativeTick(const FGeometry& Geometry, float DeltaTime)
{
	Super::NativeTick(Geometry, DeltaTime);

	// Map Zoom
	if (mapSceneCapture && GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::X))
	{
		mapSceneCapture->ZoomInMap(DeltaTime);
	}

	if (mapSceneCapture && GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Z))
	{
		mapSceneCapture->ZoomOutMap(DeltaTime);
	}

	// Map Movement
	if (mapSceneCapture && GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Up))
	{
		mapSceneCapture->MoveMap(EMapMovement::MoveUp, DeltaTime);
	}

	if (mapSceneCapture && GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Down))
	{
		mapSceneCapture->MoveMap(EMapMovement::MoveDown, DeltaTime);
	}

	if (mapSceneCapture && GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Right))
	{
		mapSceneCapture->MoveMap(EMapMovement::MoveRight, DeltaTime);
	}

	if (mapSceneCapture && GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::Left))
	{
		mapSceneCapture->MoveMap(EMapMovement::MoveLeft, DeltaTime);
	}
}

void UMapWidget::GoBack()
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		BackButton->OnClicked.RemoveDynamic(this, &UMapWidget::GoBack);

		if (UGameplayStatics::IsGamePaused(GetWorld())) UGameplayStatics::SetGamePaused(GetWorld(), false);

		if (mapSceneCapture) mapSceneCapture = nullptr;
		player->CloseMap();
	}
}