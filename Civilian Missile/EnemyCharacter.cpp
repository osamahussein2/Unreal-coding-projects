// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

int AEnemyCharacter::maxEnemyHealth = 100;
int AEnemyCharacter::currentEnemyHealth = maxEnemyHealth;

int AEnemyCharacter::maxEnemyPower = 100;
int AEnemyCharacter::currentEnemyPower = maxEnemyPower;

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	currentEnemyHealth = maxEnemyHealth;
	currentEnemyPower = maxEnemyPower;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!APlayerCharacter::IsGamePaused())
	{
		pauseMenuActor->GetPlayerHUD_UI()->SetEnemyHealthProgressBar();

		// Show the game win screen after enemy's current health reaches 0
		if (currentEnemyHealth <= 0) UGameplayStatics::OpenLevel(this, "GameWinScreen");
	}
}

void AEnemyCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* OtherActorOwner = OtherActor->GetOwner();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (OtherActorOwner == PlayerPawn)
	{
		if (OtherActor->IsA(playerFish->StaticClass()))
		{
			currentEnemyHealth -= 4;
		}

		if (OtherActor->IsA(playerLaser->StaticClass()))
		{
			currentEnemyHealth -= 4;
		}

		if (OtherActor->IsA(playerRocket->StaticClass()))
		{
			currentEnemyHealth -= 4;
		}

		if (OtherActor->IsA(playerSmoke->StaticClass()))
		{
			currentEnemyHealth -= 4;
		}
	}
}