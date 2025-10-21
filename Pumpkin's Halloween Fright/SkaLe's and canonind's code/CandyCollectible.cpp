#include "CandyCollectible.h"

#include "Components/InventoryComponent.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"

ACandyCollectible::ACandyCollectible()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root collision: MUST block the world so projectile can detect hits.
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(20.f);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetCollisionObjectType(ECC_WorldDynamic);
	Sphere->SetCollisionResponseToAllChannels(ECR_Block);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);      // overlap player to collect
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetNotifyRigidBodyCollision(true);                          // enable OnComponentHit
	RootComponent = Sphere;

	// Visual mesh doesn't handle collision
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Projectile motion
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	Projectile->UpdatedComponent = RootComponent;           // <-- critical
	Projectile->bRotationFollowsVelocity = false;
	Projectile->ProjectileGravityScale = 1.f;
	Projectile->bShouldBounce = true;
	Projectile->Bounciness = 0.25f;
	Projectile->Friction = 0.2f;
	Projectile->BounceVelocityStopSimulatingThreshold = 80.f;

	// Idle spin (activated later)
	Rotator = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotator"));
	// Rotator->RotationRate = FRotator(0.f, SpinYawDegPerSec, 0.f);
	Rotator->RotationRate = FRotator(0.0f, 0.0f, 0.f);
	Rotator->Deactivate();

	InitialLifeSpan = 0.f; // we control lifespan manually
}

void ACandyCollectible::BeginPlay()
{
	Super::BeginPlay();

	// Overlap to collect
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACandyCollectible::HandleOverlap);

	// Go idle on first stop or hit
	if (Projectile)
	{
		Projectile->OnProjectileStop.AddDynamic(this, &ACandyCollectible::EnterIdleState_FromStop);
	}
	Sphere->OnComponentHit.AddDynamic(this, &ACandyCollectible::EnterIdleState_OnHit);

	// Safety cleanup
	if (LifeSpanSeconds > 0.f)
	{
		SetLifeSpan(LifeSpanSeconds);
	}
}

void ACandyCollectible::LaunchWithVelocity(const FVector& Velocity)
{
	if (Projectile)
	{
		Projectile->Velocity = Velocity;
		Projectile->Activate(true);       // 🔑 ensure movement is active
		Projectile->UpdateComponentVelocity();
	}
}


void ACandyCollectible::EnterIdleState_OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                             const FHitResult& Hit)
{
	EnterIdleState_FromStop(Hit);
}

void ACandyCollectible::IgnoreActorWhenMoving(AActor* ActorToIgnore, bool bIgnore)
{
	if (!ActorToIgnore || !Sphere) return;
	Sphere->IgnoreActorWhenMoving(ActorToIgnore, bIgnore);
}


void ACandyCollectible::EnterIdleState_FromStop(const FHitResult& ImpactResult)
{
	if (bIdle) return;
	bIdle = true;

	// Stop projectile motion immediately
	if (Projectile)
	{
		Projectile->StopMovementImmediately();
		Projectile->Deactivate();
	}

	// Clamp to ground with a tiny hover (handles uneven terrain)
	FHitResult GroundHit;
	const FVector Start = ImpactResult.ImpactPoint + FVector(0, 0, 30.f);
	const FVector End   = ImpactResult.ImpactPoint - FVector(0, 0, 200.f);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(CandyGroundClamp), false, this);

	FVector Rest = ImpactResult.ImpactPoint;
	if (GetWorld()->LineTraceSingleByChannel(GroundHit, Start, End, ECC_Visibility, Params))
	{
		Rest = GroundHit.Location;
	}
	SetActorLocation(Rest + FVector(0, 0, GroundLift), true);

	// Start spin
	if (Rotator)
	{
		Rotator->RotationRate = FRotator(0.f, SpinYawDegPerSec, 0.f);
		//Rotator->RotationRate = FRotator(0.f, 0.f, SpinRollDegPerSec);
		//Rotator->RotationRate = FRotator(SpinRollDegPerSec, 0.f, 0.f);
		
		Rotator->Activate(true);
	}

	// Prepare bobbing
	IdleBaseLocation = GetActorLocation();
	BobTime = 0.f;
}

void ACandyCollectible::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIdle)
	{
		BobTime += DeltaSeconds;
		const float OffsetZ = BobAmplitude * FMath::Sin(BobTime * 2.f * PI * BobHz);
		SetActorLocation(IdleBaseLocation + FVector(0, 0, OffsetZ), true);
	}
}

void ACandyCollectible::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                      bool bFromSweep, const FHitResult& Sweep)
{
	// Collect when the player (Character) touches
	if (OtherActor && OtherActor != this && Cast<ACharacter>(OtherActor))
	{
		// TODO: add score / inventory / VFX / SFX here
		if (UInventoryComponent* Inventory = OtherActor->FindComponentByClass<UInventoryComponent>())
		{

			Inventory->AddCandy();
		}
		Destroy();
	}
}
