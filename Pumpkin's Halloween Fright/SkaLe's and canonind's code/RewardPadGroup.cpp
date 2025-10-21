#include "RewardPadGroup.h"
#include "RandomRewardPad.h"
#include "CandyCollectible.h"

ARewardPadGroup::ARewardPadGroup()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARewardPadGroup::BeginPlay()
{
	Super::BeginPlay();

	// Ensure each pad points back to this group (if not set manually)
	for (ARandomRewardPad* Pad : Pads)
	{
		if (Pad) { Pad->Group = this; }
	}
}

void ARewardPadGroup::NotifyPadStepped(ARandomRewardPad* Pad)
{
	if (bSolved || !Pad) return;
	SteppedPads.Add(Pad);
	TryFinish();
}

void ARewardPadGroup::RegisterCandy(ACandyCollectible* Candy)
{
	if (!Candy || bSolved) return;
	++OutstandingCandies;
	Candy->OnDestroyed.AddDynamic(this, &ARewardPadGroup::OnCandyDestroyed);
}

void ARewardPadGroup::RegisterEnemy(AActor* Enemy)
{
	if (!Enemy || bSolved) return;
	++OutstandingEnemies;
	Enemy->OnDestroyed.AddDynamic(this, &ARewardPadGroup::OnEnemyDestroyed);
}

void ARewardPadGroup::OnCandyDestroyed(AActor* /*Destroyed*/)
{
	OutstandingCandies = FMath::Max(0, OutstandingCandies - 1);
	TryFinish();
}

void ARewardPadGroup::OnEnemyDestroyed(AActor* /*Destroyed*/)
{
	OutstandingEnemies = FMath::Max(0, OutstandingEnemies - 1);
	TryFinish();
}

void ARewardPadGroup::TryFinish()
{
	const int32 TotalPads = Pads.Num();
	const bool bAllPadsStepped = (TotalPads > 0 && SteppedPads.Num() >= TotalPads);
	const bool bNoCandiesLeft  = (OutstandingCandies == 0);
	const bool bNoEnemiesLeft  = (OutstandingEnemies == 0);

	UE_LOG(LogTemp, Warning, TEXT("[RewardPadGroup %d] Pads %d/%d, Candies %d, Enemies %d, bSolved=%d"),
		PuzzleId, SteppedPads.Num(), TotalPads, OutstandingCandies, OutstandingEnemies, bSolved);

	if (!bSolved && bAllPadsStepped && bNoCandiesLeft && bNoEnemiesLeft)
	{
		bSolved = true;
		UE_LOG(LogTemp, Error, TEXT("[RewardPadGroup %d] Puzzle solved! Broadcasting!"), PuzzleId);
		OnPuzzleSolved.Broadcast(PuzzleId);
	}
}


void ARewardPadGroup::ResetGroup()
{
	bSolved = false;
	SteppedPads.Empty();
	OutstandingCandies = 0;
	OutstandingEnemies = 0;
}
