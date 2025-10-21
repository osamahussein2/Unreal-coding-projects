#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewardPadGroup.generated.h"

class ARandomRewardPad;
class ACandyCollectible;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPuzzleGroupSolved, int32, PuzzleId);

UCLASS()
class GDTV_HALLOWEEN2025_API ARewardPadGroup : public AActor
{
	GENERATED_BODY()

public:
	ARewardPadGroup();

	/** Which puzzle id this group represents (set to 2 in editor). */
	UPROPERTY(EditAnywhere, Category="Puzzle")
	int32 PuzzleId = 2;

	/** All pads that belong to this group (assign in editor). */
	UPROPERTY(EditInstanceOnly, Category="Puzzle")
	TArray<ARandomRewardPad*> Pads;

	/** Broadcast once when the puzzle finishes. */
	UPROPERTY(BlueprintAssignable, Category="Puzzle")
	FOnPuzzleGroupSolved OnPuzzleSolved;

	/** Called by pads when stepped (first touch counts). */
	UFUNCTION(BlueprintCallable) void NotifyPadStepped(ARandomRewardPad* Pad);

	/** Called when a pad (or LootSpitter) spawns a candy. */
	UFUNCTION(BlueprintCallable) void RegisterCandy(ACandyCollectible* Candy);

	/** Called when a pad spawns an enemy. */
	UFUNCTION(BlueprintCallable) void RegisterEnemy(AActor* Enemy);

	/** Optional: reset for testing. */
	UFUNCTION(BlueprintCallable) void ResetGroup();

	/** Simple getter for plates/pads to ignore if already solved. */
	UFUNCTION(BlueprintPure) bool IsSolved() const { return bSolved; }

protected:
	virtual void BeginPlay() override;

private:
	// Runtime state
	UPROPERTY() TSet<TWeakObjectPtr<ARandomRewardPad>> SteppedPads;
	UPROPERTY() int32 OutstandingCandies = 0;
	UPROPERTY() int32 OutstandingEnemies = 0;
	UPROPERTY() bool bSolved = false;

	UFUNCTION() void OnCandyDestroyed(AActor* Destroyed);
	UFUNCTION() void OnEnemyDestroyed(AActor* Destroyed);

	void TryFinish();
};
