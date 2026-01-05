// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "CreditsBillboard.generated.h"

UCLASS()
class GOEDWARECUTSCENE2JAM_API ACreditsBillboard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreditsBillboard();

	void ConstructCreditsWidget();
	void DestroyCreditsWidget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> billboardHolder;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> billboardSign;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UWidgetComponent> billboardWidgetComponent;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> creditsWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Billboard Widget Transform")
	FVector widgetComponentSpawnLocation = FVector(-73.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Billboard Widget Transform")
	FRotator widgetComponentSpawnRotation = FRotator(0.0f, 180.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Billboard Widget Transform")
	FVector widgetComponentSpawnScale = FVector(1.0f, 0.3f, 0.6f);

	UPROPERTY(EditAnywhere, Category = "Billboard Widget Transform")
	FVector2D widgetComponentDrawSize = FVector2D(500.0f, 150.0f);
};
