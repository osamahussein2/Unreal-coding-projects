// Fill out your copyright notice in the Description page of Project Settings.

#include "Trigger/CookTrigger.h"
#include "Components/BoxComponent.h"
#include "Player/PlayerCharacter.h"
#include "GameMode/PortfolioBuilders_GM.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACookTrigger::ACookTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	triggerBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACookTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	triggerBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACookTrigger::OnComponentBeginOverlap);
	triggerBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ACookTrigger::OnComponentEndOverlap);
}

// Called every frame
void ACookTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACookTrigger::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && !triggerOverlapped)
	{
		if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				if (player->GetEPlayerState() != EPlayerState::ECarryingFood && player->GetReceivedOrderFromCustomer())
				{
					player->SpawnCookingComponent();

					gameMode->InitializeInstructionWidget("Press SPACE to make meal");

					if (gameMode->GetMobileWidget())
					{
						gameMode->GetMobileWidget()->ShowInteractionButton();
						gameMode->GetMobileWidget()->SetInteractionText("Make meal");
					}

					triggerOverlapped = true;
				}
			}
		}
	}
}

void ACookTrigger::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && triggerOverlapped)
	{
		if (APortfolioBuilders_GM* gameMode = Cast<APortfolioBuilders_GM>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				player->DestroyCookingComponent();
			}

			gameMode->DestroyInstructionWidget();

			if (gameMode->GetMobileWidget()) gameMode->GetMobileWidget()->HideInteractionButton();
		}

		triggerOverlapped = false;
	}
}

