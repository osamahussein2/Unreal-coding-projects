// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structure/MetasoundInfo.h"
#include "FilmPickupMetasound.generated.h"

UCLASS()
class COMFYAUTUMNJAM_API AFilmPickupMetasound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFilmPickupMetasound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void DestroyActor();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Film Pickup")
	TArray<FMetasoundInfo> filmPickupSettings = TArray<FMetasoundInfo>();
};
