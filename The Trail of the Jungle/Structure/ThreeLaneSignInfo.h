// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/ThreeLaneSign.h"
#include "ThreeLaneSignInfo.generated.h"

USTRUCT()
struct FThreeLaneSignInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<AThreeLaneSign> threeLaneSign = nullptr;

	UPROPERTY(EditAnywhere)
	EThreeLaneSignStatus threeLaneSignStatusForUpdating = EThreeLaneSignStatus::ENotTakenPictureOfIt;
};
