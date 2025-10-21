#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StonePlateGroup.generated.h"

// --- Delegates so UI or other systems can listen ---
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWindowStarted, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWindowTick, float, TimeRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWindowEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleSolved, int32, PuzzleId);

class AStoneplate;

/**
 * Coordinates multiple Stoneplates so they share one active timer.
 * When any plate is stepped on, the group starts a shared window.
 * All plates lit during that window turn off together when it ends.
 */
UCLASS()
class GDTV_HALLOWEEN2025_API AStoneplateGroup : public AActor
{
	GENERATED_BODY()

public:
	AStoneplateGroup();

	// Called by plates when stepped
	void NotifyPlateStepped(AStoneplate* Plate);

	// --- UI helper functions ---
	UFUNCTION(BlueprintPure, Category="Stoneplate Group")
	bool IsWindowActive() const { return bWindowActive; }

	UFUNCTION(BlueprintPure, Category="Stoneplate Group")
	float GetTimeRemaining() const;

	UFUNCTION(BlueprintPure, Category="Stoneplate Group")
	float GetWindowDuration() const { return ActiveWindowSeconds; }

	// --- Blueprint events for UI ---
	UPROPERTY(BlueprintAssignable, Category="Stoneplate Group|Events")
	FOnWindowStarted OnWindowStarted;

	UPROPERTY(BlueprintAssignable, Category="Stoneplate Group|Events")
	FOnWindowTick OnWindowTick;

	UPROPERTY(BlueprintAssignable, Category="Stoneplate Group|Events")
	FOnWindowEnded OnWindowEnded;

	UPROPERTY(EditAnywhere, Category="Puzzle") int32 PuzzleId = 1; // set per group
	UPROPERTY(EditAnywhere, Category="Puzzle") int32 RequiredCount = 4;

	UPROPERTY(EditAnywhere, Category="Puzzle|Events")
	FOnPuzzleSolved OnPuzzleSolved;

	bool IsSolved() const { return bSolved; }
	

protected:
	virtual void BeginPlay() override;

	// How long the shared window lasts
	UPROPERTY(EditAnywhere, Category="Stoneplate Group", meta=(ClampMin="0.0"))
	float ActiveWindowSeconds = 3.0f;

	bool bSolved = false;

private:
	// Internal helpers
	void StartWindow();
	void EndWindow();
	void TickWindow(); // updates UI events

	FTimerHandle WindowTimerHandle;      // ends window
	FTimerHandle WindowTickTimerHandle;  // periodic UI tick

	bool bWindowActive = false;
	TSet<TWeakObjectPtr<AStoneplate>> ActivePlates;
};
