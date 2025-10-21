#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimonMonolith.generated.h"

class UMaterialInstanceDynamic;
class UArrowComponent;

UENUM(BlueprintType)
enum class EMonolithState : uint8
{
	Off,
	Showing,
	LockedOn,
	ErrorRed
};

UCLASS()
class GDTV_HALLOWEEN2025_API ASimonMonolith : public AActor
{
	GENERATED_BODY()
public:
	ASimonMonolith();

	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere) UArrowComponent* SpawnArrow;

	UPROPERTY(EditAnywhere, Category="Material") int32 MaterialIndex = 0;
	UPROPERTY(EditAnywhere, Category="Material") FName EmissiveStrengthParam = TEXT("EmissiveStrength");
	UPROPERTY(EditAnywhere, Category="Material") FName EmissiveColorParam    = TEXT("EmissiveColor");
	UPROPERTY(EditAnywhere, Category="Material") float StrengthOn  = 40.f;
	UPROPERTY(EditAnywhere, Category="Material") float StrengthOff = 0.f;
	UPROPERTY(EditAnywhere, Category="Material") FLinearColor ShowColor  = FLinearColor(0.2f,0.8f,1.f);
	UPROPERTY(EditAnywhere, Category="Material") FLinearColor LockColor  = FLinearColor(0.15f,1.f,0.25f);
	UPROPERTY(EditAnywhere, Category="Material") FLinearColor ErrorColor = FLinearColor(1.f,0.2f,0.2f);

	void SetState(EMonolithState NewState);
	EMonolithState GetState() const { return State; }
	FVector GetSpawnLocation() const;

protected:
	virtual void BeginPlay() override;
	void Apply(float Strength, const FLinearColor& Color);

private:
	UPROPERTY(Transient) UMaterialInstanceDynamic* MID = nullptr;
	EMonolithState State = EMonolithState::Off;
};
