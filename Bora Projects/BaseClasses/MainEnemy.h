// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/RegularHealthComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/WidgetComponent.h"
#include "MainEnemy.generated.h"

UCLASS()
class SHROOMJAM2025_API AMainEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetEnemyDamaged(float newValue);

private:
	UPROPERTY(EditAnywhere)
	URegularHealthComponent* regularHealthComponent;

	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* spriteComponent;

protected:
	// Enemy widget
	UPROPERTY(EditAnywhere, Category = "Components")
	UWidgetComponent* widgetComponent;

	float detectionValue;

	bool widgetVisible;
};
