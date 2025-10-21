#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LootSplitter.generated.h"

class USceneComponent;
class ACandyCollectible;

UCLASS()
class GDTV_HALLOWEEN2025_API ALootSplitter : public AActor
{
    GENERATED_BODY()

public:
    ALootSplitter();

    /** Fire all muzzles (called by PuzzleManager on puzzle success). */
    UFUNCTION(BlueprintCallable, Category="Loot")
    void Spit();

    UPROPERTY(EditAnywhere, Category="Loot")
    bool bAllowRepeatSpit = false;
    
protected:
    virtual void BeginPlay() override;

    /** Actor class to spawn (ACandyCollectible). */
    UPROPERTY(EditAnywhere, Category="Loot")
    TSubclassOf<ACandyCollectible> CandyClass;

    /** Where candies should exit. Add any number of children scene components. */
    UPROPERTY(EditAnywhere, Category="Loot")
    TArray<FComponentReference> Muzzles;

    /** Desired landing distance in front of the muzzle (cm). */
    UPROPERTY(EditAnywhere, Category="Loot|Ballistics")
    float DesiredRange = 250.f;

    /** Sideways random spread (cm) around the forward path on landing. */
    UPROPERTY(EditAnywhere, Category="Loot|Ballistics")
    float LateralSpread = 60.f;

    /** Slight upward bias on landing point (cm). */
    UPROPERTY(EditAnywhere, Category="Loot|Ballistics")
    float LandingZBias = 0.f;

    /** Time the projectile should spend in the air (sec). */
    UPROPERTY(EditAnywhere, Category="Loot|Ballistics", meta=(ClampMin="0.1"))
    float FlightTime = 0.7f;

    /** Random time offset between candies for nicer staggering (sec). */
    UPROPERTY(EditAnywhere, Category="Loot")
    FVector2D SpawnDelayRange = FVector2D(0.0f, 0.15f);

    /** How many candies per muzzle. */
    UPROPERTY(EditAnywhere, Category="Loot", meta=(ClampMin="1"))
    int32 CandiesPerMuzzle = 2;

private:
    /** Compute initial velocity to reach Target in FlightTime, with world gravity. */
    bool ComputeBallisticVelocity(const FVector& Start, const FVector& Target, float Time, FVector& OutVelocity) const;

    /** Spawn one candy from a muzzle. */
    void SpawnOne(USceneComponent* Muzzle);

    /** Handles staggering the spawns. */
    void KickStagger();

    // Private: resolved at runtime
    TArray<TWeakObjectPtr<USceneComponent>> ResolvedMuzzles;

    FTimerHandle StaggerTimerHandle;
    int32 PendingMuzzleIndex = 0;
    int32 PendingPerMuzzle = 0;

    UPROPERTY(Transient)
    bool bHasSpit = false;
};
