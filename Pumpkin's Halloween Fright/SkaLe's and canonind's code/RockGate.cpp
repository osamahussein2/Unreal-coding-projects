// ARockGate.cpp
#include "RockGate.h"
#include "Components/StaticMeshComponent.h"

ARockGate::ARockGate()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	LeftRock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftRock"));
	LeftRock->SetupAttachment(RootComponent);

	RightRock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightRock"));
	RightRock->SetupAttachment(RootComponent);
}

void ARockGate::BeginPlay()
{
	Super::BeginPlay();

	// Cache closed relative locations
	LeftClosedRel  = LeftRock->GetRelativeLocation();
	RightClosedRel = RightRock->GetRelativeLocation();

	// Compute open relative locations
	LeftOpenRel  = LeftClosedRel  + LeftOpenOffset;
	RightOpenRel = RightClosedRel + RightOpenOffset;

	// Ensure we start closed (unless you want open from start)
	ApplyPose(/*Alpha01=*/ bOpenOnBeginPlay ? 1.f : 0.f);
	bIsOpen = bOpenOnBeginPlay;

	if (bOpenOnBeginPlay)
	{
		// If you want to see it animate on BeginPlay instead, call StartMove(true) here.
	}
}

void ARockGate::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsMoving) return;

	const float Now = GetWorld()->GetTimeSeconds();
	const float Elapsed = Now - MoveStartTime;
	const float Dur     = FMath::Max(0.01f, MoveDuration);
	float Alpha = FMath::Clamp(Elapsed / Dur, 0.f, 1.f);

	// Ease 0..1
	const float Eased = UKismetMathLibrary::Ease(0.f, 1.f, Alpha, EaseFunc, 1.f);
	ApplyPose(Eased);

	if (Alpha >= 1.f)
	{
		bIsMoving = false;
		bIsOpen   = bTargetOpen;
		if (bDisableCollisionWhileMoving)
		{
			SetRocksCollision(true);
		}
	}
}

void ARockGate::ApplyPose(float Alpha01)
{
	// Interpolate RELATIVE locations between closed and open
	const FVector L = FMath::Lerp(LeftClosedRel,  LeftOpenRel,  Alpha01);
	const FVector R = FMath::Lerp(RightClosedRel, RightOpenRel, Alpha01);

	LeftRock->SetRelativeLocation(L, false, nullptr, ETeleportType::None);
	RightRock->SetRelativeLocation(R, false, nullptr, ETeleportType::None);
}

void ARockGate::SetRocksCollision(bool bEnable)
{
	ECollisionEnabled::Type Mode = bEnable ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision;
	LeftRock->SetCollisionEnabled(Mode);
	RightRock->SetCollisionEnabled(Mode);
}

void ARockGate::StartMove(bool bOpen)
{
	// Already at target?
	if (bIsOpen == bOpen && !bIsMoving)
		return;

	bTargetOpen   = bOpen;
	bIsMoving     = true;
	MoveStartTime = GetWorld()->GetTimeSeconds();
	MoveDuration  = OpenDuration;

	if (bDisableCollisionWhileMoving)
	{
		SetRocksCollision(false);
	}
}

void ARockGate::OpenGate()
{
	StartMove(true);
}

void ARockGate::CloseGate()
{
	StartMove(false);
}

void ARockGate::ToggleGate()
{
	StartMove(!bIsOpen);
}
