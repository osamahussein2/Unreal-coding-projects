// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickNerveState.generated.h"

UENUM()
enum class EPickNerveState : uint8
{
	ENone,
	EPickingThree,
	EPickingFour,
	EPickingFive
};