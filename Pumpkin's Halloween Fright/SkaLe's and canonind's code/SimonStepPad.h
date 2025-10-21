#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimonStepPad.generated.h"

class UBoxComponent;
class ASimonPuzzle;

UCLASS()
class GDTV_HALLOWEEN2025_API ASimonStepPad : public AActor
{
	GENERATED_BODY()
public:
	ASimonStepPad();

	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere) UBoxComponent* Trigger;

	UPROPERTY(EditInstanceOnly, Category="Simon") ASimonPuzzle* Puzzle = nullptr;
	UPROPERTY(EditAnywhere, Category="Simon") int32 Index = 0; // which monolith this pad represents

	UFUNCTION(BlueprintCallable) void SetPadEnabled(bool bEnabled);
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void HandleEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);


private:
	bool bOccupiedByPlayer = false;
	bool bEnabled = true;
};
