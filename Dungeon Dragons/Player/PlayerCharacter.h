// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "Player/PlayerStats.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UPlayerInputDA;
struct FInputActionValue;
class UCameraComponent;
class UActionComponent;
class USpringArmComponent;
class UPaperSpriteComponent;
class USceneCaptureComponent2D;
class AMapSceneCapture2D;
class UHealthComponent;

UCLASS()
class DUNGEONCRAWLER2026_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CloseMap();

	void UpdatePlayerHUD();
	void UpdateStatsHUD();

	UHealthComponent* GetHealthComponent() { return healthComponent; }

	void ResetPlayerValues();

	bool IsPlayerDead() const;

	// Dice functions
	int GetDiceRollNumber() const { return dieRoll; }

	// Player stat getters
	int GetPlayerDexterity() const { return playerStats.dexterity; }
	int GetPlayerStrength() const { return playerStats.strength; }
	int GetPlayerIntelligence() const { return playerStats.intelligence; }

	// Player stat incrementing
	void IncrementPlayerDexterity(const int& value_);
	void IncrementPlayerStrength(const int& value_);
	void IncrementPlayerIntelligence(const int& value_);

	// Check if player is near the door
	bool IsDoorNearby(AActor* doorActor_);

	// Get doors unlocked
	int GetDoorsUnlocked() const { return doorsUnlocked; }
	void IncrementDoorsUnlocked();

	// Defeated enemies
	/*int GetEnemiesToDefeatInRoom() const;
	void IncrementEnemiesDefeatedInRoom();*/

	// Inventory - health potions functions
	int GetCollectedSmallHealthPotions() const { return collectedSmallHealthPotions; }
	int GetCollectedMediumHealthPotions() const { return collectedMediumHealthPotions; }
	int GetCollectedLargeHealthPotions() const { return collectedLargeHealthPotions; }

	void IncrementCollectedSmallHealthPotion();
	void IncrementCollectedMediumHealthPotion();
	void IncrementCollectedLargeHealthPotion();

	void HealWithSmallHealthPotion();
	void HealWithMediumHealthPotion();
	void HealWithLargeHealthPotion();

	void CloseInventory();

private:
	// Input functions
	void MoveCharacter(const FInputActionValue& value_);
	void MoveCamera(const FInputActionValue& value_);

	void PauseGame();

	void OpenMap();
	void OpenInventory();

	void RollDie();

	void Interact();
	void AttackEnemy();

	// Check if player can move to another floor
	void MoveToClosestAheadOfPlayer();
	void MoveToClosestFloorBehindThePlayer();
	void MoveToClosestFloorToLeft();
	void MoveToClosestFloorToRight();

	bool WithinTheRangeOfNextFloor(AActor* floorActor_);

	UFUNCTION()
	void EnableMovement();

	UFUNCTION()
	void UnblockAttackAction();

	UFUNCTION()
	void EnableInteractionWithChest();

	UFUNCTION()
	void UpdateHealthHUD(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType);

	UFUNCTION()
	void DisablePlayerInput();

	void ShowGameOverScreen();

	// Dice functions
	UFUNCTION()
	void UpdateDiceRoll();

	UFUNCTION()
	void LandOnDieNumber();

	void EnableDieRoll();

	// Footstep sounds
	void PlayFootstepSounds();

	// Check if player is near the chest
	bool IsChestNearby(AActor* doorActor_);

	bool IsEnemyNearby(AActor* enemyActor_); // Player near an enemy
	bool IsEnemyNearby(AActor* floorActor_, AActor* enemyActor_); // Enemy near a floor

	// Iterate through Blood Drinker enemy actor
	void DealDamageToBloodDrinker();

private:
	// Components
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> cameraComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UActionComponent> actionComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<UHealthComponent> healthComponent = nullptr;

	// Minimap for the player icon
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> minimapSpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneCaptureComponent2D> minimapCaptureComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperSpriteComponent> spriteMinimapComponent;

	// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> inputMappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UPlayerInputDA> playerInputDA = nullptr;

	UPROPERTY(EditAnywhere, Category = "Distance to Floor", meta = (ClampMin = "0.1"))
	float maxDistanceToNextFloor = 550.0f;

	UPROPERTY(EditAnywhere, Category = "Distance to Floor", meta = (ClampMin = "0.1"))
	float minDistanceToNextFloor = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Distance to Floor", meta = (ClampMin = "0.1"))
	float enemyAbsDistanceToFloor = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Distance to Floor", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float dotProductValue = 0.6f;

	UPROPERTY(EditAnywhere, Category = "Player Actions")
	FGameplayTag movingAction;

	UPROPERTY(EditAnywhere, Category = "Player Actions")
	FGameplayTag rollingDiceAction;

	UPROPERTY(EditAnywhere, Category = "Player Actions")
	FGameplayTag attackAction;

	UPROPERTY(EditAnywhere, Category = "Player Actions")
	FGameplayTag collectFromChestAction;

	UPROPERTY(EditAnywhere, Category = "Player Actions")
	float movementCooldownTime = 0.15f;

	UPROPERTY(EditAnywhere, Category = "Player Actions")
	float attackCooldownTime = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Player Actions")
	float chestInteractionCooldownTime = 3.0f;

	float cameraYawValue = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Map Camera")
	TSubclassOf<AMapSceneCapture2D> fullMapSceneCaptureClass;

	UPROPERTY()
	AMapSceneCapture2D* fullMapSceneCapture = nullptr;

	UPROPERTY(EditAnywhere, Category = "Map Camera")
	FVector mapCameraSpawnOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Wait Times", meta = (ClampMin = "0.01"))
	float timeToShowGameOver = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Wait Times", meta = (ClampMin = "0.01"))
	float timeToLandOnDieNumber = 2.0f;

	// Dice
	UPROPERTY(VisibleAnywhere, Category = "Dice")
	int dieRoll = 1;

	FTimerHandle rollDiceHandle = {}, landOnDie = {};

	// Player Skills/Stats
	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	FPlayerStats playerStats = {};

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int increaseDexterityValue = 2;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int increaseStrengthValue = 2;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int increaseIntelligenceValue = 2;

	UPROPERTY(EditAnywhere, Category = "Distance to Door", meta = (ClampMin = "0.1"))
	float maxDistanceToDoor = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Distance to Door", meta = (ClampMin = "0.1"))
	float minDistanceToDoor = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Distance to Chest", meta = (ClampMin = "0.1"))
	float maxDistanceToChest = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Distance to Chest", meta = (ClampMin = "0.1"))
	float minDistanceToChest = 0.5f;

	UPROPERTY(VisibleAnywhere, Category = "Doors Unlocked")
	int doorsUnlocked = 0;

	UPROPERTY(EditAnywhere, Category = "Sound Effects")
	TArray<USoundBase*> footstepSounds;

	UPROPERTY(EditAnywhere, Category = "Sound Effects")
	TObjectPtr<USoundBase> healSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Actor Tags")
	FName enemyTag = "Enemy";

	UPROPERTY(EditAnywhere, Category = "Enemy Damaged Values")
	float damagedValueForBloodDrinker = 20.0f;

	// Enemies defeated
	/*UPROPERTY(EditAnywhere, Category = "Enemies Defeated")
	TArray<int> enemiesToDefeatInRoom;

	UPROPERTY(VisibleAnywhere, Category = "Enemies Defeated")
	int enemiesDefeatedInRoom = 0;

	UPROPERTY(VisibleAnywhere, Category = "Enemies Defeated")
	int enemiesDefeatedOnFloor = 0;*/

	// Inventory - collected health potions
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int collectedSmallHealthPotions = 0;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int collectedMediumHealthPotions = 0;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int collectedLargeHealthPotions = 0;
};
