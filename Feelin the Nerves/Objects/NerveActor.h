// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NerveActor.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSequenceSuccessful, int, correctSequences, int, bodyIndex);

UCLASS()
class GOEDWARECOMMUNITYJAM_API ANerveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANerveActor();

	/*UPROPERTY(VisibleAnywhere, Category = "Delegates")
	FOnSequenceSuccessful OnSequenceSuccessful;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	/*UFUNCTION()
	void SetNerveSprite(int correctSequences_, int index_);*/

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	class UPaperSpriteComponent* spriteComponent = nullptr;

	/*UPROPERTY(EditAnywhere, Category = "Components")
	TArray<class UPaperSprite*> blueNerveSprites;

	UPROPERTY(EditAnywhere, Category = "Components")
	TArray<class UPaperSprite*> orangeNerveSprites;

	UPROPERTY(EditAnywhere, Category = "Components")
	TArray<class UPaperSprite*> purpleNerveSprites;

	UPROPERTY(EditAnywhere, Category = "Components")
	TArray<class UPaperSprite*> yellowNerveSprites;

	UPROPERTY(EditAnywhere, Category = "Transform Parameters")
	FVector nerveScale = FVector::OneVector;*/
};
