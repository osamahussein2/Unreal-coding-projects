#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CandyCollectible.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class URotatingMovementComponent;

/**
 * Spawns with a projectile arc. On first ground contact, switches to idle:
 * spinning around Z and bobbing up/down until the player overlaps it.
 */
UCLASS()
class GDTV_HALLOWEEN2025_API ACandyCollectible : public AActor
{
	GENERATED_BODY()

public:
	ACandyCollectible();

	/** Called right after spawning to set initial velocity. */
	UFUNCTION(BlueprintCallable, Category="Candy")
	void LaunchWithVelocity(const FVector& Velocity);

	UFUNCTION(BlueprintCallable, Category="Candy")
	void IgnoreActorWhenMoving(AActor* ActorToIgnore, bool bIgnore);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Enter idle state (spin+bob) using the impact result for placement. */
	UFUNCTION()
	void EnterIdleState_FromStop(const FHitResult& ImpactResult);   // for OnProjectileStop

	/** OnComponentHit signature — forwards to EnterIdleState. */
	UFUNCTION()
	void EnterIdleState_OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	                          const FHitResult& Hit);

	/** Player overlap → collect (destroy). */
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                   bool bFromSweep, const FHitResult& Sweep);

	/* ---------- Components ---------- */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* Sphere = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UProjectileMovementComponent* Projectile = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	URotatingMovementComponent* Rotator = nullptr;

	/* ---------- Idle (spin+bob) tuning ---------- */
	/** Degrees per second around Z (Yaw). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Candy|Idle")
	float SpinYawDegPerSec = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Candy|Idle")
	float SpinRollDegPerSec = 120.f;

	/** Vertical bobbing amplitude (cm). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Candy|Idle")
	float BobAmplitude = 8.f;

	/** Bobbing speed (cycles per second). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Candy|Idle")
	float BobHz = 0.8f;

	/** Small Z lift when resting on ground. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Candy|Idle")
	float GroundLift = 2.f;

	/** Safety: auto-despawn if not collected. 0 = infinite. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Candy")
	float LifeSpanSeconds = 30.f;

private:
	bool   bIdle = false;
	FVector IdleBaseLocation = FVector::ZeroVector;
	float  BobTime = 0.f;
};
