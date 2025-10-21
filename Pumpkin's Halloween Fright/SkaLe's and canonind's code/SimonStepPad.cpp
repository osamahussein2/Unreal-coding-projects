#include "SimonStepPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "SimonPuzzle.h"

ASimonStepPad::ASimonStepPad()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);
	Trigger->InitBoxExtent(FVector(40,40,20));
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ASimonStepPad::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASimonStepPad::HandleBeginOverlap);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ASimonStepPad::HandleEndOverlap);
}

void ASimonStepPad::SetPadEnabled(bool bInEnabled)
{
	bEnabled = bInEnabled;
	if (!Trigger) return;

	if (bEnabled)
	{
		Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Trigger->SetGenerateOverlapEvents(true);
	}
	else
	{
		// fully disable and clear the latch
		Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Trigger->SetGenerateOverlapEvents(false);
		bOccupiedByPlayer = false;
	}
}


void ASimonStepPad::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									   bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Puzzle) return;
	if (!Cast<ACharacter>(OtherActor)) return;

	if (bOccupiedByPlayer) return;          // already inside → ignore repeats
	bOccupiedByPlayer = true;

	Puzzle->OnPlayerStep(Index);
}

void ASimonStepPad::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Cast<ACharacter>(OtherActor)) return;
	bOccupiedByPlayer = false;              // arm again when player leaves
}

