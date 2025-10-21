// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ImpactSystem/SurfaceImpactTypes.h"
#include "AnimNotify_SurfaceImpact.generated.h"

/**
 * Anim notify for triggering surface impact effects SurfaceImpactSubsystem
 */
UCLASS()
class GDTV_HALLOWEEN2025_API UAnimNotify_SurfaceImpact : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_SurfaceImpact();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;

protected:
	bool PerformSurfaceTrace(USkeletalMeshComponent* MeshComp, FSurfaceImpactParams& OutParams) const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact")
	ESurfaceImpactType ImpactType = ESurfaceImpactType::Footstep_Walk;

	/* Socket to trace from */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Trace")
	FName SocketName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Trace", meta = (ClampMin = "1.0", ClampMax = "200.0"))
	float TraceDistance = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Trace", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float TraceStartOffset = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Modifiers", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float VFXScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Modifiers", meta = (ClampMin = "0.0", ClampMax = "4.0"))
	float VolumeMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Modifiers", meta = (ClampMin = "0.5", ClampMax = "2.0"))
	float PitchMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Options")
	bool bAttachSound = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Options")
	bool bDisableSound = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Options")
	bool bDisableVFX = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact|Options")
	bool bDisableDecal = false;
};
