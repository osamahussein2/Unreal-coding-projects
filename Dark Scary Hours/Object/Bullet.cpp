// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/Bullet.h"
#include "Components/BoxComponent.h"
#include "EhGameJam/EhGameJam.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayerCharacter.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	bulletMesh->SetupAttachment(GetRootComponent());

	bulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bulletMesh->SetCollisionObjectType(COLLISION_BULLET);
	bulletMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	bulletCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BulletCollision"));
	bulletCollision->SetupAttachment(bulletMesh);

	bulletCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bulletCollision->SetCollisionObjectType(COLLISION_BULLET);
	bulletCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	bulletCollision->SetCollisionResponseToChannel(COLLISION_PLAYER, ECR_Ignore);
	bulletCollision->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	bulletCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	bulletCollision->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	bulletCollision->SetGenerateOverlapEvents(true);
}

void ABullet::SetDamage(float& damageAmount_)
{
	damage = damageAmount_;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	//bulletMesh->IgnoreActorWhenMoving(this, true);
	//bulletMesh->IgnoreActorWhenMoving(Cast<AWeapon>(AWeapon::StaticClass()), true);

	bulletCollision->IgnoreActorWhenMoving(this, true);

	//bulletCollision->OnComponentHit.AddDynamic(this, &ABullet::OnBulletHit);
	bulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletBeginOverlap);

	bulletMesh->SetCastShadow(false);
	bulletMesh->SetHiddenInSceneCapture(true);

	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerCharacter::StaticClass(), foundPlayer);

	APlayerCharacter* player = Cast<APlayerCharacter>(foundPlayer[0]);

	if (player)
	{
		startingPosition = player->GetActorLocation();

		foundPlayer[0] = nullptr;
		player = nullptr;
	}

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ABullet::DestroyBullet, bulletLifeTime, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move the bullet towards its set destination
	FVector bulletLocation = GetActorLocation();
	const FVector& bulletNextLocation = bulletDirection;

	bulletLocation += (bulletNextLocation - GetActorLocation()).GetSafeNormal() * bulletSpeed * DeltaTime;

	SetActorLocation(bulletLocation);

	// Destroy bullet when it goes beyond a set range or it isn't moving anymore
	if (FVector::Distance(startingPosition, GetActorLocation()) >= range || GetActorMagnitude() <= 0.1f) Destroy();
}

void ABullet::SetBulletDirection(FVector direction_)
{
	bulletDirection = direction_;
}

void ABullet::SetBulletRange(float& rangeAmount_, float offsetValue_)
{
	range = rangeAmount_ + offsetValue_;
}

float ABullet::GetActorMagnitude() const
{
	return FMath::Sqrt(GetActorLocation().X * GetActorLocation().X +
		GetActorLocation().Y * GetActorLocation().Y +
		GetActorLocation().Z * GetActorLocation().Z);
}

void ABullet::DestroyBullet()
{
	Destroy();
}

void ABullet::OnBulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AController* controller = GetInstigator()->GetController();

	if (!OtherActor || OtherActor == this || !controller) return;

	UGameplayStatics::ApplyPointDamage(OtherActor, damage, GetVelocity().GetSafeNormal(), Hit, controller, this, nullptr);

	UE_LOG(LogTemp, Warning, TEXT("Hit something"));

	Destroy();
}

void ABullet::OnBulletBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AController* controller = GetInstigator()->GetController();

	if (!OtherActor || OtherActor == this || !controller || OtherActor == GetInstigator()) return;

	UGameplayStatics::ApplyPointDamage(OtherActor, damage, GetVelocity().GetSafeNormal(), FHitResult(), controller, this, nullptr);

	Destroy();
}