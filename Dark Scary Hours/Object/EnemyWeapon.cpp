// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/EnemyWeapon.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "Engine/CollisionProfile.h"
#include "EhGameJam/EhGameJam.h"
#include "Kismet/GameplayStatics.h"
#include "Object/EnemyBullet.h"


// Sets default values
AEnemyWeapon::AEnemyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	weaponMesh->SetupAttachment(GetRootComponent());

	weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	weaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	sceneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SceneComponent"));
	sceneComponent->SetupAttachment(weaponMesh);

	sceneComponentDestination = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponentDestination"));
	sceneComponentDestination->SetupAttachment(sceneComponent);

	weaponSolidCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponSolidCollision"));
	weaponSolidCollision->SetupAttachment(GetRootComponent());

	weaponSolidCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	weaponSolidCollision->SetCollisionResponseToAllChannels(ECR_Block);
	weaponSolidCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

// Called when the game starts or when spawned
void AEnemyWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	clipsInMagazine = maxClips;
}

// Called every frame
void AEnemyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyWeapon::FireWeapon(FVector&& directionVector_)
{
	if (clipsInMagazine > 0)
	{
		--clipsInMagazine;

		SpawnWeapon(directionVector_);
	}

	else
	{
		FTimerHandle reloadHandle;
		GetWorld()->GetTimerManager().SetTimer(reloadHandle, this, &AEnemyWeapon::ReloadWeapon, reloadSpeed, false);
	}
}

void AEnemyWeapon::SpawnWeapon(FVector& directionVector_)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (bulletClass)
	{
		AEnemyBullet* bullet = GetWorld()->SpawnActor<AEnemyBullet>(bulletClass, sceneComponent->GetComponentLocation(),
			sceneComponent->GetComponentRotation(), spawnParams);

		if (!bullet) return;

		bullet->SetBulletDirection(directionVector_);
		bullet->SetBulletRange(weaponRange, 0.3f);
		bullet->SetDamage(weaponDamage);

		bullet->SetActorScale3D(bulletSize);

		bullet = nullptr;
	}
}

void AEnemyWeapon::ReloadWeapon()
{
	if (clipsInMagazine <= 0)
	{
		clipsInMagazine = maxClips;
	}
}
