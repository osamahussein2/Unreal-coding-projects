#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleManager.generated.h"

class ABlinkManager;
class AStoneplateGroup;
class ARewardPadGroup;

USTRUCT(BlueprintType)
struct FPuzzleRouting
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) int32 PuzzleId = 1; // 1..N
	UPROPERTY(EditAnywhere) int32 RoadIdToBlink = 1;
	UPROPERTY(EditAnywhere) float BlinkDuration = 3.0f;
};

USTRUCT(BlueprintType)
struct FPrizeRoute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) int32 PuzzleId = 1;
	UPROPERTY(EditAnywhere) TArray<class ALootSplitter*> Spitters;
};

UCLASS()
class GDTV_HALLOWEEN2025_API APuzzleManager : public AActor
{
	GENERATED_BODY()
public:
	APuzzleManager();

	UFUNCTION(BlueprintCallable) void OnPuzzleSolved(int32 PuzzleId);

	UFUNCTION(CallInEditor) void Debug_ResetP2() { SolvedPuzzles.Remove(2); }
	UFUNCTION(CallInEditor) void Debug_SolveP2() { SolvedPuzzles.Remove(2); OnPuzzleSolved(2); }


	UFUNCTION(CallInEditor) void Debug_ResetP1() { SolvedPuzzles.Remove(1); }
	UFUNCTION(CallInEditor) void Debug_SolveP1() { SolvedPuzzles.Remove(1); OnPuzzleSolved(1); }

	UPROPERTY(EditInstanceOnly, Category="Puzzle 3")
	class ARockGate* RockGate = nullptr;


protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Configure in editor
	UPROPERTY(EditAnywhere, Category="PuzzleManager") TArray<FPuzzleRouting> Routes;
	UPROPERTY(EditAnywhere, Category="PuzzleManager") ABlinkManager* BlinkManager = nullptr;

	UPROPERTY(EditAnywhere, Category="Prize")
	TArray<FPrizeRoute> PrizeRoutes;

	UPROPERTY()
	TSet<int32> SolvedPuzzles;

private:
	const FPuzzleRouting* FindRoute(int32 PuzzleId) const;
	UFUNCTION() void HandleGroupPuzzleSolved(int32 PuzzleId);
	UPROPERTY() TArray<AStoneplateGroup*> BoundGroups;
};
