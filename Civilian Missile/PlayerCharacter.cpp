// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PauseMenuActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"


int APlayerCharacter::maxHealth = 100;
int APlayerCharacter::currentHealth = maxHealth;

int APlayerCharacter::maxPower = 100;
int APlayerCharacter::currentPower = maxPower;

float APlayerCharacter::currentTime = 60.0f;

bool APlayerCharacter::gamePaused = false;

bool APlayerCharacter::inMenu = true;

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	currentHealth = maxHealth;
	currentPower = maxPower;

	currentTime = maxTime;

	gamePaused = false;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!gamePaused && !inMenu)
	{
		// Update current time if it's greater than 0, otherwise show the game over screen
		if (currentTime <= 0) UGameplayStatics::OpenLevel(this, "GameOverScreen");
		else currentTime -= DeltaTime;

		// Show the game over screen when player's current health reaches 0
		if (currentHealth <= 0) UGameplayStatics::OpenLevel(this, "GameOverScreen");
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("PauseGame"), IE_Pressed, this, &APlayerCharacter::PauseGame);
}

bool APlayerCharacter::IsGamePaused()
{
	return gamePaused;
}

void APlayerCharacter::SetGamePaused(bool gamePaused_)
{
	gamePaused = gamePaused_;
}

void APlayerCharacter::PauseGame()
{
	// Change between game paused booleans
	if (gamePaused == false) gamePaused = true;
	else if (gamePaused == true) gamePaused = false;

	if (APauseMenuActor::pauseStateChanged != false) APauseMenuActor::pauseStateChanged = false;
}

void APlayerCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* OtherActorOwner = OtherActor->GetOwner();
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (OtherActorOwner != PlayerPawn)
	{

		if (OtherActor->IsA(enemyFish->StaticClass()))
		{
			currentHealth -= 1;
		}

		if (OtherActor->IsA(enemyLaser->StaticClass()))
		{
			currentHealth -= 1;
		}

		if (OtherActor->IsA(enemyRocket->StaticClass()))
		{
			currentHealth -= 1;
		}

		if (OtherActor->IsA(enemySmoke->StaticClass()))
		{
			currentHealth -= 1;
		}
	}
}