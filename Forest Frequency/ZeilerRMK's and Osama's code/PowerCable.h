// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Structs/CableConnection.h"
#include "PowerCable.generated.h"

class UCableComponent;
class APowerConnector;

UCLASS()
class GAMEDEVTVGAMEJAM2026_API APowerCable : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerCable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Cable")
	TObjectPtr<UCableComponent> CableComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Cable")
	bool bIsBeingHeld = false;

	UPROPERTY(VisibleAnywhere, Category = "Cable")
	ADefaultPlayer* Holder = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Connetions")
	FCableConnection ConnectionA;

	UPROPERTY(VisibleAnywhere, Category = "Connetions")
	FCableConnection ConnectionB;

	UPROPERTY()
	TObjectPtr<USceneComponent> SceneRoot;

public:	

	void ConnectTo(APowerConnector* Connector);

	void UpdateCableVisual();

	bool HasOnlyConnectionA();

	bool HasFreeConnection();

	bool HasOnlyConnectionB();

	bool IsFullyConnected();

	void TryConnect(AActor* Target);

	bool IsHitByTrace(const FVector& TraceStart, const FVector& TraceEnd, float Radius, float& OutDistance);

	virtual bool CanInteract_Implementation(ADefaultPlayer* Interactor) const override;

	virtual void Interact_Implementation(ADefaultPlayer* Interactor) override;
};
