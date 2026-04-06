// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Chest.generated.h"

class UBoxComponent;
class UPaperSpriteComponent;
class AHealthPotion;

UENUM()
enum class EChestItem : uint8
{
	EHealthPotion = 0 UMETA(DisplayName = "Health Potion"),
	ECombatPotion = 1 UMETA(DisplayName = "Combat Potion")
};

UCLASS()
class DUNGEONCRAWLER2026_API AChest : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact() override;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> chestMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UPaperSpriteComponent> chestSprite;

	UPROPERTY(EditDefaultsOnly, Category = "Chest Parameters")
	TArray<TSubclassOf<AHealthPotion>> healthPotions;

	UPROPERTY(EditAnywhere, Category = "Chest Parameters")
	FVector potionSpawnOffset = FVector(0.0f, 0.0f, 200.0f);

	UPROPERTY(EditAnywhere, Category = "Chest Parameters")
	FVector potionSpawnScale = FVector(1.0f, 1.0f, 1.0f);

	/*UPROPERTY(EditDefaultsOnly, Category = "Chest Parameters")
	TArray<TSubclassOf<ACombatPotion>> combatPotions;*/
};
