// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArcadeMachine.generated.h"

class UBoxComponent;
class ACoin;

UCLASS()
class SHROOMJAM2026_API AArcadeMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArcadeMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetPlayerInTrigger() const { return playerInTrigger; }

	TArray<TObjectPtr<ACoin>> GetCoins() const { return coins; }

	void RemoveCoinFromArray(ACoin* coinToRemove_);

	void StopFlashing();

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> arcadeMachine = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> boxComponent = nullptr;

	bool playerInTrigger = false;

	UPROPERTY(EditAnywhere, Category = "Coins")
	TArray<TObjectPtr<ACoin>> coins = TArray<TObjectPtr<ACoin>>();

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> matInstanceDynamic = nullptr;

	UPROPERTY(EditAnywhere, Category = "Material Settings")
	FLinearColor playerInTriggerColor = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Material Settings")
	float lerpSpeed = 3.0f;

	bool colorLerpChanged = false;

	float timer = 0.0f;
};
