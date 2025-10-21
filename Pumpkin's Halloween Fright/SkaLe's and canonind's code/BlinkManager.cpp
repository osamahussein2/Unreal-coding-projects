#include "BlinkManager.h"
#include "GuideBeacon.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

ABlinkManager::ABlinkManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABlinkManager::BeginPlay()
{
	Super::BeginPlay();
}

TArray<AGuideBeacon*>* ABlinkManager::FindGroup(int32 RoadId)
{
	for (FRoadBlinkGroup& G : RoadGroups)
	{
		if (G.RoadId == RoadId) return &G.Beacons;
	}
	return nullptr;
}

void ABlinkManager::StartBlink(int32 RoadId, float Duration)
{
	// Stop any current blinking
	StopCurrent();

	if (TArray<AGuideBeacon*>* Group = FindGroup(RoadId))
	{
		for (AGuideBeacon* B : *Group)
		{
			if (B) B->SetBlinking(true);
		}

		if (Duration > 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(
				StopTimer, this, &ABlinkManager::StopCurrent, Duration, false);
		}
	}
}

void ABlinkManager::StopCurrent()
{
	GetWorld()->GetTimerManager().ClearTimer(StopTimer);
	// Turn off every beacon in all groups
	for (FRoadBlinkGroup& G : RoadGroups)
		for (AGuideBeacon* B : G.Beacons)
			if (B) B->SetBlinking(false);
}

void ABlinkManager::StopAll()
{
	StopCurrent();
}

void ABlinkManager::SortBeaconsAlong(TArray<AGuideBeacon*>& Beacons, const FVector& DirOrZero)
{
	if (Beacons.Num() <= 1) return;

	// Choose an axis: if DirectionHint is zero, default to world X (left→right).
	FVector Dir = DirOrZero.IsNearlyZero() ? FVector(1,0,0) : DirOrZero.GetSafeNormal();

	// Project positions onto Dir and sort ascending
	Beacons.StableSort([&](const AGuideBeacon& A, const AGuideBeacon& B){
		const FVector PA = A.GetActorLocation();
		const FVector PB = B.GetActorLocation();
		const float a = FVector::DotProduct(PA, Dir);
		const float b = FVector::DotProduct(PB, Dir);
		return a < b;
	});
}

// BlinkManager.cpp
//void ABlinkManager::StartBlinkSequenced(int32 RoadId, float Duration /*seconds*/, float BlinkHz /*=2.0f*/)
void ABlinkManager::StartBlinkSequenced(int32 RoadId, float Duration, float BlinkHz /*=2.0f*/, bool bReverse /*=false*/)
{
    // Stop any current blinking
    StopCurrent();

    // Find the beacon group for this road
    if (TArray<AGuideBeacon*>* Group = FindGroup(RoadId))
    {
        // Copy and clean up any null entries
        TArray<AGuideBeacon*> Beacons;
        Beacons.Reserve(Group->Num());
        for (AGuideBeacon* B : *Group)
        {
            if (B)
            {
                Beacons.Add(B);
            }
        }
        if (Beacons.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("BlinkManager: No valid beacons for Road %d"), RoadId);
            return;
        }

        // Sort by PairIndex so left/right pumpkins blink together
        Beacons.StableSort([](const AGuideBeacon& A, const AGuideBeacon& B)
        {
            return A.PairIndex < B.PairIndex;
        });

        // Reverse order if needed
        if (bReverse)
        {
            Algo::Reverse(Beacons);
        }

        // Determine the number of distinct pairs
        int32 MaxPair = -1;
        for (AGuideBeacon* B : Beacons)
        {
            MaxPair = FMath::Max(MaxPair, B->PairIndex);
        }
        const int32 NumPairs = FMath::Max(1, MaxPair + 1);

        // Common blink period (derived from frequency)
        const float PeriodSec = 1.0f / FMath::Max(0.01f, BlinkHz);

        // Apply phase offset per pair so both pumpkins in each pair blink together
        for (AGuideBeacon* B : Beacons)
        {
            if (!B) continue;

            // Get order (reversed or normal)
            int32 PairIdx = FMath::Clamp(B->PairIndex, 0, NumPairs - 1);
            if (bReverse)
            {
                PairIdx = (NumPairs - 1) - PairIdx;
            }

            // Convert to normalized 0..1 phase and then to time offset (seconds)
            const float Phase01 = float(PairIdx) / float(NumPairs);
            const float PhaseSec = Phase01 * PeriodSec;

            // Send data to beacon
            B->SetBlinkSpeed(BlinkHz);
            B->SetPhaseOffset(PhaseSec);
            B->SetBlinking(true);
        	
            UE_LOG(LogTemp, Log, TEXT("[BlinkManager] Road %d | Pair %d | Phase01=%.3f | PhaseSec=%.3f"),
                RoadId, PairIdx, Phase01, PhaseSec);

        }

        // Optional: Stop blinking automatically after duration
        if (Duration > 0.f)
        {
            GetWorld()->GetTimerManager().SetTimer(
                StopTimer, this, &ABlinkManager::StopCurrent, Duration, false);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BlinkManager: Road group %d not found"), RoadId);
    }
}