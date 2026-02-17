// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BodyActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSequenceSuccessful, int&, bodyIndex);

UCLASS()
class GOEDWARECOMMUNITYJAM_API ABodyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABodyActor();

	void BindOnSequenceSuccessful();

	UPROPERTY(VisibleAnywhere, Category = "Delegates")
	FOnSequenceSuccessful OnSequenceSuccessful;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void ResetSprite();

	UFUNCTION()
	void ShowBodyNervesSprite(int& bodyIndex_);

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	class UPaperSpriteComponent* spriteComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TArray<class UPaperSprite*> bodySprites;

	UPROPERTY(EditAnywhere, Category = "Wait Time Parameters")
	float timeToResetSprite = 2.0f;
};
