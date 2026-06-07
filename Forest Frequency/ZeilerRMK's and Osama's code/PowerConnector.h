// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConnectionInterface.h"
#include "PowerConnector.generated.h"

class APowerCable;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class GAMEDEVTVGAMEJAM2026_API APowerConnector : public AActor, public IConnectionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerConnector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot = nullptr;

	//Conection variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Connection")
	bool bIsPowered = false;

	UPROPERTY(EditDefaultsOnly, Category = "Connection")
	bool bCanProvidePower = false;

public:
	UPROPERTY(VisibleAnywhere, Category = "Connection")
	TObjectPtr<APowerCable> ConnectedCable = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> ConnectionPoint = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Connection")
	FName ConnectionSocketName = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Connection")
	bool bIsOccupied = false;

public:

	virtual bool CanConnectCable_Implementation(APowerCable* Cable) const override;

	virtual void ConnectCable_Implementation(APowerCable* Cable) override;

	virtual void DisconnectCable_Implementation(APowerCable* Cable) override;

};
