// Fill out your copyright notice in the Description page of Project Settings.


#include "TVs/MainMenuTV.h"
#include "Widgets/MainMenuWidget.h"

// Sets default values
AMainMenuTV::AMainMenuTV()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cubeMesh"));
	cubeMesh->SetupAttachment(RootComponent);

	planeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("planeMesh"));
	planeMesh->SetupAttachment(cubeMesh);

	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	widgetComponent->SetupAttachment(cubeMesh);

	widgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComponent"));
	widgetInteraction->SetupAttachment(widgetComponent);

	widgetInteraction->InteractionDistance = 10000.f;
	widgetInteraction->PointerIndex = 0;
	widgetInteraction->bShowDebug = false;
	widgetInteraction->bEnableHitTesting = true;
	widgetInteraction->InteractionSource = EWidgetInteractionSource::World;

	lastWidgetMousePosition = FVector2D::ZeroVector;
	renderTargetSize = FVector2D::ZeroVector; // Get render target size when main menu widget is found
}

// Called when the game starts or when spawned
void AMainMenuTV::BeginPlay()
{
	Super::BeginPlay();

	if (!playerController)
	{
		playerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	}

	if (widgetComponent && widgetComponent->GetWidget())
	{
		renderTargetSize = widgetComponent->GetDrawSize();
	}

	matInstanceDynamic = UMaterialInstanceDynamic::Create(planeMesh->GetMaterial(0), this);
	planeMesh->SetMaterial(0, matInstanceDynamic); // Assign the dynamic material instance

	EnableUIInput();
}

// Called every frame
void AMainMenuTV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (matInstanceDynamic && widgetComponent)
	{
		matInstanceDynamic->SetTextureParameterValue(FName("Texture"), widgetComponent->GetRenderTarget());
	}

	if (!widgetComponent && !widgetInteraction)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid widget component and/or interaction!"));
		return;
	}

	ProcessInteraction(DeltaTime);
}

void AMainMenuTV::ProcessInteraction(float DeltaTime)
{
	if (!playerController || !planeMesh || !widgetComponent) return;

	FVector HitLocation;
	FVector2D UV;

	if (TraceForUIMesh(HitLocation, UV))
	{
		currentHitLocation = HitLocation;
		FVector2D WidgetPosition = WorldToWidgetSpace(UV);

		UpdateWidgetInteractionTransform(WidgetPosition);
		lastWidgetMousePosition = WidgetPosition;
		meshHit = true;
	}

	else
	{
		meshHit = false;
	}
	
	PollMouseInput();
}

void AMainMenuTV::PollMouseInput()
{
	if (!playerController || !meshHit) return;

	HandleMouseButton(EKeys::LeftMouseButton, leftMousePressed);
}

bool AMainMenuTV::TraceForUIMesh(FVector& OutHitLocation, FVector2D& OutUVCoordinates)
{
	if (!playerController || !planeMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing player controller and/or plane mesh!"));
		return false;
	}

	float MouseX, MouseY;
	if (!playerController->GetMousePosition(MouseX, MouseY)) return false;

	FVector WorldLocation, WorldDirection;
	if (!playerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection)) return false;

	FVector TraceEnd = WorldLocation + (WorldDirection * maxInteractionDistance);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;
	QueryParams.AddIgnoredActor(playerController->GetPawn());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams);

	if (bHit && HitResult.GetComponent() == planeMesh)
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

FVector2D AMainMenuTV::WorldToWidgetSpace(const FVector2D& UV)
{
	FVector2D PixelPosition;
	PixelPosition.X = UV.X * renderTargetSize.X;
	PixelPosition.Y = UV.Y * renderTargetSize.Y;

	PixelPosition.X = FMath::Clamp(PixelPosition.X, 0.0f, renderTargetSize.X);
	PixelPosition.Y = FMath::Clamp(PixelPosition.Y, 0.0f, renderTargetSize.Y);

	return PixelPosition;
}


void AMainMenuTV::UpdateWidgetInteractionTransform(const FVector2D& WidgetPosition)
{
	if (!widgetComponent || !widgetInteraction) return;

	FVector2D NormalizedPos = FVector2D(WidgetPosition.X / renderTargetSize.X, 
		WidgetPosition.Y / renderTargetSize.Y);

	FVector2D LocalPosition = (NormalizedPos - FVector2D(0.5f, 0.5f)) * renderTargetSize;

	FVector ComponentLocation = widgetComponent->GetComponentLocation();
	FVector RightVector = widgetComponent->GetRightVector();
	FVector UpVector = -widgetComponent->GetUpVector(); // Reverse up vector to properly detect mouse hover on buttons
	FVector ForwardVector = widgetComponent->GetForwardVector();

	FVector WorldLocation = ComponentLocation + RightVector * LocalPosition.X * 0.4f
		+ UpVector * LocalPosition.Y * 0.4f + ForwardVector * 20.f;

	widgetInteraction->SetWorldLocation(WorldLocation);
	widgetInteraction->SetWorldRotation((-ForwardVector).Rotation());
}



void AMainMenuTV::SendPointerEvent(const FKey& Key, bool bIsPressed)
{
	if (!widgetComponent || !widgetInteraction || !meshHit) return;

	if (bIsPressed)
	{
		widgetInteraction->PressPointerKey(Key);
	}

	else
	{
		widgetInteraction->ReleasePointerKey(Key);
	}

	widgetComponent->RequestRedraw();
}

void AMainMenuTV::HandleMouseButton(const FKey& Key, bool& WasPressed)
{
	if (playerController->IsInputKeyDown(Key) != WasPressed)
	{
		SendPointerEvent(Key, playerController->IsInputKeyDown(Key));
		WasPressed = playerController->IsInputKeyDown(Key);
	}
}

void AMainMenuTV::EnableUIInput()
{
	FInputModeGameOnly GameInputMode;
	GameInputMode.SetConsumeCaptureMouseDown(true); // Capture mouse down right away
	playerController->SetInputMode(GameInputMode);

	// Make sure the mouse cursor is turned on for clicking on main menu buttons
	if (playerController->bShowMouseCursor != true) playerController->bShowMouseCursor = true;
}
