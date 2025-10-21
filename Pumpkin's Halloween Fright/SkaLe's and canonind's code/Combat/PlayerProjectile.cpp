// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PlayerProjectile.h"
#include "ImpactSystem/SurfaceImpactSubsystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GDTV_Halloween2025/GDTV_Halloween2025.h" // For projectile channel macro

APlayerProjectile::APlayerProjectile()
{
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>("Sphere");
	CollisionComp->InitSphereRadius(8.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(COLLISION_PROJECTILE);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	//CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	//CollisionComp->SetNotifyRigidBodyCollision(true);
	CollisionComp->OnComponentHit.AddDynamic(this, &APlayerProjectile::OnProjectileHit);
	SetRootComponent(CollisionComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = InitialSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 1.0f;

	InitialLifeSpan = LifeTime;

}

void APlayerProjectile::LaunchProjectile(const FVector& Direction)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
	}
}

void APlayerProjectile::LaunchProjectileWithSpeed(const FVector& InDirection, float InInitialSpeed)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = InInitialSpeed;
		ProjectileMovement->MaxSpeed = InInitialSpeed;
		LaunchProjectile(InDirection);
	}
}

void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (TrailEffect)
	{
		TrailComp = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailEffect, RootComponent, NAME_None, FVector::ZeroVector,
			FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
	}

	if (AActor* MyOwner = GetOwner())
	{
		CollisionComp->IgnoreActorWhenMoving(MyOwner, true);

	}
	
}

void APlayerProjectile::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this || OtherActor == InstigatorActor)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *OtherActor->GetName());

	UGameplayStatics::ApplyPointDamage(
		OtherActor,
		Damage,
		GetVelocity().GetSafeNormal(),
		Hit,
		InstigatorActor ? InstigatorActor->GetInstigatorController() : nullptr,
		this,
		nullptr
	);

	USurfaceImpactSubsystem* System = GetWorld()->GetSubsystem<USurfaceImpactSubsystem>();

	FSurfaceImpactParams Params;
	Params.Location = Hit.ImpactPoint;
	Params.Normal = Hit.ImpactNormal;
	Params.ImpactType = ESurfaceImpactType::Footstep_Run;

	System->SpawnSurfaceImpact(Params, this);

	Destroy();
}


