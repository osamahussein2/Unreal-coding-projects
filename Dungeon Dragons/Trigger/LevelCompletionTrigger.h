// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelCompletionTrigger.generated.h"

class UBoxComponent;
class UPaperSpriteComponent;

UCLASS()
class DUNGEONCRAWLER2026_API ALevelCompletionTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelCompletionTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnLevelCompletionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> levelCompletionBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UPaperSpriteComponent> levelCompletionSprite = nullptr;

	UPROPERTY(EditAnywhere, Category = "Level Completion Parameters", meta = (ClampMin = "0"))
	int doorsUnlockedAmount = 0;
};
