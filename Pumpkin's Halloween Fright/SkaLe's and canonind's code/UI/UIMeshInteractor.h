// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "UIMeshInteractor.generated.h"

class AUIManager;
class UWidgetComponent;
class UStaticMeshComponent;


/*
* Enables interaction with UI rendered on a world mesh
* Forwards all input to widgets
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GDTV_HALLOWEEN2025_API UUIMeshInteractor : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUIMeshInteractor();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	void ProcessInteraction(float DeltaTime);
	void PollMouseInput();
	bool TraceForUIMesh(FVector& OutHitLocation, FVector2D& OutUVCoordinates);
	FVector2D WorldToWidgetSpace(const FVector2D& UV);
	void UpdateWidgetInteractionTransform(const FVector2D& WidgetPosition);
	void SendPointerEvent(const FKey& Key, bool bIsPressed);
	void SendMouseWheelEvent(float WheelDelta);
	bool CacheWidgetComponent();
	void CreateWidgetInteraction();
	bool IsWidgetInteractionValid() const;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interaction")
	TObjectPtr<UStaticMeshComponent> TargetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interaction")
	TObjectPtr<AUIManager> UIManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interaction", meta = (ClampMin = "100.0", ClampMax = "100000.0"))
	float MaxInteractionDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interaction|Debug")
	bool bDebugDrawRays = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interaction")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interaction")
	TEnumAsByte<ECollisionChannel> MeshTraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interaction")
	bool bEnableInteraction = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Interaction")
	bool bForwardMouseInput = true;

private:
	UPROPERTY()
	TObjectPtr<UWidgetInteractionComponent> WidgetInteraction = nullptr;
	UPROPERTY()
	TObjectPtr<UWidgetComponent> CachedWidgetComponent;

	FVector2D LastWidgetMousePosition;
	FVector2D RenderTargetSize;
	bool bWidgetComponentCached = false;
	bool bIsHittingMesh = false;
	bool bWasLeftMousePressed = false;
	bool bWasRightMousePressed = false;
	bool bWasMiddleMousePressed = false;

	FVector CurrentHitLocation;
};

