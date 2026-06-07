// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerCable.h"
#include "CableComponent.h"
#include "DefaultPlayer.h"
#include "PowerConnector.h"

// Sets default values
APowerCable::APowerCable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	CableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	CableComponent->SetupAttachment(SceneRoot);

	CableComponent->bAttachStart = true;
	CableComponent->bAttachEnd = true;

	CableComponent->SetSimulatePhysics(true);
	CableComponent->CableLength = 1000.f;
}

// Called when the game starts or when spawned
void APowerCable::BeginPlay()
{
	Super::BeginPlay();
	
}

void APowerCable::ConnectTo(APowerConnector* Connector)
{
	if (IsFullyConnected()) return;

	if (!ConnectionA.ConnectedActor)
	{
		ConnectionA.ConnectedActor = Connector;
		ConnectionA.Socket = Connector->ConnectionPoint;
	} 
	else if (!ConnectionB.ConnectedActor)
	{
		ConnectionB.ConnectedActor = Connector;
		ConnectionB.Socket = Connector->ConnectionPoint;
	}

	UpdateCableVisual();
}

void APowerCable::UpdateCableVisual()
{
	CableComponent->SetSimulatePhysics(false);

	CableComponent->SetAttachEndTo(nullptr, NAME_None, NAME_None);

	if (HasOnlyConnectionA())
	{
		if (!IsValid(ConnectionA.Socket)) return;

		CableComponent->AttachToComponent(ConnectionA.Socket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		if (Holder)
		{
			CableComponent->bAttachEnd = true;

			CableComponent->SetAttachEndToComponent(Holder->GetMesh(), TEXT("middle_01_rSocket"));
			
			CableComponent->EndLocation = FVector::ZeroVector;
		}
	}
	else if (HasOnlyConnectionB())
	{
		if (!IsValid(ConnectionB.Socket)) return;

		CableComponent->AttachToComponent(ConnectionB.Socket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		if (Holder)
		{
			CableComponent->bAttachEnd = true;

			CableComponent->SetAttachEndToComponent(Holder->GetMesh(), TEXT("middle_01_rSocket"));
			
			CableComponent->EndLocation = FVector::ZeroVector;
		}
	}
	else if (IsFullyConnected())
	{
		if (!IsValid(ConnectionA.Socket) ||
			!IsValid(ConnectionB.Socket))
		{
			return;
		}

		CableComponent->AttachToComponent(ConnectionA.Socket, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		
		CableComponent->SetAttachEndToComponent(ConnectionB.Socket);
		CableComponent->EndLocation = FVector::ZeroVector;

		if (Holder)
		{
			Holder->CurrentEquipment = nullptr;
			Holder->bIsHoldingCable = false;
			Holder = nullptr;
		}
	}
}

bool APowerCable::HasOnlyConnectionA()
{
	return ConnectionA.ConnectedActor && !ConnectionB.ConnectedActor;
}

bool APowerCable::HasFreeConnection()
{
	return !ConnectionA.ConnectedActor || !ConnectionB.ConnectedActor;
}

bool APowerCable::HasOnlyConnectionB()
{
	return !ConnectionA.ConnectedActor && ConnectionB.ConnectedActor;
}
 
bool APowerCable::IsFullyConnected()
{
	return ConnectionA.ConnectedActor && ConnectionB.ConnectedActor;
}

bool APowerCable::CanInteract_Implementation(ADefaultPlayer* Interactor) const
{
	if(!Interactor) return false;

	if (Interactor->bIsHoldingCable) return false;

	return true;
}

void APowerCable::Interact_Implementation(ADefaultPlayer* Interactor)
{
	if (!Interactor) return;

	Holder = Interactor;

	Interactor->CurrentEquipment = this;
	Interactor->bIsHoldingCable = true;

	CableComponent->SetSimulatePhysics(false);

	AttachToComponent(Interactor->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("middle_01_rSocket"));

	CableComponent->AttachToComponent(SceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CableComponent->SetAttachEndTo(nullptr, NAME_None, NAME_None);
}

void APowerCable::TryConnect(AActor* Target)
{
	if (!Holder) return;
	if (!Target) return;
	if (!Target->Implements<UConnectionInterface>()) return;
	if (Target == ConnectionA.ConnectedActor) return;
	if (Target == ConnectionB.ConnectedActor) return;

	if (IConnectionInterface::Execute_CanConnectCable(Target, this))
	{
		IConnectionInterface::Execute_ConnectCable(Target, this);
	}
}

bool APowerCable::IsHitByTrace(const FVector& TraceStart, const FVector& TraceEnd, float Radius, float& OutDistance)
{
	TArray<FVector> Particles;
	CableComponent->GetCableParticleLocations(Particles);
	UE_LOG(LogTemp, Warning, TEXT("Particles: %d"), Particles.Num());

	if (Particles.Num() < 2)
	{
		return false;
	}

	float BestDistance = TNumericLimits<float>::Max();

	for (int32 i = 0; i < Particles.Num() - 1; i++)
	{
		FVector SegmentCenter = (Particles[i] + Particles[i + 1]) * 0.5f;

		FVector ClosestPoint = FMath::ClosestPointOnSegment(SegmentCenter, TraceStart, TraceEnd);

		float Distance = FVector::Distance(SegmentCenter, ClosestPoint);

		if (Distance <= Radius)
		{
			BestDistance = FMath::Min(BestDistance, FVector::Distance(TraceStart, SegmentCenter));
		}
	}

	if (BestDistance != TNumericLimits<float>::Max())
	{
		OutDistance = BestDistance;
		return true;
	}
	return false;
}


