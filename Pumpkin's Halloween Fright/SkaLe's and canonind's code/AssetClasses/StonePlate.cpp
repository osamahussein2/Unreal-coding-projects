#include "AssetClasses/StonePlate.h"
#include "AssetClasses/StonePlateGroup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstanceDynamic.h"

AStoneplate::AStoneplate()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	StoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoneMesh"));
	StoneMesh->SetupAttachment(RootComponent);
	StoneMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StoneMesh->SetCollisionResponseToAllChannels(ECR_Block);

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	BoxTrigger->SetupAttachment(RootComponent);
	BoxTrigger->SetBoxExtent(FVector(50.f, 50.f, 20.f));
	BoxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxTrigger->SetGenerateOverlapEvents(true);

	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AStoneplate::HandleBeginOverlap);
	BoxTrigger->OnComponentEndOverlap.AddDynamic(this, &AStoneplate::HandleEndOverlap);
}

void AStoneplate::BeginPlay()
{
	Super::BeginPlay();

	if (StoneMesh)
	{
		DynMaterial = StoneMesh->CreateDynamicMaterialInstance(MaterialElementIndex);
		if (DynMaterial && EmissiveParamName != NAME_None)
		{
			DynMaterial->SetScalarParameterValue(EmissiveParamName, EmissiveOff);
		}
	}
}

void AStoneplate::ActivatePlate()
{
	// 🔒 If the puzzle is already solved, ignore any attempt to light this plate
	if (Group && Group->IsSolved())
	{
		return;
	}

	if (DynMaterial && EmissiveParamName != NAME_None)
	{
		DynMaterial->SetScalarParameterValue(EmissiveParamName, EmissiveOn);
	}
}

void AStoneplate::DeactivatePlate()
{
	if (DynMaterial && EmissiveParamName != NAME_None)
	{
		DynMaterial->SetScalarParameterValue(EmissiveParamName, EmissiveOff);
	}
}

void AStoneplate::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// Only react to player characters (adjust as needed)
	if (OtherActor && OtherActor != this && Cast<ACharacter>(OtherActor))
	{
		// 🔒 If group exists and is already solved, do nothing.
		if (Group && Group->IsSolved())
		{
			return;
		}

		// Tell the group we were stepped on; it will start the shared timer if needed
		if (Group)
		{
			Group->NotifyPlateStepped(this);
		}
		else
		{
			// Fallback (optional): behave locally if no group assigned
			ActivatePlate();
		}
	}
}

void AStoneplate::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Do nothing; group owns the off-timing.
}
