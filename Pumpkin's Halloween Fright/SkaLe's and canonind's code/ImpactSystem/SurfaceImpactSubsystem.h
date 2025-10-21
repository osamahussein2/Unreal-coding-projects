// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ImpactSystem/SurfaceImpactTypes.h"
#include "SurfaceImpactSubsystem.generated.h"

class USurfaceImpactSet;
class UAudioComponent;
class UNiagaraComponent;

/**
 * Manages surface impact effects
 */
UCLASS()
class GDTV_HALLOWEEN2025_API USurfaceImpactSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Surface Impact", meta = (WorldContext = "WorldContextObject"))
	bool SpawnSurfaceImpact(const FSurfaceImpactParams& Params, AActor* Instigator = nullptr);

	/* Helper function to trace and spawn impact effects */
	UFUNCTION(BlueprintCallable, Category = "Surface Impact")
	bool TraceSurfaceImpact(const FVector& Start, const FVector& End, ESurfaceImpactType ImpactType, FSurfaceImpactParams Params, AActor* Instigator = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Surface Impact")
	USurfaceImpactSet* GetSurfaceImpactSet(UPhysicalMaterial* PhysicalMaterial) const;

	UFUNCTION(BlueprintCallable, Category = "Surface Impact")
	void RegisterSurfaceImpactSet(USurfaceImpactSet* ImpactSet);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	TMap<TEnumAsByte<EPhysicalSurface>, TSoftObjectPtr<USurfaceImpactSet>> SurfaceImpactSets;

	/* Default impact set to use when no specific set is found */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
	TSoftObjectPtr<USurfaceImpactSet> DefaultImpactSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration", meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float MasterVolume = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration", meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float MasterVFXScale = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebugDraw = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug", meta = (EditCondition = "bDebugDraw"))
	float DebugDrawDuration = 2.f;

private:
	/* Cache loaded impact sets */
	// Do not save this value
	UPROPERTY(Transient) 
	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<USurfaceImpactSet>> LoadedImpactSets;

private:

	UAudioComponent* SpawnSound(const FSurfaceImpactEffect& Effect, const FSurfaceImpactParams& Params, AActor* Instigator);

	UNiagaraComponent* SpawnVisualEffect(const FSurfaceImpactEffect& Effect, const FSurfaceImpactParams& Params);

	void SpawnDecal(const FSurfaceImpactEffect& Effect, const FSurfaceImpactParams& Params);

	void DrawDebugImpact(const FSurfaceImpactParams& Params, bool bSuccess);
};
