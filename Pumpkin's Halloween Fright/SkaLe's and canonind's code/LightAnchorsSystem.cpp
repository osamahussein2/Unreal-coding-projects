// Fill out your copyright notice in the Description page of Project Settings.


#include "LightAnchorsSystem.h"
#include "LightAnchor.h"

ALightAnchorsSystem::ALightAnchorsSystem()
{


}



void ALightAnchorsSystem::ActivateAnchorsForSolvedPuzzle(int32 PuzzleId)
{
	if (LightAnchorsSections.Num() <= 0) return;

	int32* ActivatedId = ActivatedIds.Find(PuzzleId);
	if (ActivatedId) return;

	FLightAnchorSection* NextSection = LightAnchorsSections.Find(PuzzleId);
	if (NextSection)
	{
		for (int32 i = 0; i < NextSection->Anchors.Num(); i++)
		{
			ALightAnchor* Anchor = NextSection->Anchors[i];
			if (Anchor)
			{

				FLightAnchorData AnchorData{
					Anchor->GetAnchorPosition(),
					Anchor->GetOuterRadius(),
					Anchor->GetInnerRadius(),
					Anchor->GetLightRadius()
				};
				ActiveLightAnchors.Add(AnchorData);
			}
		}
	}
	ActivatedIds.Add(PuzzleId);
}

void ALightAnchorsSystem::ResetLightAnchorsSections()
{
	ActiveLightAnchors.Empty();
	ActivatedIds.Empty();
}



void ALightAnchorsSystem::ActivateInitialAnchors()
{
	ActivateAnchorsForSolvedPuzzle(999);
}

FVector ALightAnchorsSystem::CaclulateLightPosition(FVector PlayerLocation, float& OutRadius)
{
	FVector ResultLightLocation;

	if (ActiveLightAnchors.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No anchors available"));
		OutRadius = 0.0f;
		return PlayerLocation;
	}

	// 	TArray<FLightAnchorData> CloseAnchors;
	// 
	// 	FLightAnchorData ClosestAnchor = ActiveLightAnchors.Num() > 0 ? ActiveLightAnchors[0] : FLightAnchorData();
	// 	float ClosestDistanceToEdge = (PlayerLocation - ClosestAnchor.Position).Length() - ClosestAnchor.RadiusOuter - ClosestAnchor.LightRadius;
	// 
	// 	// Find all anchors where player inside
	// 	for (int32 i = 0; i < ActiveLightAnchors.Num(); i++)
	// 	{
	// 		FLightAnchorData& Data = ActiveLightAnchors[i];
	// 		float DistanceToCenter = (PlayerLocation - Data.Position).Length();
	// 		float DistanceToEdge = DistanceToCenter - Data.RadiusOuter - Data.LightRadius;
	// 
	// 
	// 		if (ClosestDistanceToEdge > DistanceToEdge)
	// 		{
	// 			ClosestDistanceToEdge = DistanceToEdge;
	// 			ClosestAnchor = Data;
	// 		}
	// 
	// 		if (DistanceToEdge < 0)
	// 		{
	// 			CloseAnchors.Add(Data);
	// 		}
	// 	}
	// 
	// 	FLightAnchorData FoundAnchor;
	// 	if (CloseAnchors.Num() <= 0)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("No anchors"));
	// 		FoundAnchor = ClosestAnchor;
	// 	}
	// 	else
	// 	{
	// 		FLightAnchorData Sum;
	// 		float TotalWeight = 0.0f;
	// 		for (const FLightAnchorData& Loc : CloseAnchors)
	// 		{
	// 			Sum.Position += Loc.Position;
	// 		}
	// 		FoundAnchor.Position = Sum.Position / CloseAnchors.Num();	
	// 
	// 		for (const FLightAnchorData& Loc : CloseAnchors)
	// 		{
	// 			// Weighted average for radiuses 
	// 			float Distance = FVector::Distance(FoundAnchor.Position, Loc.Position);
	// 			float Weight = 1.0f / FMath::Max(Distance, 1.0f); // Closer = higher influence
	// 
	// 			Sum.RadiusOuter += Loc.RadiusOuter * Weight;
	// 			Sum.RadiusInner += Loc.RadiusInner * Weight;
	// 			Sum.LightRadius += Loc.LightRadius * Weight;
	// 
	// 			TotalWeight += Weight;
	// 		}
	// 
	// 		FoundAnchor.Position = ClosestAnchor.Position;
	// 		if (TotalWeight > 0.0f)
	// 		{
	// 			FoundAnchor.RadiusOuter = Sum.RadiusOuter / TotalWeight;
	// 			FoundAnchor.RadiusInner = Sum.RadiusInner / TotalWeight;
	// 			FoundAnchor.LightRadius = Sum.LightRadius / TotalWeight;
	// 		}
	// 		else
	// 		{
	// 			FoundAnchor.RadiusOuter = FoundAnchor.RadiusInner = FoundAnchor.LightRadius = 0.0f;
	// 		}
	//	}

	FLightAnchorData Sum;
	float TotalWeight = 0.0f;

	FLightAnchorData ClosestAnchor = ActiveLightAnchors[0];
	float ClosestDistance = (PlayerLocation - ClosestAnchor.Position).Length();

	for (const FLightAnchorData& Data : ActiveLightAnchors)
	{
		float DistanceToCenter = (PlayerLocation - Data.Position).Length();
		float InfluenceRadius = Data.RadiusOuter + Data.LightRadius;

		if (DistanceToCenter < ClosestDistance)
		{
			ClosestDistance = DistanceToCenter;
			ClosestAnchor = Data;
		}

		float Weight = FMath::Clamp(1.0f - (DistanceToCenter / InfluenceRadius), 0.0f, 1.0f);

		if (Weight > 0.0f)
		{
			Sum.Position += Data.Position * Weight;
			Sum.RadiusOuter += Data.RadiusOuter * Weight;
			Sum.RadiusInner += Data.RadiusInner * Weight;
			Sum.LightRadius += Data.LightRadius * Weight;
			TotalWeight += Weight;
		}
	}

	FLightAnchorData FoundAnchor;

	if (TotalWeight > 0.0f)
	{
		FoundAnchor.Position = Sum.Position / TotalWeight;
		FoundAnchor.RadiusOuter = Sum.RadiusOuter / TotalWeight;
		FoundAnchor.RadiusInner = Sum.RadiusInner / TotalWeight;
		FoundAnchor.LightRadius = Sum.LightRadius / TotalWeight;
	}
	else
	{
		// Fallback to closest
		FoundAnchor = ClosestAnchor;
	}

	OutRadius = FoundAnchor.LightRadius;

	// ------
	OutRadius = FoundAnchor.LightRadius;

	FVector DirectionToPlayer = PlayerLocation - FoundAnchor.Position;
	float DistanceToLight = DirectionToPlayer.Length();
	float DistanceToInner = DistanceToLight - FoundAnchor.RadiusInner;
	float LightInnerRadius = FoundAnchor.RadiusInner;
	float LightOuterRadius = FoundAnchor.RadiusOuter;


	ResultLightLocation = PlayerLocation;
	if (DistanceToLight > LightInnerRadius)
	{
		float DistanceToAdd = ((FMath::Clamp(DistanceToLight, LightInnerRadius, 2.f * LightOuterRadius - LightInnerRadius) - LightInnerRadius) * 0.5f) + LightInnerRadius;
		ResultLightLocation = FoundAnchor.Position + DirectionToPlayer.GetSafeNormal() * DistanceToAdd;
	}
	return ResultLightLocation;
}

