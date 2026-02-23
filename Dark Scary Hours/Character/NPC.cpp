// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/NPC.h"
#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PaperSpriteComponent.h"
#include "Widget/MarkerWidget.h"

// Sets default values
ANPC::ANPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(RootComponent);

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnComponentBeginOverlap);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnComponentEndOverlap);

	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	cubeMesh->SetupAttachment(RootComponent);

	sceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapSceneCapture"));
	sceneCaptureComponent->SetupAttachment(GetCapsuleComponent());

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
	spriteComponent->SetupAttachment(GetMesh());

	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	dialogueIndex = 0;
	transitionToGivenDialogue = 0;
	transitionToCompletedDialogue = 0;

	npcDialogueState = NPCDialogueState::NotGiven;
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	if (spriteComponent) spriteComponent->bVisibleInSceneCaptureOnly = true;

	playerTriggerFound = false;

	dialogueIndex = 0;

	npcDialogueState = NPCDialogueState::NotGiven;

	check(dialogueLines.Num() == typeSpeed.Num());

	initialRotator = GetActorRotation();

	SpawnMarkerWidget();
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANPC::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && !playerTriggerFound)
	{
		if (npcDialogueState == NPCDialogueState::NotGiven) DestroyMarkerWidget();
		if (npcDialogueState == NPCDialogueState::Completed) DestroyMarkerWidget();
		SpawnInstructionWidget();

		playerTriggerFound = true;
	}
}

void ANPC::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && playerTriggerFound)
	{
		DestroyInstructionWidget();

		if (npcDialogueState == NPCDialogueState::NotGiven) SpawnMarkerWidget();
		if (npcDialogueState == NPCDialogueState::Completed) SpawnMarkerWidget();

		playerTriggerFound = false;
	}
}

void ANPC::GoToNextDialogue(int incrementedValue_)
{
	dialogueIndex += incrementedValue_;
}

void ANPC::SetDialogueIndex(int value_)
{
	dialogueIndex = value_;
}

void ANPC::ResetDialogueIndex()
{
	dialogueIndex = 0;
}

void ANPC::SpawnInstructionWidget()
{
	if (!widgetComponent && inputWidgetClass)
	{
		// Create a new widget component object
		widgetComponent = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(), TEXT("WidgetComponent"));

		widgetComponent->SetupAttachment(GetMesh());

		// Register the created component during runtime
		widgetComponent->RegisterComponent();

		widgetComponent->bHiddenInSceneCapture = true;

		widgetComponent->SetWidgetSpace(EWidgetSpace::World);

		// Set the billboard widget component transform values
		widgetComponent->SetRelativeLocation(widgetComponentSpawnLocation);
		widgetComponent->SetRelativeRotation(widgetComponentSpawnRotation);
		widgetComponent->SetRelativeScale3D(widgetComponentSpawnScale);

		// Also set its widget class and draw size with it
		widgetComponent->SetWidgetClass(inputWidgetClass);
		widgetComponent->SetDrawSize(widgetComponentDrawSize);

		GetWorld()->GetTimerManager().SetTimer(rotateWidgetHandle, this, &ANPC::RotateInstructionWidget,
			GetWorld()->GetDeltaSeconds(), true);
	}
}

void ANPC::RotateInstructionWidget()
{
	if (widgetComponent)
	{
		// Make this rotator a const reference to prevent copying this value every frame
		const FRotator& lookAtRotation = UKismetMathLibrary::FindLookAtRotation(widgetComponent->GetComponentLocation(),
			GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

		// Set the widget component to look at the player's direction
		widgetComponent->SetWorldRotation(FRotator(0.0f, lookAtRotation.Yaw, 0.0f));
	}
}

void ANPC::DestroyInstructionWidget()
{
	if (widgetComponent && widgetComponent->GetWidgetClass())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(rotateWidgetHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(rotateWidgetHandle);
		}

		// Set widget class to null and destroy the instruction widget component with it
		widgetComponent->SetWidgetClass(nullptr);
		widgetComponent->DestroyComponent();
		widgetComponent = nullptr;
	}
}

void ANPC::SpawnMarkerWidget()
{
	if (!markerWidgetComponent && markerWidgetClass)
	{
		// Create a new widget component object
		markerWidgetComponent = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(), TEXT("MarkerWidgetComponent"));

		markerWidgetComponent->SetupAttachment(GetMesh());

		// Register the created component during runtime
		markerWidgetComponent->RegisterComponent();

		markerWidgetComponent->bHiddenInSceneCapture = true;

		markerWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

		// Set the billboard widget component transform values
		markerWidgetComponent->SetRelativeLocation(markerWidgetSpawnLocation);
		markerWidgetComponent->SetRelativeRotation(markerWidgetSpawnRotation);
		markerWidgetComponent->SetRelativeScale3D(markerWidgetSpawnScale);

		// Also set its widget class and draw size with it
		markerWidgetComponent->SetWidgetClass(markerWidgetClass);
		markerWidgetComponent->SetDrawSize(markerWidgetDrawSize);

		markerWidget = Cast<UMarkerWidget>(markerWidgetComponent->GetWidget());
		if (!markerWidget) UE_LOG(LogTemp, Warning, TEXT("Marker widget not found"));

		GetWorld()->GetTimerManager().SetTimer(updateDistanceHandle, this, &ANPC::UpdateMarkerWidget,
			GetWorld()->GetDeltaSeconds(), true);
	}
}

void ANPC::UpdateMarkerWidget()
{
	if (markerWidget && GetWorld() && GetWorld()->GetFirstPlayerController() &&
		GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		markerWidget->UpdateDistanceText(GetActorLocation(),
			GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	}
}

void ANPC::DestroyMarkerWidget()
{
	if (markerWidgetComponent && markerWidgetComponent->GetWidgetClass())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(updateDistanceHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(updateDistanceHandle);
		}

		if (markerWidget)
		{
			markerWidget->RemoveFromParent();
			markerWidget = nullptr;
		}

		// Set widget class to null and destroy the marker widget component with it
		markerWidgetComponent->SetWidgetClass(nullptr);
		markerWidgetComponent->DestroyComponent();
		markerWidgetComponent = nullptr;
	}
}
