// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Swingable.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "DefaultPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameDevTvGameJam2026/GameDevTvGameJam2026.h"

// Sets default values
ASwingable::ASwingable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(staticMeshComponent);

	staticMeshComponent->SetCollisionResponseToChannel(GRAPPELABLE_TRACE, ECR_Block);
}

// Called when the game starts or when spawned
void ASwingable::BeginPlay()
{
	Super::BeginPlay();
	
	playerCharacter = Cast<ADefaultPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void ASwingable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (playerCharacter)
	{
		if (!playerCharacter->GetGrapplableOrSwingActor() && !widgetComponent &&
			playerCharacter->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Flying &&
			playerCharacter->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
		{
			FHitResult hitResult{};
			FHitResult visibilityHitResult{};

			FCollisionQueryParams queryParams{};

			FCollisionQueryParams visibilityQueryParams{};
			visibilityQueryParams.AddIgnoredActor(playerCharacter);
			visibilityQueryParams.AddIgnoredActor(playerCharacter->GetPlayerCameraManager());
			visibilityQueryParams.AddIgnoredActor(this);

			const FVector& TraceStart = playerCharacter->GetPlayerCameraLocation();
			const FVector& TraceEnd = TraceStart + (playerCharacter->GetPlayerCameraForwardVector() * traceDistance);

			// Make sure only the grappelable objects are shown to tell the player where they will grapple to
			if (GetWorld()->LineTraceSingleByChannel(hitResult, TraceStart, TraceEnd, GRAPPELABLE_TRACE, queryParams) &&
				!GetWorld()->LineTraceSingleByChannel(visibilityHitResult, TraceStart, TraceEnd, ECC_Visibility, visibilityQueryParams))
			{
				SpawnSwingingWidget();

				canSwing = true;
			}
		}

		else if (playerCharacter->GetGrapplableOrSwingActor() && widgetComponent)
		{
			FHitResult hitResult{};
			FHitResult visibilityHitResult{};

			FCollisionQueryParams queryParams{};

			FCollisionQueryParams visibilityQueryParams{};
			visibilityQueryParams.AddIgnoredActor(playerCharacter);
			visibilityQueryParams.AddIgnoredActor(playerCharacter->GetPlayerCameraManager());
			visibilityQueryParams.AddIgnoredActor(this);

			const FVector& TraceStart = playerCharacter->GetPlayerCameraLocation();
			const FVector& TraceEnd = TraceStart + (playerCharacter->GetPlayerCameraForwardVector() * traceDistance);

			GetWorld()->LineTraceSingleByChannel(hitResult, TraceStart, TraceEnd, GRAPPELABLE_TRACE, queryParams);

			// Prevent the player from grappling non-grappelable objects
			if (!GetWorld()->LineTraceSingleByChannel(hitResult, TraceStart, TraceEnd, GRAPPELABLE_TRACE, queryParams) ||
				GetWorld()->LineTraceSingleByChannel(visibilityHitResult, TraceStart, TraceEnd, ECC_Visibility, visibilityQueryParams))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Invalid hit actor"));
				DestroyWidgetComponent();
			}
		}
	}
}

void ASwingable::SpawnSwingingWidget()
{
	if (playerCharacter) playerCharacter->SetGrapplableOrSwingActor(this);

	if (!widgetClass || widgetComponent) return;

	// Create a new widget component object
	widgetComponent = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(), TEXT("WidgetComponent"));
	if (!widgetComponent || !staticMeshComponent) return;

	widgetComponent->SetupAttachment(staticMeshComponent);

	// Register the created component during runtime
	widgetComponent->RegisterComponent();

	widgetComponent->bHiddenInSceneCapture = true;

	widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	// Set the widget component transform values
	widgetComponent->SetWorldLocation(staticMeshComponent->GetComponentLocation());
	widgetComponent->SetWorldRotation(staticMeshComponent->GetComponentRotation());
	widgetComponent->SetRelativeScale3D(FVector::OneVector);

	// Also set its widget class and draw size with it
	widgetComponent->SetWidgetClass(widgetClass);
	widgetComponent->SetDrawSize(FVector2D(500.0f, 500.0f));
}

void ASwingable::DestroyWidgetComponent()
{
	canSwing = false;

	if (playerCharacter) playerCharacter->SetGrapplableOrSwingActor(nullptr);

	if (!widgetComponent) return;

	// Set widget class to null and destroy the widget component with it
	widgetComponent->SetWidgetClass(nullptr);
	widgetComponent->DestroyComponent();
	widgetComponent = nullptr;
}