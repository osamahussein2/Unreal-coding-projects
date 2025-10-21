// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpactSystem/AnimNotify_SurfaceImpact.h"
#include "ImpactSystem/SurfaceImpactSubsystem.h"
#include "ImpactSystem/SurfaceImpactInterface.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


UAnimNotify_SurfaceImpact::UAnimNotify_SurfaceImpact()
{
	ImpactType = ESurfaceImpactType::Footstep_Walk;
	SocketName = NAME_None;
	TraceDistance = 50.f;
	TraceStartOffset = 5.f;
	VFXScale = 1.f;
	VolumeMultiplier = 1.f;
	PitchMultiplier = 1.f;
	bAttachSound = false;
	bDisableSound = false;
	bDisableVFX = false;
	bDisableDecal = false;

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(100, 200, 255, 255);
#endif
}

void UAnimNotify_SurfaceImpact::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetWorld()) return;

	USurfaceImpactSubsystem* ImpactSubsystem = MeshComp->GetWorld()->GetSubsystem<USurfaceImpactSubsystem>();
	if (!ImpactSubsystem) return;

	FSurfaceImpactParams Params;
	if (PerformSurfaceTrace(MeshComp, Params))
	{
		Params.ImpactType = ImpactType;
		Params.VFXScale = VFXScale;
		Params.VolumeMultiplier = VolumeMultiplier;
		Params.PitchMultiplier = PitchMultiplier;
		Params.bAttachSound = bAttachSound;
		Params.bDisableSound = bDisableSound;
		Params.bDisableVFX = bDisableVFX;
		Params.bDisableDecal = bDisableDecal;

		AActor* Owner = MeshComp->GetOwner();
		if (Owner && Owner->Implements<USurfaceImpactInterface>())
		{
			if (!ISurfaceImpactInterface::Execute_OnPreSurfaceImpact(Owner, Params))
			{
				return;
			}
		}

		ImpactSubsystem->SpawnSurfaceImpact(Params, Owner);

		if (Owner && Owner->Implements<USurfaceImpactInterface>())
		{
			ISurfaceImpactInterface::Execute_OnPostSurfaceImpact(Owner, Params);
		}
	}
}

bool UAnimNotify_SurfaceImpact::PerformSurfaceTrace(USkeletalMeshComponent* MeshComp, FSurfaceImpactParams& OutParams) const
{
	if (!MeshComp || !MeshComp->GetWorld()) return false;

	FName ActualSocketName = SocketName;
	AActor* Owner = MeshComp->GetOwner();
	if (Owner && Owner->Implements<USurfaceImpactInterface>())
	{
		FName CustomSocket;
		if (ISurfaceImpactInterface::Execute_GetCustomImpactSocket(Owner, ImpactType, CustomSocket))
		{
			ActualSocketName = CustomSocket;
		}
	}

	FVector SocketLocation = MeshComp->GetSocketLocation(ActualSocketName);
	if (SocketLocation.IsZero())
	{
		// Fallback to comp location
		SocketLocation = MeshComp->GetComponentLocation();
	}

	const FVector TraceStart = SocketLocation + FVector(0.f, 0.f, TraceStartOffset);
	const FVector TraceEnd = SocketLocation - FVector(0.f, 0.f, TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);
	QueryParams.bReturnPhysicalMaterial = true;

	if (MeshComp->GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams))
	{
		OutParams.Location = HitResult.ImpactPoint;
		OutParams.Normal = HitResult.ImpactPoint;
		OutParams.PhysicalMaterial = HitResult.PhysMaterial;
		return true;
	}
	return false;
}

FString UAnimNotify_SurfaceImpact::GetNotifyName_Implementation() const
{
	FString ImpactTypeName = UEnum::GetDisplayValueAsText(ImpactType).ToString();
	return FString::Printf(TEXT("Surface Impact: %s (%s)"), *ImpactTypeName, *SocketName.ToString());
}
