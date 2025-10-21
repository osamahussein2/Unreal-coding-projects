// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GuideBeacon.generated.h"

UCLASS()
class GDTV_HALLOWEEN2025_API AGuideBeacon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGuideBeacon();

	UFUNCTION(BlueprintCallable, Category="GuideBeacon|Blink")
	void SetBlinkSpeed(float Hz);
	
	// Turn blinking on/off (called by BlinkManager)
	UFUNCTION(BlueprintCallable) void SetBlinking(bool bEnable);

	UFUNCTION(BlueprintCallable, Category="GuideBeacon|Blink")
	void SetPhaseOffset(float Seconds);

	UPROPERTY(EditAnywhere, Category="GuideBeacon|Blink")
	bool bUsePhaseOffset = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GuideBeacon|Blink")
	int32 PairIndex = 0;  // same value for both pumpkins in a left–right pair

	UFUNCTION(CallInEditor) void DebugBlink()
	{
		SetBlinkSpeed(2.0f);
		SetPhaseOffset(0.0f);
		SetBlinking(true);
	}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere) class UStaticMeshComponent* Mesh = nullptr;

	
	// Material setup
	UPROPERTY(EditAnywhere, Category="GuideBeacon|Material") FName EmissiveOnOff = TEXT("Blink");
	UPROPERTY(EditAnywhere, Category="GuideBeacon|Material") FName EmissivePhase = TEXT("BlinkPhaseSec");
	UPROPERTY(EditAnywhere, Category="GuideBeacon|Material") FName EmissiveSpeed = TEXT("BlinkSpeed");
	UPROPERTY(EditAnywhere, Category="GuideBeacon|Material") int32 MatElement = 0;
	UPROPERTY(EditAnywhere, Category="GuideBeacon|Blink") float EmissiveMin = 2.f;
	UPROPERTY(EditAnywhere, Category="GuideBeacon|Blink") float EmissiveMax = 30.f;
	UPROPERTY(EditAnywhere, Category="GuideBeacon|Blink") float BlinkSpeed = 4.f;

private:
	UPROPERTY(Transient) class UMaterialInstanceDynamic* MatID = nullptr;
	bool bBlinking = false;
	float TimeAcc = 0.f;
	float PhaseOffset = 0.f;

	
};
