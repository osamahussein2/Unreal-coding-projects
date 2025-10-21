// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIMeshInteractor.h"
#include "UI/UIManager.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"




UUIMeshInteractor::UUIMeshInteractor()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	//PrimaryComponentTick.TickInterval = 0.016f; // Improves performance, but introduces glitch with debug visuals

	LastWidgetMousePosition = FVector2D::ZeroVector;
	RenderTargetSize = FVector2D(1920.f, 1090.f); // Get size later from UIManager

}

void UUIMeshInteractor::BeginPlay()
{
	Super::BeginPlay();

	if (!PlayerController)
	{
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	}
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("UIMeshInteractor: PlayerController not found!"));
		return;
	}

	if (!TargetMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIMeshInteractor: TargetMesh not set!"));
	}
	if (!UIManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIMeshInteractor: UIManager not set!"));
	}
	CreateWidgetInteraction();
}

void UUIMeshInteractor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bWidgetComponentCached)
	{
		CacheWidgetComponent();
	}

	if (!IsWidgetInteractionValid()) return;

	bEnableInteraction = UIManager->IsRenderingToTarget();
	if (!bEnableInteraction) return;

	ProcessInteraction(DeltaTime);
}



void UUIMeshInteractor::ProcessInteraction(float DeltaTime)
{
	if (!PlayerController || !TargetMesh || !CachedWidgetComponent) return;

	FVector HitLocation;
	FVector2D UV;
	bool bHitMesh = TraceForUIMesh(HitLocation, UV);
	
	if (bHitMesh)
	{
		CurrentHitLocation = HitLocation;
		FVector2D WidgetPosition = WorldToWidgetSpace(UV);

		// 		if (bDebugDrawRays)
		// 		{
		// 			UE_LOG(LogTemp, Log, TEXT("Hit mesh at UV: (%f, %f) -> Widget Pos: (%f, %f)"),
		// 				UV.X, UV.Y, WidgetPosition.X, WidgetPosition.Y);
		// 		}
		UpdateWidgetInteractionTransform(WidgetPosition);
		LastWidgetMousePosition = WidgetPosition;
		bIsHittingMesh = true;
	}
	else
	{
		bIsHittingMesh = false;
	}
	if (bForwardMouseInput)
	{
		PollMouseInput();
	}
}

void UUIMeshInteractor::PollMouseInput()
{
	if (!PlayerController || !bIsHittingMesh) return;

	auto HandleMouseButton = [&](const FKey& Key, bool& WasPressed)
		{
			bool bPressed = PlayerController->IsInputKeyDown(Key);
			if (bPressed != WasPressed)
			{
				SendPointerEvent(Key, bPressed);
				WasPressed = bPressed;
			}
		};

	HandleMouseButton(EKeys::LeftMouseButton, bWasLeftMousePressed);
	HandleMouseButton(EKeys::RightMouseButton, bWasRightMousePressed);
	HandleMouseButton(EKeys::MiddleMouseButton, bWasMiddleMousePressed);
}

bool UUIMeshInteractor::TraceForUIMesh(FVector& OutHitLocation, FVector2D& OutUVCoordinates)
{
	if (!PlayerController || !TargetMesh) return false;

	float MouseX, MouseY;
	if (!PlayerController->GetMousePosition(MouseX, MouseY)) return false;

	FVector WorldLocation, WorldDirection;
	if (!PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection)) return false;

	FVector TraceEnd = WorldLocation + (WorldDirection * MaxInteractionDistance);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;
	QueryParams.AddIgnoredActor(PlayerController->GetPawn());

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		WorldLocation,
		TraceEnd,
		MeshTraceChannel,
		QueryParams
	);

	if (bDebugDrawRays && bHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.Location, 10.0f, 8, FColor::Yellow, false, 0.0f);
	}

	if (bHit && HitResult.GetComponent() == TargetMesh)
	{
		OutHitLocation = HitResult.Location;
		FVector2D UV;
		if (UGameplayStatics::FindCollisionUV(HitResult, 0, UV))
		{
			OutUVCoordinates = UV;
			return true;
		}
	}
	return false;
}

FVector2D UUIMeshInteractor::WorldToWidgetSpace(const FVector2D& UV)
{
	FVector2D PixelPosition;
	PixelPosition.X = UV.X * RenderTargetSize.X;
	PixelPosition.Y = UV.Y * RenderTargetSize.Y;

	PixelPosition.X = FMath::Clamp(PixelPosition.X, 0.0f, RenderTargetSize.X);
	PixelPosition.Y = FMath::Clamp(PixelPosition.Y, 0.0f, RenderTargetSize.Y);

	return PixelPosition;
}


void UUIMeshInteractor::UpdateWidgetInteractionTransform(const FVector2D& WidgetPosition)
{
	if (!CachedWidgetComponent || !WidgetInteraction) return;

	FVector2D NormalizedPos = FVector2D(
		WidgetPosition.X / RenderTargetSize.X,
		WidgetPosition.Y / RenderTargetSize.Y
	);

	FVector2D LocalPosition = (NormalizedPos - FVector2D(0.5f, 0.5f)) * RenderTargetSize;

	FVector ComponentLocation = CachedWidgetComponent->GetComponentLocation();
	FVector RightVector = CachedWidgetComponent->GetRightVector();
	FVector UpVector = CachedWidgetComponent->GetUpVector();
	FVector ForwardVector = CachedWidgetComponent->GetForwardVector();

	FVector WorldLocation = ComponentLocation
		+ RightVector * LocalPosition.X
		+ UpVector * LocalPosition.Y
		+ ForwardVector * 10.f;

	WidgetInteraction->SetWorldLocation(WorldLocation);
	WidgetInteraction->SetWorldRotation((-ForwardVector).Rotation());
}



void UUIMeshInteractor::SendPointerEvent(const FKey& Key, bool bIsPressed)
{
	if (!IsWidgetInteractionValid() || !bIsHittingMesh) return;
	if (bIsPressed)
	{
		WidgetInteraction->PressPointerKey(Key);
	}
	else
	{
		WidgetInteraction->ReleasePointerKey(Key);
	}

	CachedWidgetComponent->RequestRedraw();
}

void UUIMeshInteractor::SendMouseWheelEvent(float WheelDelta)
{
	if (!IsWidgetInteractionValid() || !bIsHittingMesh)return;
	WidgetInteraction->ScrollWheel(WheelDelta);
}

bool UUIMeshInteractor::CacheWidgetComponent()
{
	if (!UIManager) return false;

	AUIManager* UIManagerInstance = Cast<AUIManager>(UIManager);
	if (!UIManagerInstance) return false;

	UActorComponent* Component = UIManagerInstance->GetUIRenderComponent();
	CachedWidgetComponent = Cast<UWidgetComponent>(Component);

	if (CachedWidgetComponent)
	{
		RenderTargetSize = CachedWidgetComponent->GetDrawSize();
		bWidgetComponentCached = true;
		return true;
	}
	return false;
}

void UUIMeshInteractor::CreateWidgetInteraction()
{
	if (!WidgetInteraction && GetOwner())
	{
		WidgetInteraction = NewObject<UWidgetInteractionComponent>(GetOwner(), UWidgetInteractionComponent::StaticClass(), TEXT("WidgetInteractor"));
		if (WidgetInteraction)
		{
			WidgetInteraction->RegisterComponent();
			WidgetInteraction->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			WidgetInteraction->InteractionDistance = 1000.f;
			WidgetInteraction->PointerIndex = 0;
			WidgetInteraction->bShowDebug = bDebugDrawRays;
			WidgetInteraction->bEnableHitTesting = true;
			WidgetInteraction->InteractionSource = EWidgetInteractionSource::World;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UIMeshInteractor: Failed to create WidgetInteractionComponent"));
		}
	}
}

bool UUIMeshInteractor::IsWidgetInteractionValid() const
{
	return WidgetInteraction != nullptr && CachedWidgetComponent != nullptr;
}

