#include "PuzzleManager.h"
#include "LootSplitter.h"
#include "BlinkManager.h"
#include "AssetClasses/StonePlateGroup.h"
#include "EngineUtils.h"        // TActorIterator
#include "SimonPuzzle.h"
#include "RockGate.h"
#include "RewardPadGroup.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Core/HJGameModeBase.h"

APuzzleManager::APuzzleManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APuzzleManager::BeginPlay()
{
	Super::BeginPlay();

	// Auto-find BlinkManager if not set
	if (!BlinkManager)
	{
		TArray<AActor*> FoundBM;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlinkManager::StaticClass(), FoundBM);
		if (FoundBM.Num() > 0)
		{
			BlinkManager = Cast<ABlinkManager>(FoundBM[0]);
		}
	}


	// Find all StoneplateGroups and bind
	TArray<AActor*> FoundGroups;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStoneplateGroup::StaticClass(), FoundGroups);
	for (AActor* A : FoundGroups)
	{
		if (AStoneplateGroup* G = Cast<AStoneplateGroup>(A))
		{
			// avoid duplicate binds if something reruns BeginPlay in PIE
			G->OnPuzzleSolved.RemoveDynamic(this, &APuzzleManager::HandleGroupPuzzleSolved);
			G->OnPuzzleSolved.AddDynamic(this, &APuzzleManager::HandleGroupPuzzleSolved);
			BoundGroups.Add(G);
		}
	}

	// Puzzle 2: Cemetary
	for (TActorIterator<ARewardPadGroup> It(GetWorld()); It; ++It)
	{
		ARewardPadGroup* Group = *It;
		if (!Group) continue;

		// Avoid duplicate binds in PIE
		Group->OnPuzzleSolved.RemoveDynamic(this, &APuzzleManager::OnPuzzleSolved);
		Group->OnPuzzleSolved.AddDynamic(this, &APuzzleManager::OnPuzzleSolved);

		UE_LOG(LogTemp, Warning, TEXT("PuzzleManager bound RewardPadGroup %s (Id %d)"),
			*Group->GetName(), Group->PuzzleId);
	}


	// Puzzle 3: Simon Monoliths
	for (TActorIterator<ASimonPuzzle> It(GetWorld()); It; ++It)
	{
		ASimonPuzzle* P = *It;
		if (!P) continue;

		// avoid duplicate binds if BeginPlay runs again in PIE
		P->OnPuzzleSolved.RemoveDynamic(this, &APuzzleManager::OnPuzzleSolved);
		P->OnPuzzleSolved.AddDynamic(this, &APuzzleManager::OnPuzzleSolved);

		UE_LOG(LogTemp, Warning, TEXT("PuzzleManager bound Simon puzzle %s (Id %d)"),
			*P->GetName(), P->PuzzleId);
	}




}

void APuzzleManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Unbind cleanly
	for (AStoneplateGroup* G : BoundGroups)
	{
		if (IsValid(G))
		{
			G->OnPuzzleSolved.RemoveDynamic(this, &APuzzleManager::HandleGroupPuzzleSolved);
		}
	}
	BoundGroups.Empty();

	Super::EndPlay(EndPlayReason);
}


const FPuzzleRouting* APuzzleManager::FindRoute(int32 PuzzleId) const
{
	for (const FPuzzleRouting& R : Routes)
		if (R.PuzzleId == PuzzleId) return &R;
	return nullptr;
}

void APuzzleManager::HandleGroupPuzzleSolved(int32 PuzzleId)
{
	// Route the solved puzzle to the blink call
	OnPuzzleSolved(PuzzleId);
}

void APuzzleManager::OnPuzzleSolved(int32 PuzzleId)
{
	UE_LOG(LogTemp, Warning, TEXT("PM::OnPuzzleSolved(%d)"), PuzzleId);

	const bool bFirstSolve = !SolvedPuzzles.Contains(PuzzleId);
	if (!bFirstSolve)
	{
		UE_LOG(LogTemp, Log, TEXT("Puzzle %d already solved — re-triggering visuals only."), PuzzleId);
	}
	else
	{
		SolvedPuzzles.Add(PuzzleId);
	}
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	AHJGameModeBase* HJGameMode = Cast<AHJGameModeBase>(GameMode);
	HJGameMode->ActivateAnchorsForSolvedPuzzle(PuzzleId);

	// ---- PRIZES (only on first solve) --------------------------------------
	if (bFirstSolve)
	{
		// Deduplicate spitters just in case they were added twice in data
		TSet<ALootSplitter*> Unique;
		for (const FPrizeRoute& R : PrizeRoutes)
		{
			if (R.PuzzleId != PuzzleId) continue;
			for (ALootSplitter* S : R.Spitters)
			{
				if (IsValid(S))
				{
					if (!Unique.Contains(S))
					{
						Unique.Add(S);
						UE_LOG(LogTemp, Warning, TEXT("PM: Spitting from %s"), *S->GetName());
						S->Spit();
					}
				}
			}
		}
	}

	// ---- BLINK GUIDES ------------------------------------------------------
	if (BlinkManager)
	{
		// Prefer your routing table for puzzles 1 & 2 (and also 3 if you add it there)
		if (const FPuzzleRouting* Route = FindRoute(PuzzleId))
		{
			// Your 4-arg StartBlinkSequenced signature:
			// (RoadId, Duration, StaggerSeconds, PauseAfterWaveSec)
			BlinkManager->StartBlinkSequenced(
				Route->RoadIdToBlink,
				Route->BlinkDuration,
				/*StaggerSeconds=*/0.15f,
				/*PauseAfterWaveSec=*/1.0f
			);
		}
		else
		{
			// No route entry found. Handle Simon (Puzzle 3) explicitly so
			// we don't break the older puzzles that already use routing.
			if (PuzzleId == 3)
			{
				// RoadId must match the group you set up for the yellow pumpkins
				BlinkManager->StartBlinkSequenced(
					/*Route->RoadIdToBlink*/ 3,
					/*Route->BlinkDuration*/30,
					/*StaggerSeconds=*/0.12f,
					/*PauseAfterWaveSec=*/1.0f
				);
			}
		}
	}

	if (PuzzleId == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("PM: Puzzle 3 solved. RockGate=%s"),
			*GetNameSafe(RockGate));

		if (RockGate)
		{
			RockGate->OpenGate();
		}
	}

	if (PuzzleId == 6)
	{
		AGameModeBase* GameModeA = UGameplayStatics::GetGameMode(GetWorld());
		if (GameModeA)
		{
			AHJGameModeBase* HJGameModeA = Cast<AHJGameModeBase>(GameModeA);
			if (HJGameModeA)
			{
				HJGameModeA->PlayEndSequence();

			}

		}

		UE_LOG(LogTemp, Warning, TEXT("PM: Puzzle 6 solved. ENDGAME!"));

	}
}

