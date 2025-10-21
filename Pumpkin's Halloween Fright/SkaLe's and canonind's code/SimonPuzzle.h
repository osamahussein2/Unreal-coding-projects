#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimonPuzzle.generated.h"

class UBoxComponent;
class ASimonMonolith;
class ACandyCollectible;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSimonSolved, int32, PuzzleId);

UENUM()
enum class ESimonState : uint8 { Idle, Showing, AwaitInput, Solved };

UCLASS()
class GDTV_HALLOWEEN2025_API ASimonPuzzle : public AActor
{
    GENERATED_BODY()
public:
    ASimonPuzzle();

    /** Blue activation pad */
    UPROPERTY(VisibleAnywhere) UBoxComponent* ActivationPad;

    /** Order-mapped monoliths (size N). The pad with Index i maps to Monoliths[i]. */
    UPROPERTY(EditInstanceOnly, Category="Simon") TArray<ASimonMonolith*> Monoliths;

    /** All red step pads (set their Index to match Monoliths) */
    UPROPERTY(EditInstanceOnly, Category="Simon") TArray<class ASimonStepPad*> StepPads;

    /** Fallback candy class (used when no spitter) and enemy */
    UPROPERTY(EditAnywhere, Category="Spawns") TSubclassOf<ACandyCollectible> CandyClass;
    UPROPERTY(EditAnywhere, Category="Spawns") TSubclassOf<AActor> EnemyClass;

    /** Puzzle tuning */
    UPROPERTY(EditAnywhere, Category="Simon") int32 SequenceLength = 5;      // how many steps to show
    UPROPERTY(EditAnywhere, Category="Simon") float ShowOnTime = 0.6f;       // monolith lit duration
    UPROPERTY(EditAnywhere, Category="Simon") float ShowOffGap = 0.25f;      // gap between steps

    UPROPERTY(EditAnywhere, Category="Simon") int32 PuzzleId = 3;

    /** Broadcast when finished successfully */
    UPROPERTY(BlueprintAssignable, Category="Simon") FOnSimonSolved OnPuzzleSolved;

    /** Called by red pads */
    void OnPlayerStep(int32 PadIndex);

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleActivation(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

private:
    double NextAllowedInputTime = 0.0;

    UPROPERTY(EditAnywhere, Category="Simon")
    float WrongInputLockout = 0.50f; // seconds to ignore new inputs after a mistake

    ESimonState State = ESimonState::Idle;
    TArray<int32> Sequence;
    int32 Expect = 0;
    FTimerHandle ShowTimer;
    int32 ShowIdx = -1;

    void GenerateSequence();
    void StartShowing();
    void ShowTick();
    void AllOff();
    void SpawnCandyAt(int32 Index);
    void SpawnEnemyAt(int32 Index);
    void LockMonolith(int32 Index);
};
