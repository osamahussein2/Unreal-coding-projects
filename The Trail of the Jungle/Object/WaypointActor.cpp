// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/WaypointActor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWaypointActor::AWaypointActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWaypointActor::BeginPlay()
{
	Super::BeginPlay();

}

void AWaypointActor::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (!path.IsEmpty()) path.Empty();

	if (splineComponent)
	{
		splineComponent->DestroyComponent();
		splineComponent = nullptr;
	}
}

// Called every frame
void AWaypointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaypointActor::InitializeWaypointLocations(APlayerCharacter* playerCharacter_, FVector goalLocation_)
{
	if (!playerCharacter_) return;

	goalLocation = goalLocation_;

	InitializeWaypoint(playerCharacter_);
}

void AWaypointActor::InitializeWaypoint(APlayerCharacter* playerCharacter_)
{
	if (!playerCharacter_) return;

	SetActorLocation(playerCharacter_->GetActorLocation());

	if (!splineComponent)
	{
		splineComponent = NewObject<USplineComponent>(this, USplineComponent::StaticClass());
		if (!splineComponent) return;

		splineComponent->RegisterComponent();
		splineComponent->SetMobility(EComponentMobility::Static);

		if (!path.IsEmpty()) path.Empty();
		GetPath(playerCharacter_, goalLocation);
	}

	else
	{
		splineComponent->ClearSplinePoints();

		if (!path.IsEmpty()) path.Empty();
		GetPath(playerCharacter_, goalLocation);
	}
}

void AWaypointActor::UpdateWaypoint(APlayerCharacter* playerCharacter_)
{
	if (!playerCharacter_) return;
	
	InitializeWaypoint(playerCharacter_);
}

void AWaypointActor::RemoveWaypoint()
{
	for (USplineMeshComponent* splineMesh : splineMeshes)
	{
		splineMesh->DestroyComponent();
		splineMesh = nullptr;
	}

	if (!splineMeshes.IsEmpty()) splineMeshes.Empty();
}

void AWaypointActor::GetPath(APlayerCharacter* playerCharacter_, FVector targetLocation_)
{
	if (!playerCharacter_) return;

	if (UNavigationPath* navigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(),
		playerCharacter_->GetActorLocation(), targetLocation_))
	{
		// Create a waypoint path if there are more than 0 path points
		if (navigationPath->PathPoints.Num() > 0)
		{
			CreateWaypoint(navigationPath->PathPoints);
		}
	}

	// Invalid navigation path so destroy waypoint component on any owning actor
	else
	{
		playerCharacter_->DestroyWaypoint();
	}
}

void AWaypointActor::CreateWaypoint(TArray<FVector> path_)
{
	path = path_;
	if (path.IsEmpty()) return;

	RemoveWaypoint();

	for (int i = 0; i < path.Num(); i++)
	{
		if (splineComponent)
		{
			const FVector& addedZOffset = path[i] + FVector(0.0f, 0.0f, zOffset);

			splineComponent->AddSplinePointAtIndex(addedZOffset, i, ESplineCoordinateSpace::World, false);
			splineComponent->SetSplinePointType(i, ESplinePointType::Constant, false);
		}
	}

	if (splineComponent)
	{
		splineComponent->UpdateSpline();

		for (int i = 0; i < path.Num() - 1; i++)
		{
			USplineMeshComponent* splineMeshComponent = NewObject<USplineMeshComponent>(this,
				USplineMeshComponent::StaticClass());

			if (!splineMeshComponent) return;

			splineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			splineMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

			splineMeshComponent->AttachToComponent(splineComponent, FAttachmentTransformRules::KeepRelativeTransform);
			splineMeshComponent->RegisterComponent();

			splineMeshComponent->SetCastShadow(false);

			splineMeshComponent->SetMobility(EComponentMobility::Movable);
			if (staticMesh) splineMeshComponent->SetStaticMesh(staticMesh);

			splineMeshComponent->SetStartAndEnd(splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World),
				splineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World),
				splineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World),
				splineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::World), false);

			splineMeshComponent->SetStartScale(meshSplineScale, false);
			splineMeshComponent->SetEndScale(meshSplineScale, true);

			splineMeshes.Add(splineMeshComponent);
		}
	}
}
