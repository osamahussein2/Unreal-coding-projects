// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Actions/ASAction_ThrowAttack.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "Combat/PlayerProjectile.h"
#include "Components/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"


bool UASAction_ThrowAttack::OnAnimNotifyReceived(UActionSystemComponent* ActionComp, FName EventName)
{
	bool bHandled = Super::OnAnimNotifyReceived(ActionComp, EventName);
	if (bHandled) return bHandled;

	if (EventName == "SpawnProjectile")
	{
		SpawnProjectile(ActionComp);
		return true;
	}
	return false;
}



void UASAction_ThrowAttack::SpawnProjectile(UActionSystemComponent* ActionComp)
{
	if (!ProjectileClass || !ActionComp) return;

	AActor* Avatar = ActionComp->GetAvatarActor();
	if (!Avatar) return;
	ACharacter* AvatarChar = Cast<ACharacter>(Avatar);
	if (!AvatarChar) return;

	USkeletalMeshComponent* AvatarMesh = AvatarChar->GetMesh();
	if (!AvatarMesh)return;

	FVector SpawnLocation(FVector::ZeroVector);

	if (AvatarMesh->DoesSocketExist(SpawnSocket))
	{
		SpawnLocation = AvatarMesh->GetSocketLocation(SpawnSocket);
	}
	else
	{
		SpawnLocation = AvatarMesh->GetComponentLocation();
	}

	UCombatComponent* CombatComponent = AvatarChar->GetComponentByClass<UCombatComponent>();
	if (!CombatComponent) return;

	TargetLocation = CombatComponent->GetTargetLocation();

	FRotator SpawnRotation = Avatar->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Avatar;
	SpawnParams.Instigator = Cast<APawn>(Avatar);

	APlayerProjectile* Projectile = Avatar->GetWorld()->SpawnActor<APlayerProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (Projectile)
	{
		Projectile->SetInstigatorActor(Avatar);

		FVector LaunchDirection = FVector::ForwardVector;
		float CalculatedPitch = 0.0f;
		float CalculatedSpeed = 0.0f;


		if (CalculateBalisticTrajectory(SpawnLocation, TargetLocation, MaxProjectileSpeed, DefaultGravity, CalculatedPitch, CalculatedSpeed, LaunchDirection))
		{
			Projectile->LaunchProjectileWithSpeed(LaunchDirection, CalculatedSpeed);
		}
		else
		{
			// Fallback if we are unable to reach the target destination with given max speed
			// Just throw with max speed with 45 angle
			FVector DirectionToTarget = (TargetLocation - SpawnLocation).GetSafeNormal();
			FRotator FallbackRotation(45.f, DirectionToTarget.Rotation().Yaw, 0.0f);
			Projectile->LaunchProjectileWithSpeed(FallbackRotation.Vector(), MaxProjectileSpeed);
		}		
	}

}


bool UASAction_ThrowAttack::CalculateBalisticTrajectory(const FVector& StartPos, const FVector& TargetPos, float MaxSpeed, float Gravity, float& OutPitch, float& OutSpeed, FVector& OutLaunchDirection)
{
	FVector Displacement = TargetPos - StartPos;
	float Distance = Displacement.Length();

	if (Distance < 0.01f) return false;

	float Height = Displacement.Z;
	float HorizontalDistance = FVector(Displacement.X, Displacement.Y, 0.0f).Length();

	if (HorizontalDistance < 0.01f)
	{
		// Directly above or below. This shouldn't happen, but just in case:
		OutPitch = (Height > 0) ? 90.f : -90.f;
		OutSpeed = MaxSpeed;
		OutLaunchDirection = FVector(0, 0, Height > 0 ? 1 : -1);
		return true;
	}

	// Preferred pitch angle 45 degrees - gives maximum range
	const float PreferredPitch = 45.f;

	float BestSpeed = MaxSpeed;
	float BestPitch = PreferredPitch;
	bool FoundSolution = false;

	// This could be computationally expensive
	for (int32 i = 0; i <= 6; ++i)
	{
		float TestPitch = FMath::Lerp(MinPitch, MaxPitch, i / 6.f);
		float PitchRad = FMath::DegreesToRadians(TestPitch);
		float SinPitch = FMath::Sin(PitchRad);
		float CosPitch = FMath::Cos(PitchRad);

		float Numerator = Gravity * HorizontalDistance * HorizontalDistance;
		float Denominator = 2.0f * CosPitch * CosPitch * (HorizontalDistance * FMath::Tan(PitchRad) - Height);
		if (Denominator <= 0.01f)
		{
			continue;
		}
		float RequiredSpeed = FMath::Sqrt(Numerator / Denominator);

		if (RequiredSpeed <= MaxSpeed)
		{
			FoundSolution = true;
			BestSpeed = RequiredSpeed;
			BestPitch = TestPitch;
			break; // Use the first lowest angle solution
		}
	}
	if (!FoundSolution) return false;

	FVector HorizontalDisplacement(Displacement.X, Displacement.Y, 0.0f);
	FRotator DirectionRotation = HorizontalDisplacement.Rotation();

	FRotator LaunchRotation(BestPitch, DirectionRotation.Yaw, 0.0f);
	OutLaunchDirection = LaunchRotation.Vector();
	OutPitch = BestPitch;
	OutSpeed = BestSpeed;

	return true;
}
