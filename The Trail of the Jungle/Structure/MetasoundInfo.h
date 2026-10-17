// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundInfo.generated.h"

class UMetaSoundSource;

USTRUCT()
struct FMetasoundInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMetaSoundSource> metasoundSource = TObjectPtr<UMetaSoundSource>();

	UPROPERTY(EditDefaultsOnly)
	float endTime = 0.0f;
};