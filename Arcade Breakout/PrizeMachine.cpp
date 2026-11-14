// Fill out your copyright notice in the Description page of Project Settings.

#include "PrizeMachine.h"
#include "Components/BoxComponent.h"
#include "Player/PlayerCharacter.h"
#include "Widget/DialogueWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/WidgetManager.h"
#include "Widget/PlayerHUDWidget.h"

// Sets default values
APrizeMachine::APrizeMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	prizeMachine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrizeMachine"));
	SetRootComponent(prizeMachine);

	prizeMachine->SetCollisionResponseToAllChannels(ECR_Block);
	prizeMachine->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(prizeMachine);

	boxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	boxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);

	boxComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void APrizeMachine::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);

	if (boxComponent)
	{
		boxComponent->OnComponentBeginOverlap.AddDynamic(this, &APrizeMachine::OnComponentBeginOverlap);
	}
}

// Called every frame
void APrizeMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (matInstanceDynamic)
	{
		if (!colorLerpChanged)
		{
			timer += flashSpeed * DeltaTime;

			matInstanceDynamic->SetScalarParameterValue(TEXT("Emissive"), timer);

			if (timer >= 1.0f)
			{
				timer = 1.0f;

				colorLerpChanged = true;
			}
		}

		else
		{
			timer -= flashSpeed * DeltaTime;

			matInstanceDynamic->SetScalarParameterValue(TEXT("Emissive"), timer);

			if (timer <= 0.0f)
			{
				timer = 0.0f;
				colorLerpChanged = false;
			}
		}
	}
}

void APrizeMachine::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		if (playerCharacter->GetCoinsAmount() < playerCharacter->GetTotalCoins()) return;

		if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			playerCharacter->DisableInput(playerController);

			if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
			{
				widgetManager->PopWidget(widgetManager->GetPlayerHUDClass());
				widgetManager->PushWidget(widgetManager->GetPrizeMachineDialogueClass());
			}
		}
	}
}

void APrizeMachine::TurnFlashOn()
{
	if (!prizeMachine) return;

	matInstanceDynamic = UMaterialInstanceDynamic::Create(prizeMachine->GetMaterial(0), this);
	if (!matInstanceDynamic) return;

	prizeMachine->SetMaterial(0, matInstanceDynamic); // Assign the dynamic material instance

	if (timer != 0.0f) timer = 0.0f;

	matInstanceDynamic->SetScalarParameterValue(TEXT("Emissive"), timer);

	SetActorTickEnabled(true);
}