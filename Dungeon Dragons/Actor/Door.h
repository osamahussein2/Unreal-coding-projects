// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UPaperSpriteComponent;

UENUM()
enum class EDoorUnlocked : uint8
{
	EDexterity = 0 UMETA(DisplayName = "Unlocked by Dexterity"),
	EStrength = 1 UMETA(DisplayName = "Unlocked by Strength"),
	EIntelligence = 2 UMETA(DisplayName = "Unlocked by Intelligence"),
	EEverySkill = 3 UMETA(DisplayName = "Unlocked by Every Skill")
};

UCLASS()
class DUNGEONCRAWLER2026_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	void StartOpeningDoor();

	EDoorUnlocked GetDoorUnlocked() const { return doorUnlocked; }
	int GetUnlockedAfterReachingSkill() const { return unlockedAfterReachingSkill; }

	bool GetDoorOpening() const { return doorOpening; }
	bool HasDoorAlreadyOpened() const { return doorSprite == nullptr; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OpenDoor();

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> doorScene;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> doorMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UPaperSpriteComponent> doorSprite;

	UPROPERTY(EditAnywhere, Category = "Door Parameters")
	EDoorUnlocked doorUnlocked = EDoorUnlocked::EDexterity;

	UPROPERTY(EditAnywhere, Category = "Door Parameters")
	int unlockedAfterReachingSkill = 10;

	UPROPERTY(EditAnywhere, Category = "Door Parameters")
	float doorOpenSpeed = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Door Parameters")
	float rotationValueToLetPlayerProceed = 3.0f;

	FTimerHandle openDoorHandle = {};

	float doorRotation = 0.0f;

	bool doorOpening = false;

	UPROPERTY(EditAnywhere, Category = "Door")
	TArray<UStaticMesh*> doorStateMesh; // 0 for locked, 1 for unlocked
};
