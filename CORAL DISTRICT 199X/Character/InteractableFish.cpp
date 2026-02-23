// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/InteractableFish.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/PlayerCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameMode/BrackeysGM.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractableFish::AInteractableFish()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	fishInteractionTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("FishInteractionTrigger"));
	fishInteractionTrigger->SetupAttachment(GetCapsuleComponent());

	fishInteractionTrigger->SetGenerateOverlapEvents(true);

	fishInteractionTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	fishInteractionTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECR_Ignore);
}

// Called when the game starts or when spawned
void AInteractableFish::BeginPlay()
{
	Super::BeginPlay();

	if (!startingFishMaterial)
	{
		startingFishMaterial = GetMesh()->GetMaterial(0);
	}
	
	if (fishInteractionTrigger)
	{
		fishInteractionTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &AInteractableFish::OnFishBeginOverlap);
		fishInteractionTrigger->OnComponentEndOverlap.AddUniqueDynamic(this, &AInteractableFish::OnFishEndOverlap);

		startingBoxExtents = fishInteractionTrigger->GetUnscaledBoxExtent();
	}
}

// Called every frame
void AInteractableFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fishDynamicMaterial)
	{
		// Increase emissive color alpha
		if (!emissiveColorChanged)
		{
			saturation += updateMaterialSpeed * DeltaTime;
			if (saturation >= 1.0f) emissiveColorChanged = true;
		}

		// Decrease emissive color alpha
		else
		{
			saturation -= updateMaterialSpeed * DeltaTime;
			if (saturation <= 0.0f) emissiveColorChanged = false;
		}

		//UE_LOG(LogTemp, Warning, TEXT("%f"), saturation);

		// Update the fish dynamic material's saturation value
		fishDynamicMaterial->SetScalarParameterValue(TEXT("Emission_Strength"), saturation);
	}

	if (widgetComponent)
	{
		// Make this rotator a const reference to prevent copying this value every frame
		const FRotator& lookAtRotation = UKismetMathLibrary::FindLookAtRotation(widgetComponent->GetComponentLocation(),
			GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

		// Set the widget component to look at the player's direction
		widgetComponent->SetWorldRotation(FRotator(0.0f, lookAtRotation.Yaw, 0.0f));
	}
}

// Called to bind functionality to input
void AInteractableFish::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AInteractableFish::ConstructInputWidget()
{
	if (!widgetComponent && inputWidgetClass)
	{
		// Create a new widget component object
		widgetComponent = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(), TEXT("WidgetComponent"));

		widgetComponent->SetupAttachment(GetMesh());

		// Register the created component during runtime
		widgetComponent->RegisterComponent();

		widgetComponent->SetWidgetSpace(EWidgetSpace::World);

		// Set the billboard widget component transform values
		widgetComponent->SetRelativeLocation(widgetComponentSpawnLocation);
		widgetComponent->SetRelativeRotation(widgetComponentSpawnRotation);
		widgetComponent->SetRelativeScale3D(widgetComponentSpawnScale);

		// Also set its widget class and draw size with it
		widgetComponent->SetWidgetClass(inputWidgetClass);
		widgetComponent->SetDrawSize(widgetComponentDrawSize);
	}
}

void AInteractableFish::DestroyInputWidget()
{
	if (widgetComponent && widgetComponent->GetWidgetClass())
	{
		// Set widget class to null and destroy the billboard widget component with it
		widgetComponent->SetWidgetClass(nullptr);
		widgetComponent->DestroyComponent();
		widgetComponent = nullptr;
	}
}

void AInteractableFish::DestroyBoxTrigger()
{
	if (fishInteractionTrigger)
	{
		fishInteractionTrigger->OnComponentBeginOverlap.RemoveDynamic(this, &AInteractableFish::OnFishBeginOverlap);
		fishInteractionTrigger->OnComponentEndOverlap.RemoveDynamic(this, &AInteractableFish::OnFishEndOverlap);

		fishInteractionTrigger->UnregisterComponent();
		fishInteractionTrigger->DestroyComponent();
		fishInteractionTrigger = nullptr;
	}
}

void AInteractableFish::InvalidateDynamicMaterial()
{
	if (fishDynamicMaterial)
	{
		saturation = 1.0f;
		emissiveColorChanged = false;

		if (startingFishMaterial) GetMesh()->SetMaterial(0, startingFishMaterial);
		
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			if (!gameMode->GetIsForMobile()) DestroyInputWidget();
		}

		fishDynamicMaterial = nullptr;
	}
}

void AInteractableFish::SpawnBoxTrigger()
{
	if (!fishInteractionTrigger)
	{
		// Create a new widget component object
		fishInteractionTrigger = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("FishInteractTrigger"));

		fishInteractionTrigger->SetupAttachment(GetCapsuleComponent());

		// Register the box trigger component during runtime
		fishInteractionTrigger->RegisterComponent();

		fishInteractionTrigger->SetWorldScale3D(FVector::OneVector);
		fishInteractionTrigger->SetBoxExtent(startingBoxExtents);

		fishInteractionTrigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &AInteractableFish::OnFishBeginOverlap);
		fishInteractionTrigger->OnComponentEndOverlap.AddUniqueDynamic(this, &AInteractableFish::OnFishEndOverlap);
	}
}

void AInteractableFish::OnFishBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && !fishDynamicMaterial)
	{
		fishDynamicMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
		GetMesh()->SetMaterial(0, fishDynamicMaterial); // Assign the dynamic material instance

		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			if (!gameMode->GetIsForMobile()) ConstructInputWidget();
		}
	}
}

void AInteractableFish::OnFishEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && fishDynamicMaterial)
	{
		saturation = 1.0f;
		emissiveColorChanged = false;

		if (startingFishMaterial) GetMesh()->SetMaterial(0, startingFishMaterial);
		
		if (ABrackeysGM* gameMode = Cast<ABrackeysGM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			if (!gameMode->GetIsForMobile()) DestroyInputWidget();
		}

		fishDynamicMaterial = nullptr;
	}
}