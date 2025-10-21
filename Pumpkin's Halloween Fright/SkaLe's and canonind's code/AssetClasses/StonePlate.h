#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StonePlate.generated.h"

class UBoxComponent;
class UMaterialInstanceDynamic;
class AStoneplateGroup;

UCLASS()
class GDTV_HALLOWEEN2025_API AStoneplate : public AActor
{
	GENERATED_BODY()

public:
	AStoneplate();

	// Called by the group
	void ActivatePlate();
	void DeactivatePlate();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stoneplate")
	class UStaticMeshComponent* StoneMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stoneplate")
	UBoxComponent* BoxTrigger;

	// Assigned in editor per plate
	UPROPERTY(EditInstanceOnly, Category="Stoneplate|Group")
	AStoneplateGroup* Group = nullptr;

	// Material control
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stoneplate|Material")
	FName EmissiveParamName = TEXT("EmissiveStrength");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stoneplate|Material")
	float EmissiveOn = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stoneplate|Material")
	float EmissiveOff = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stoneplate|Material")
	int32 MaterialElementIndex = 0;

	// Overlap hooks
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
