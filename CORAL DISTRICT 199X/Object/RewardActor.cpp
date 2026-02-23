// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/RewardActor.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
ARewardActor::ARewardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	staticMesh->SetupAttachment(GetRootComponent());

	staticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	staticMesh->MinDrawDistance = 2000.0f;
	staticMesh->LDMaxDrawDistance = 2000.0f;

	sceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	sceneCaptureComponent->SetupAttachment(staticMesh);
}

// Called when the game starts or when spawned
void ARewardActor::BeginPlay()
{
	Super::BeginPlay();

	if (staticMesh)
	{
		staticMesh->bVisibleInSceneCaptureOnly = true;
		staticMesh->SetCastShadow(false);
	}
	
	if (sceneCaptureComponent && GetWorld())
	{
		sceneCaptureComponent->ShowOnlyActorComponents(this);

		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ARewardActor::RotateAroundActor,
			GetWorld()->GetDeltaSeconds(), true);
	}
}

// Called every frame
void ARewardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARewardActor::RotateAroundActor()
{
	FRotator pitchRotation = FRotator(0.0f, rotateSpeed, 0.0f);

	AddActorWorldRotation(pitchRotation);
}

