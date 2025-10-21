#include "SimonMonolith.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

ASimonMonolith::ASimonMonolith()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	SpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnArrow"));
	SpawnArrow->SetupAttachment(RootComponent);
	SpawnArrow->SetRelativeLocation(FVector(40,0,0));
}

void ASimonMonolith::BeginPlay()
{
	Super::BeginPlay();
	MID = Mesh->CreateDynamicMaterialInstance(MaterialIndex);
	SetState(EMonolithState::Off);
}

void ASimonMonolith::Apply(float Strength, const FLinearColor& Color)
{
	if (!MID) return;
	if (EmissiveStrengthParam != NAME_None) MID->SetScalarParameterValue(EmissiveStrengthParam, Strength);
	if (EmissiveColorParam    != NAME_None) MID->SetVectorParameterValue(EmissiveColorParam,  Color);
}

void ASimonMonolith::SetState(EMonolithState NewState)
{
	State = NewState;
	switch (State)
	{
	case EMonolithState::Off:       Apply(StrengthOff, FLinearColor::Black); break;
	case EMonolithState::Showing:   Apply(StrengthOn,  ShowColor);           break;
	case EMonolithState::LockedOn:  Apply(StrengthOn,  LockColor);           break;
	case EMonolithState::ErrorRed:  Apply(StrengthOn,  ErrorColor);          break;
	}
}

FVector ASimonMonolith::GetSpawnLocation() const
{
	return SpawnArrow ? SpawnArrow->GetComponentLocation() : GetActorLocation() + FVector(40,0,0);
}
