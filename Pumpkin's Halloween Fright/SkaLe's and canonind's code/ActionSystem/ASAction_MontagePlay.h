// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/ASAction.h"
#include "ASAction_MontagePlay.generated.h"

class UActionSystemComponent;

/**
 * 
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UASAction_MontagePlay : public UASAction
{
	GENERATED_BODY()

public:
	virtual bool OnAnimNotifyReceived(UActionSystemComponent* ActionComp, FName EventName) override;

	virtual void StartAction(UActionSystemComponent* ActionComp) override;
	virtual void StopAction(UActionSystemComponent* ActionComp) override;
	virtual void RestartAction(UActionSystemComponent* ActionComp) override;

protected:
	UFUNCTION()
	virtual void OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY()
	UActionSystemComponent* OwningActionComp = nullptr;

	UPROPERTY(EditAnywhere, Category = "Action Montage")
	int32 SectionIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Action Montage")
	float MontageStopBlendTime = 0.2f;
};
