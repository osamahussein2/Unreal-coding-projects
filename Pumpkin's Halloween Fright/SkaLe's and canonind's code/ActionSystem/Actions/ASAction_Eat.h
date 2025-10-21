// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/ASAction_MontagePlay.h"
#include "ASAction_Eat.generated.h"

class UActionSystemComponent;
class USoundBase;
class UNiagaraSystem;

/**
 *
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UASAction_Eat : public UASAction_MontagePlay
{
	GENERATED_BODY()

public:
	virtual bool OnAnimNotifyReceived(UActionSystemComponent* ActionComp, FName EventName) override;

protected:
	UFUNCTION()
	void SpawnCandy(UActionSystemComponent* ActionComp);

	UFUNCTION()
	void EatCandy(UActionSystemComponent* ActionComp);

	void SpawnSound(const FVector& Location);
	void SpawnVisualEffect(const FVector& Location, const FRotator& Rotation);

protected:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AActor> CandyClass;

	UPROPERTY(EditAnywhere, Category = "Eat")
	FName SpawnSocket;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float Volume = 1.f;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float Pitch = 1.f;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float Scale = 1.f;


	TObjectPtr<AActor> CandyActor;
};
