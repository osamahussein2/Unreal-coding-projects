#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlinkManager.generated.h"

class AGuideBeacon;

USTRUCT(BlueprintType)
struct FRoadBlinkGroup
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) int32 RoadId = 1;
	UPROPERTY(EditAnywhere) TArray<AGuideBeacon*> Beacons;
};

UCLASS()
class GDTV_HALLOWEEN2025_API ABlinkManager : public AActor
{
	GENERATED_BODY()
public:
	ABlinkManager();

	UFUNCTION(BlueprintCallable) void StartBlink(int32 RoadId, float Duration);
	UFUNCTION(BlueprintCallable) void StopAll();

	// sequenced start with stagger (seconds between neighbors)
	UFUNCTION(BlueprintCallable)
	//void StartBlinkSequenced(int32 RoadId, float Duration, float PauseAfter = 0.f);
	void StartBlinkSequenced(int32 RoadId, float Duration, float BlinkHz = 2.0f, bool bReverse = false);


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="BlinkManager") TArray<FRoadBlinkGroup> RoadGroups;

private:
	FTimerHandle StopTimer;
	void StopCurrent();
	static void SortBeaconsAlong(TArray<AGuideBeacon*>& Beacons, const FVector& DirOrZero);

	// Cache lookups
	TArray<AGuideBeacon*>* FindGroup(int32 RoadId);
};
