// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatComponent.h"
#include "Combat/TargetIndicator.h"
#include "Components/HealthComponent.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ClosestEnemy = nullptr;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	Indicator = GetWorld()->SpawnActor<ATargetIndicator>(IndicatorClass);

	FTimerHandle Detect_TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(Detect_TimerHandle, this, &UCombatComponent::DetectEnemies, 0.1f, true);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateIndicator(DeltaTime);
}

void UCombatComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	if (IsValid(Indicator))
	{
		Indicator->Destroy();
		Indicator = nullptr;
	}
}

FVector UCombatComponent::GetTargetLocation()
{
	FVector Location;

	if (ClosestEnemy)
	{
		Location = ClosestEnemy->GetActorLocation();
	}
	else
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		FVector Direction = GetOwner()->GetActorForwardVector();

		Location = OwnerLocation + Direction * DefaultThrowDistance;

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());
		if (GetWorld()->LineTraceSingleByChannel(Hit, Location + FVector(0, 0, 1000), Location - FVector(0, 0, 1000), ECC_Visibility, Params))
		{
			Location = Hit.Location + 10.f;
		}

	}

	return Location;
}

bool UCombatComponent::IsBlocking() const
{
	return bIsBlocking;
}

void UCombatComponent::StartBlocking(float DamageReductionFactor)
{
	bIsBlocking = true;
	UHealthComponent* HealthComp = GetOwner()->GetComponentByClass<UHealthComponent>();

	if (HealthComp)
	{
		HealthComp->SetDamageModifier(DamageReductionFactor);
	}
}

void UCombatComponent::StopBlocking()
{
	bIsBlocking = false;
	UHealthComponent* HealthComp = GetOwner()->GetComponentByClass<UHealthComponent>();

	if (HealthComp)
	{
		HealthComp->RestoreDamageModifier();
	}
}

FVector UCombatComponent::GetTargetLocationXY()
{
	FVector Location;

	if (ClosestEnemy)
	{
		Location = ClosestEnemy->GetActorLocation();
	}
	else
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		FVector Direction = GetOwner()->GetActorForwardVector();

		Location = OwnerLocation + Direction * DefaultThrowDistance;
	}

	return Location;
}

void UCombatComponent::DetectEnemies()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	APawn* OwnerPawn = Cast<APawn>(Owner);
	if (!OwnerPawn) return;

	FVector OwnerLocation = Owner->GetActorLocation();
	FVector LookDirection = Owner->GetActorForwardVector();

	float HalfAngleRadians = FMath::DegreesToRadians(DetectionAngle / 2.0f);
	ClosestEnemy = nullptr;
	float ClosestDistance = DetectionRange;

	for (TActorIterator<AActor> ActorItr(Owner->GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* TestActor = *ActorItr;
		if (!TestActor || TestActor == Owner || TestActor->IsHidden()) continue;
		if (!TestActor->ActorHasTag(EnemyTag)) continue;

		FVector DirectionToActor = (TestActor->GetActorLocation() - OwnerLocation).GetSafeNormal();
		float Distance = FVector::Dist(TestActor->GetActorLocation(), OwnerLocation);

		if (Distance > DetectionRange) continue;

		float DotProduct = FVector::DotProduct(LookDirection, DirectionToActor);
		float Angle = FMath::Acos(FMath::Clamp(DotProduct, -1.f, 1.f));

		if (Angle <= HalfAngleRadians && Distance < ClosestDistance)
		{
			ClosestEnemy = TestActor;
			ClosestDistance = Distance;
		}

	}
}

void UCombatComponent::UpdateIndicator(float DeltaTime)
{
	if (!Indicator) return;

	FVector IndicatorTarget = GetTargetLocationXY();


	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(Indicator);
	if (ClosestEnemy)
	{
		Params.AddIgnoredActor(ClosestEnemy);
	}
	if (GetWorld()->LineTraceSingleByChannel(Hit, IndicatorTarget + FVector(0, 0, 1000), IndicatorTarget - FVector(0, 0, 1000), ECC_Visibility, Params))
	{
		IndicatorTarget = Hit.Location + 10.f;
	}


	FVector TargetScale = FVector::OneVector;

	float VariableScale = FMath::Sin(GetWorld()->GetTimeSeconds() * 5.f) * IndicatorPulseScale;
	float TargetScaleF = 1;
	if (ClosestEnemy)
	{
		FVector Origin;
		FVector BoxExtent;
		ClosestEnemy->GetActorBounds(true, Origin, BoxExtent);
		FVector2D BoxExtent2D(BoxExtent.X, BoxExtent.Y);
		TargetScaleF = BoxExtent2D.GetAbsMax() / 50.f;
	}
	float FinalScale = (VariableScale + TargetScaleF) * IndicatorScale;
	TargetScale = FVector(FinalScale, FinalScale, 1.f);

	FVector CurrentLocation = Indicator->GetActorLocation();
	FVector CurrentScale = Indicator->GetActorScale3D();

	CurrentLocation = FMath::VInterpTo(CurrentLocation, IndicatorTarget, DeltaTime, 12.f);
	CurrentScale = FMath::VInterpTo(CurrentScale, TargetScale, DeltaTime, 12.f);
	Indicator->SetActorLocation(CurrentLocation);
	Indicator->SetActorScale3D(CurrentScale);
}

