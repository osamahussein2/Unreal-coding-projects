// ARockGate.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "RockGate.generated.h"

UCLASS()
class GDTV_HALLOWEEN2025_API ARockGate : public AActor
{
	GENERATED_BODY()

public:
	ARockGate();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gate")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gate")
	UStaticMeshComponent* LeftRock;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gate")
	UStaticMeshComponent* RightRock;

	// How far to move each rock when opening (relative space, in cm)
	UPROPERTY(EditAnywhere, Category="Gate|Config")
	FVector LeftOpenOffset = FVector(-250.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category="Gate|Config")
	FVector RightOpenOffset = FVector(250.f, 0.f, 0.f);

	// Seconds to fully open/close
	UPROPERTY(EditAnywhere, Category="Gate|Config", meta=(ClampMin="0.01"))
	float OpenDuration = 1.25f;

	// Easing function for the animation
	UPROPERTY(EditAnywhere, Category="Gate|Config")
	TEnumAsByte<EEasingFunc::Type> EaseFunc = EEasingFunc::EaseInOut;

	// Optional: auto-open on BeginPlay
	UPROPERTY(EditAnywhere, Category="Gate|Config")
	bool bOpenOnBeginPlay = false;

	// Optional: temporarily disable collision while moving (prevents snagging)
	UPROPERTY(EditAnywhere, Category="Gate|Config")
	bool bDisableCollisionWhileMoving = true;

	// API
	UFUNCTION(BlueprintCallable, Category="Gate")
	void OpenGate();

	UFUNCTION(BlueprintCallable, Category="Gate")
	void CloseGate();

	UFUNCTION(BlueprintCallable, Category="Gate")
	void ToggleGate();

	UFUNCTION(BlueprintPure, Category="Gate")
	bool IsOpen() const { return bIsOpen; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	// Closed positions we cache on BeginPlay (RELATIVE to Root)
	FVector LeftClosedRel = FVector::ZeroVector;
	FVector RightClosedRel = FVector::ZeroVector;

	// Derived open positions (closed + offset)
	FVector LeftOpenRel = FVector::ZeroVector;
	FVector RightOpenRel = FVector::ZeroVector;

	// Animation state
	bool  bIsOpen   = false;
	bool  bIsMoving = false;
	bool  bTargetOpen = false;
	float MoveStartTime = 0.f;
	float MoveDuration  = 0.f;

	void StartMove(bool bOpen);
	void ApplyPose(float Alpha01);         // 0=closed, 1=open
	void SetRocksCollision(bool bEnable);
};
