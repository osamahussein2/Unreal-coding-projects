// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpactSystem/SurfaceImpactSubsystem.h"
#include "ImpactSystem/SurfaceImpactSet.h"
#include "ImpactSystem/SurfaceImpactSettings.h"

#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DrawDebugHelpers.h"

void USurfaceImpactSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadedImpactSets.Reset();

	const USurfaceImpactSettings* Settings = GetDefault<USurfaceImpactSettings>();
	if (Settings)
	{
		SurfaceImpactSets = Settings->SurfaceImpactSets;
		DefaultImpactSet = Settings->DefaultImpactSet;
		MasterVolume = Settings->MasterVolume;
		MasterVFXScale = Settings->MasterVFXScale;
		bDebugDraw = Settings->bDebugDraw;
		DebugDrawDuration = Settings->DebugDrawDuration;
	}

}

void USurfaceImpactSubsystem::Deinitialize()
{
	LoadedImpactSets.Reset();
	Super::Deinitialize();
}

bool USurfaceImpactSubsystem::SpawnSurfaceImpact(const FSurfaceImpactParams& Params, AActor* Instigator /*= nullptr*/)
{
	EPhysicalSurface SurfaceType = EPhysicalSurface::SurfaceType_Default;

	if (Params.PhysicalMaterial.IsValid())
	{
		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Params.PhysicalMaterial.Get());
	}

	USurfaceImpactSet* ImpactSet = GetSurfaceImpactSet(Params.PhysicalMaterial.Get());
	if (!ImpactSet)
	{
		if (bDebugDraw)
		{
			DrawDebugImpact(Params, false);
		}
		return false;
	}

	FSurfaceImpactEffect Effect;
	if (!ImpactSet->GetImpactEffect(Params.ImpactType, Effect))
	{
		if (bDebugDraw)
		{
			DrawDebugImpact(Params, false);
		}
		return false;
	}

	if (!Params.bDisableSound) { SpawnSound(Effect, Params, Instigator);}
	if (!Params.bDisableVFX) { SpawnVisualEffect(Effect, Params); }
	if (!Params.bDisableDecal) { SpawnDecal(Effect, Params); }

	if (bDebugDraw) { DrawDebugImpact(Params, true); }
	return true;
}

bool USurfaceImpactSubsystem::TraceSurfaceImpact(const FVector& Start, const FVector& End, ESurfaceImpactType ImpactType, FSurfaceImpactParams Params, AActor* Instigator /*= nullptr*/)
{
	if (!GetWorld())
	{
		return false;
	}

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Instigator);
	QueryParams.bReturnPhysicalMaterial = true;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
	{
		Params.Location = HitResult.ImpactPoint;
		Params.Normal = HitResult.ImpactNormal;
		Params.PhysicalMaterial = HitResult.PhysMaterial;
		Params.ImpactType = ImpactType;

		return SpawnSurfaceImpact(Params, Instigator);
	}
	return false;
}

USurfaceImpactSet* USurfaceImpactSubsystem::GetSurfaceImpactSet(UPhysicalMaterial* PhysicalMaterial) const
{
	EPhysicalSurface SurfaceType = EPhysicalSurface::SurfaceType_Default;

	if (PhysicalMaterial)
	{
		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysicalMaterial);
	}

	// Check cache first
	if (const TObjectPtr<USurfaceImpactSet>* CachedSet = LoadedImpactSets.Find(SurfaceType))
	{
		return *CachedSet;
	}

	// Load set
	if (const TSoftObjectPtr<USurfaceImpactSet>* SetPtr = SurfaceImpactSets.Find(SurfaceType))
	{
		if (USurfaceImpactSet* LoadedSet = SetPtr->LoadSynchronous())
		{
			const_cast<USurfaceImpactSubsystem*>(this)->LoadedImpactSets.Add(SurfaceType, LoadedSet);
			return LoadedSet;
		}
	}

	// Fall back to default
	if (USurfaceImpactSet* DefaultSet = DefaultImpactSet.LoadSynchronous())
	{
		return DefaultSet;
	}
	return nullptr;
}

void USurfaceImpactSubsystem::RegisterSurfaceImpactSet(USurfaceImpactSet* ImpactSet)
{
	if (ImpactSet)
	{
		LoadedImpactSets.Add(ImpactSet->SurfaceType, ImpactSet);
	}
}

UAudioComponent* USurfaceImpactSubsystem::SpawnSound(const FSurfaceImpactEffect& Effect, const FSurfaceImpactParams& Params, AActor* Instigator)
{
	if (!GetWorld() || Effect.Sound.IsNull())
	{
		return nullptr;
	}

	USoundBase* Sound = Effect.Sound.LoadSynchronous();
	if (!Sound)
	{
		return nullptr;
	}

	UAudioComponent* AudioComp = nullptr;
	const float FinalVolume = Params.VolumeMultiplier * MasterVolume;
	const float FinalPitch = Params.PitchMultiplier;

	if (Params.bAttachSound && Instigator)
	{
		AudioComp = UGameplayStatics::SpawnSoundAttached(
			Sound,
			Instigator->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::SnapToTarget,
			false,
			FinalVolume,
			FinalPitch
		);
	}
	else
	{
		AudioComp = UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			Sound,
			Params.Location,
			FRotator::ZeroRotator,
			FinalVolume,
			FinalPitch
		);
	}
	return AudioComp;
}

UNiagaraComponent* USurfaceImpactSubsystem::SpawnVisualEffect(const FSurfaceImpactEffect& Effect, const FSurfaceImpactParams& Params)
{
	if (!GetWorld() || Effect.NiagaraSystem.IsNull())
	{
		return nullptr;
	}

	UNiagaraSystem* System = Effect.NiagaraSystem.LoadSynchronous();
	if (!System)
	{
		return nullptr;
	}
	const FRotator Rotation = Params.Normal.Rotation();
	const FVector Scale = FVector(Params.VFXScale * MasterVFXScale);

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		System,
		Params.Location,
		Rotation,
		Scale,
		true,
		true,
		ENCPoolMethod::AutoRelease
	);
	return NiagaraComp;
}

void USurfaceImpactSubsystem::SpawnDecal(const FSurfaceImpactEffect& Effect, const FSurfaceImpactParams& Params)
{
	if (!GetWorld() || Effect.DecalMaterial.IsNull())
	{
		return;
	}

	UMaterialInterface* Material = Effect.DecalMaterial.LoadSynchronous();
	if (!Material)
	{
		return;
	}

	const FRotator DecalRotation = Params.Normal.Rotation();

	UGameplayStatics::SpawnDecalAtLocation(
		GetWorld(),
		Material,
		Effect.DecalSize,
		Params.Location,
		DecalRotation,
		Effect.DecalLifeTime
	);
}

void USurfaceImpactSubsystem::DrawDebugImpact(const FSurfaceImpactParams& Params, bool bSuccess)
{
	if (!GetWorld() || !bDebugDraw)
	{
		return;
	}

	const FColor Color = bSuccess ? FColor::Green : FColor::Red;

	DrawDebugSphere(GetWorld(), Params.Location, 10.f, 8, Color, false, DebugDrawDuration, 0, 2.f);
	DrawDebugLine(GetWorld(), Params.Location, Params.Location + Params.Normal * 50.f, FColor::Blue, false, DebugDrawDuration, 0, 2.f);

	FString DebugText = FString::Printf(TEXT("Impact: %s\n%s"),
		*UEnum::GetValueAsString(Params.ImpactType),
		bSuccess ? TEXT("SUCCESS") : TEXT("FAILED")
	);

	DrawDebugString(GetWorld(), Params.Location + FVector(0, 0, 20), DebugText, nullptr, Color, DebugDrawDuration);
}
