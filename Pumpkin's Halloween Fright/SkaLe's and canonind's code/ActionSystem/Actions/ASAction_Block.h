// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/ASAction.h"
#include "ASAction_Block.generated.h"

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UASAction_Block : public UASAction
{
	GENERATED_BODY()
	
public:
	virtual void StartAction(UActionSystemComponent* ActionComp) override;
	virtual void StopAction(UActionSystemComponent* ActionComp) override;

private:
	UPROPERTY(EditAnywhere, Category = "Block")
	float DamageReductionFactor = 0.2f;

	/* Set max block time to prevent constant blocking. Value 0 disables restriction */
	UPROPERTY(EditAnywhere, Category = "Block")
	float BlockMaxTime = 3.f; 

	FTimerHandle StopBlock_TimerHandle;
};
