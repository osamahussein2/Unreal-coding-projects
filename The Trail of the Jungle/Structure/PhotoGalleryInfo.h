// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhotoGalleryInfo.generated.h"

USTRUCT()
struct FPhotoGalleryInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> photoGalleryWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Snapshot")
	int maxSnapshots = 2;
};
