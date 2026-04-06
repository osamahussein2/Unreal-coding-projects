// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/HealthPotion/HealthPotion.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/HealthComponent.h"
#include "GameMode/DungeonCrawlerGM.h"
//#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AHealthPotion::AHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	potionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionMesh"));
	SetRootComponent(potionMesh);

	potionMesh->bHiddenInSceneCapture = true;

	potionMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	potionMesh->SetGenerateOverlapEvents(true);

	potionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	potionMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	potionCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("PotionCollider"));
	potionCollider->SetupAttachment(potionMesh);

	/*potionProjectile = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	potionProjectile->InitialSpeed = potionMoveSpeed;
	potionProjectile->MaxSpeed = potionMoveSpeed;
	potionProjectile->bRotationFollowsVelocity = true;
	potionProjectile->ProjectileGravityScale = 1.0f;
	potionProjectile->Velocity = FVector::ZeroVector;

	InitialLifeSpan = 0;*/
}

// Called when the game starts or when spawned
void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();

	potionCollider->OnComponentBeginOverlap.AddDynamic(this, &AHealthPotion::OnHealthPotionBeginOverlap);
}

void AHealthPotion::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (player)
	{
		// Get a safe normalized direction vector
		FVector LaunchDirection = (player->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		// Launch the projectile upward and towards the direction vector
		FRotator LaunchRotation(LaunchDirection.Rotation().Pitch + throwAngle,
			LaunchDirection.Rotation().Yaw, 0.0f);

		SetActorRotation(initialRotation);

		// Launch the projectile using velocity
		potionProjectile->Velocity = LaunchRotation.Vector() * potionMoveSpeed;
	}*/
}