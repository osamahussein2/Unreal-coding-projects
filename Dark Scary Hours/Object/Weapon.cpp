// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/Weapon.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "Engine/CollisionProfile.h"
#include "EhGameJam/EhGameJam.h"
#include "Kismet/GameplayStatics.h"
#include "Object/Bullet.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	weaponMesh->SetupAttachment(GetRootComponent());

	weaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	weaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	weaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	weaponMesh->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Ignore);
	weaponMesh->SetCollisionResponseToChannel(COLLISION_WEAPON, ECollisionResponse::ECR_Ignore);
	weaponMesh->SetCollisionResponseToChannel(COLLISION_BULLET, ECollisionResponse::ECR_Ignore);

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(weaponMesh);

	glowingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlowingMesh"));
	glowingMesh->SetupAttachment(boxComponent);

	glowingMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	glowingMesh->SetHiddenInSceneCapture(true);

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSpawnLocation"));
	sceneComponent->SetupAttachment(weaponMesh);

	sceneComponentDestination = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSpawnDestination"));
	sceneComponentDestination->SetupAttachment(sceneComponent);

	boxComponent->SetGenerateOverlapEvents(true);

	boxComponent->SetCollisionObjectType(COLLISION_WEAPON);
	boxComponent->SetCollisionResponseToChannel(COLLISION_PLAYER, ECR_Overlap);

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnComponentBeginOverlap);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnComponentEndOverlap);

	SetCanBeDamaged(false);
}

FVector AWeapon::GetDestinationLocation() const
{
	if (sceneComponentDestination)
	{
		return sceneComponentDestination->GetComponentLocation();
	}

	return FVector::ZeroVector;
}

float AWeapon::GetWeaponRange() const
{
	return weaponRange;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	static_cast<int&>(clipsInMagazine) = maxClips;
	static_cast<int&>(totalAmmo) = maxAmmo;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::FireWeapon(FVector&& directionVector_)
{
	if (clipsInMagazine > 0)
	{
		--clipsInMagazine;

		SpawnWeapon(directionVector_);
	}
}

void AWeapon::SpawnWeapon(FVector& directionVector_)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (bulletClass)
	{
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletClass, sceneComponent->GetComponentLocation(),
			sceneComponent->GetComponentRotation(), spawnParams);

		if (!bullet) return;

		bullet->SetBulletDirection(directionVector_);
		bullet->SetBulletRange(weaponRange, 0.3f);
		bullet->SetDamage(weaponDamage);

		bullet->SetActorScale3D(bulletSize);

		bullet = nullptr;
	}
}

void AWeapon::ReloadWeapon()
{
	if (clipsInMagazine < maxClips && totalAmmo > 0)
	{
		// Return the last recorded ammo by setting it equal to total ammo before decrementing total ammo
		lastRecordedAmmo = totalAmmo;

		// Decrement the total ammo count depending on clips in magazine value before reloading
		totalAmmo -= maxClips - clipsInMagazine;

		// Check if total ammo reaches 0
		if (totalAmmo <= 0)
		{
			// Set the clips in magazine value to increment by whatever the last recorded ammo was
			clipsInMagazine += lastRecordedAmmo;
			totalAmmo = 0; // Set total ammo to 0

			return;
		}

		// Set the clips in magazine depending what the value of total ammo in comparison to max clips of the weapon
		if (totalAmmo >= maxClips) clipsInMagazine = maxClips;

		// Set the clips in magazine by subtracting max clips from the inverse of the total ammo of the weapon
		else clipsInMagazine = maxClips - (1 / totalAmmo);
	}
}

void AWeapon::RefillAmmo()
{
	if (totalAmmo != maxAmmo) totalAmmo = maxAmmo;
	if (clipsInMagazine != maxClips) clipsInMagazine = maxClips;
}

bool AWeapon::GetPlayerIsOnWeapon() const
{
	return playerOnWeapon;
}

void AWeapon::DisableCollisions()
{
	weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	weaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AWeapon::EnableCollisions()
{
	weaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	weaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	weaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	weaponMesh->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Ignore);
	weaponMesh->SetCollisionResponseToChannel(COLLISION_WEAPON, ECollisionResponse::ECR_Ignore);
	weaponMesh->SetCollisionResponseToChannel(COLLISION_BULLET, ECollisionResponse::ECR_Ignore);
}

void AWeapon::ShowGlowingMesh()
{
	if (glowingMesh)
	{
		glowingMesh->SetHiddenInGame(false);
	}
}

void AWeapon::HideGlowingMesh()
{
	if (glowingMesh)
	{
		glowingMesh->DestroyComponent();
		glowingMesh = nullptr;
	}

	/*if (boxComponent)
	{
		boxComponent->DestroyComponent();
		boxComponent = nullptr;
	}*/
}

void AWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	if (OtherActor->GetComponentsCollisionResponseToChannel(COLLISION_PLAYER) && !playerOnWeapon)
	{
		playerOnWeapon = true;
	}
}

void AWeapon::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && playerOnWeapon)
	{
		playerOnWeapon = false;
	}
}