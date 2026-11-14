// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueDataTable.generated.h"

USTRUCT(BlueprintType)
struct SHROOMJAM2026_API FDialogueDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Line = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float typeSpeed = 0.0f;
};