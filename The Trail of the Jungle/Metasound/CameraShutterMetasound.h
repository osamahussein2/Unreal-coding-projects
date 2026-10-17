// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structure/MetasoundInfo.h"
#include "CameraShutterMetasound.generated.h"

UCLASS()
class COMFYAUTUMNJAM_API ACameraShutterMetasound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraShutterMetasound();

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
	UPROPERTY(EditDefaultsOnly, Category = "Camera Shutter")
	TArray<FMetasoundInfo> cameraShutterSettings = TArray<FMetasoundInfo>();
};
