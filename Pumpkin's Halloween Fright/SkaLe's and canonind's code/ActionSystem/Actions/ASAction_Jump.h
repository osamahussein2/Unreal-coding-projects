// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/ASAction.h"
#include "ASAction_Jump.generated.h"

class ACharacter;

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UASAction_Jump : public UASAction
{
	GENERATED_BODY()

public:
	virtual void StartAction(UActionSystemComponent* ActionComp) override;
	virtual void StopAction(UActionSystemComponent* ActionComp) override;
private:
	void PerformJump(UActionSystemComponent* ActionComp);
	UFUNCTION()
	void EndJump(class ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);

private:
	UPROPERTY()
	UActionSystemComponent* OwningActionComp = nullptr;
};
