// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"
#include "DefaultPlayerController.h"
#include "DefaultPlayer.h"
#include "Kismet/GameplayStatics.h"

ADefaultGameMode::ADefaultGameMode()
{
	DefaultPawnClass = ADefaultPlayer::StaticClass();
	PlayerControllerClass = ADefaultPlayerController::StaticClass();

}

void ADefaultGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ambienceSound) UGameplayStatics::PlaySound2D(GetWorld(), ambienceSound);
	if (gameMusic) UGameplayStatics::PlaySound2D(GetWorld(), gameMusic);
}