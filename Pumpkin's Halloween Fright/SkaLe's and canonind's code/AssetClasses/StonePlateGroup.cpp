#include "AssetClasses/StonePlateGroup.h"
#include "AssetClasses/StonePlate.h"
#include "Engine/World.h"
#include "TimerManager.h"

AStoneplateGroup::AStoneplateGroup()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AStoneplateGroup::BeginPlay()
{
	Super::BeginPlay();
}

void AStoneplateGroup::NotifyPlateStepped(AStoneplate* Plate)
{
	if (!Plate) return;

	// Start shared window if not already active
	if (!bWindowActive)
	{
		StartWindow();
	}

	// Light this plate and remember it
	Plate->ActivatePlate();
	ActivePlates.Add(Plate);

	// If the rule is "all within window":
	if (bWindowActive && ActivePlates.Num() >= RequiredCount)
	{
		bSolved = true;

		EndWindow();
		
		// Broadcast solved immediately (optionally EndWindow() too)
		OnPuzzleSolved.Broadcast(PuzzleId);
	}
}

float AStoneplateGroup::GetTimeRemaining() const
{
	if (!bWindowActive) return 0.f;

	if (const UWorld* World = GetWorld())
	{
		return World->GetTimerManager().GetTimerRemaining(WindowTimerHandle);
	}
	return 0.f;
}

void AStoneplateGroup::StartWindow()
{
	bWindowActive = true;

	// Notify UI
	OnWindowStarted.Broadcast(ActiveWindowSeconds);

	// Schedule window end
	GetWorld()->GetTimerManager().ClearTimer(WindowTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		WindowTimerHandle,
		this,
		&AStoneplateGroup::EndWindow,
		ActiveWindowSeconds,
		false);

	// Repeating tick for UI updates (20 Hz)
	GetWorld()->GetTimerManager().ClearTimer(WindowTickTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		WindowTickTimerHandle,
		this,
		&AStoneplateGroup::TickWindow,
		0.05f,
		true);
}

void AStoneplateGroup::TickWindow()
{
	const float Remaining = GetTimeRemaining();
	OnWindowTick.Broadcast(FMath::Max(0.f, Remaining));
}

void AStoneplateGroup::EndWindow()
{
	// Stop UI updates
	GetWorld()->GetTimerManager().ClearTimer(WindowTickTimerHandle);

	// Turn off every active plate
	for (TWeakObjectPtr<AStoneplate> PlatePtr : ActivePlates)
	{
		if (AStoneplate* Plate = PlatePtr.Get())
		{
			Plate->DeactivatePlate();
		}
	}

	ActivePlates.Empty();
	bWindowActive = false;

	// Notify UI
	OnWindowEnded.Broadcast();
}
