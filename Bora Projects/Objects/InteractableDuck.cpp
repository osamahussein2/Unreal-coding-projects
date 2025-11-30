// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/InteractableDuck.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Characters/Enemies/Enemy.h"
#include "Triggers/OutOfBoundsTrigger.h"

// Sets default values
AInteractableDuck::AInteractableDuck()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	duckMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DuckMesh"));
	duckMesh->SetupAttachment(RootComponent);

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(duckMesh);

	boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableDuck::OnComponentBeginOverlap);
	boxComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractableDuck::OnComponentEndOverlap);

	duckMesh->OnComponentHit.AddDynamic(this, &AInteractableDuck::OnComponentHit);

	targetLocation = FVector::Zero();

	startingThrowingAngle = 0.0f;

	isFalling = false;

	startingLocation = FVector::Zero();
	startingRotation = FRotator::ZeroRotator;

	threwPastOutOfBoundsTrigger = false;
}

// Called when the game starts or when spawned
void AInteractableDuck::BeginPlay()
{
	Super::BeginPlay();

	startingThrowingAngle = throwAngle;

	startingLocation = GetActorLocation();
	startingRotation = GetActorRotation();

	materialInst = UMaterialInstanceDynamic::Create(duckMesh->GetMaterial(0), this);
	duckMesh->SetMaterial(0, materialInst); // Assign the dynamic material instance

	if (materialInst) materialInst->SetScalarParameterValue(TEXT("DiffuseColorMapWeight"), 0.0f);

	// Find multiple enemies in the level
	TArray<AActor*> foundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), foundEnemies);
	
	for (AActor* enemy : foundEnemies)
	{
		// Add found enemy actors into the enemies array only if the cast to enemy class is valid
		if (Cast<AEnemy>(enemy))
		{
			enemies.Add(Cast<AEnemy>(enemy));
			enemy = nullptr;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Enemies: %d"), enemies.Num());
}

// Called every frame
void AInteractableDuck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Only tick once the player has thrown the duck to throw it
	if (IsPlayerNotThrowing() && projectileMovement) ThrowDuck(DeltaTime);

	// Make duck fall if isFalling returns true
	if (isFalling) MakeDuckFall(DeltaTime);

	// Let's say the duck drops on the floor, stop the duck from moving
	if (GetActorLocation().Z <= fallOnFloorLocation) RemoveProjectileComponent();

	// Disable duck movement and respawn when it goes out of bounds of the level
	if (threwPastOutOfBoundsTrigger && IsPlayerNotThrowing()) DisableAnyMovement();
}

void AInteractableDuck::ThrowDuck(float& DeltaTime)
{
	// Get a safe normalized direction vector
	FVector LaunchDirection = (targetLocation - GetActorLocation()).GetSafeNormal();

	// Launch the projectile upward and towards the direction vector
	FRotator LaunchRotation(LaunchDirection.Rotation().Pitch + throwAngle, LaunchDirection.Rotation().Yaw, 0.0f);

	// Launch projectile using its homing projectile speed
	if (projectileMovement)
	{
		projectileMovement->Velocity = LaunchRotation.Vector() * projectileMovement->MaxSpeed;
	}
}

void AInteractableDuck::MakeDuckFall(float& DeltaTime)
{
	FVector duckLocation = GetActorLocation();
	FVector fallDirection = FVector(duckLocation.X, duckLocation.Y, duckLocation.Z - 1000.0f);

	duckLocation += (fallDirection - GetActorLocation()).GetSafeNormal() * maxSpeed * DeltaTime;

	SetActorLocation(duckLocation);
}

void AInteractableDuck::RemoveProjectileComponent()
{
	if (isFalling != false) isFalling = false;

	for (int i = 0; i < enemies.Num(); i++)
	{
		if (enemies[i]->GetEnemyState() != EnemyState::EDetectedPlayer &&
			FVector::Distance(enemies[i]->GetActorLocation(), GetActorLocation()) <= duckEnemyDistance)
		{
			if (enemies[i]->GetEnemyState() != EnemyState::ECheckingOutDuck)
			{
				enemies[i]->EnemyIsCloseToDuck();
				enemies[i]->SetEnemyToDetectPlayer();
				enemies[i]->GoToDuckLocation(GetActorLocation());
			}
		}
	}

	if (projectileMovement && IsValid(projectileMovement))
	{
		projectileMovement->DestroyComponent();
		projectileMovement = nullptr;
	}

	SetDuckCanTick(false); // Also disable can tick
}

void AInteractableDuck::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && !insideTrigger)
	{
		if (!player)
		{
			// Find a player in the level
			TArray<AActor*> FoundPlayer;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundPlayer);

			player = Cast<APlayerCharacter>(FoundPlayer[0]);

			FoundPlayer[0] = nullptr;
		}

		// Change the duck's diffuse value upon player trigger
		SetDuckDiffuseValue(true);

		insideTrigger = true;
	}
	
	if (OtherActor->IsA(AOutOfBoundsTrigger::StaticClass()) && IsPlayerCurrentlyThrowing() && 
		!threwPastOutOfBoundsTrigger)
	{
		threwPastOutOfBoundsTrigger = true;
	}
}

void AInteractableDuck::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()) && insideTrigger)
	{
		// Make sure the duck's diffuse value is 0 once the player isn't throwing
		if (IsPlayerNotThrowing())
		{
			SetDuckDiffuseValue(false);
		}

		insideTrigger = false;
	}

	if (OtherActor->IsA(AOutOfBoundsTrigger::StaticClass()) && IsPlayerCurrentlyThrowing() && 
		threwPastOutOfBoundsTrigger)
	{
		threwPastOutOfBoundsTrigger = false;
	}
}

void AInteractableDuck::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// Duck hits floor
	if (OtherActor->ActorHasTag(FName("Floor")) && IsPlayerNotThrowing())
	{
		RemoveProjectileComponent();
	}

	// Duck hits any object in the world
	else if (OtherActor->ActorHasTag(FName("Object")) && IsPlayerNotThrowing())
	{
		if (isFalling != false) isFalling = false;

		// Destroy projectile movement component
		if (projectileMovement && IsValid(projectileMovement))
		{
			projectileMovement->DestroyComponent();
			projectileMovement = nullptr;
		}

		ResetToOriginalLocation();

		SetDuckCanTick(false); // Also disable can tick
	}

	// Anything else is hit, make the duck fall
	else
	{
		isFalling = true;
	}
}

void AInteractableDuck::CreateProjectileComponent()
{
	if (!projectileMovement)
	{
		// Create a new projectile movement component object
		projectileMovement = NewObject<UProjectileMovementComponent>(this, UProjectileMovementComponent::StaticClass(),
			TEXT("ProjectileMovement"));

		// Register the created component during runtime
		projectileMovement->RegisterComponent();

		// Set any projectile movement parameters here
		projectileMovement->InitialSpeed = initialSpeed;
		projectileMovement->MaxSpeed = maxSpeed;
		projectileMovement->bRotationFollowsVelocity = true;
		projectileMovement->ProjectileGravityScale = 1.0f;
	}
}

// Get a right hand reference of FVector and set it equal to the target location (the left-hand variable)
void AInteractableDuck::SetTargetLocation(FVector&& targetLocation_)
{
	targetLocation = targetLocation_;
}

void AInteractableDuck::SetThrowingAngleModifier(float value_)
{
	// Update throw angle when throwing
	throwAngle = startingThrowingAngle + value_;

	// Clamp throw angle between the lowest possible offset and the highest possible offset
	throwAngle = FMath::Clamp(throwAngle, -startingThrowingAngle - throwAngleOffset, 
		startingThrowingAngle + throwAngleOffset);

	//UE_LOG(LogTemp, Warning, TEXT("%f"), throwAngle);
}

// Update duck's diffuse value
// Boolean is being passed in instead of a float to make updating scalar parameter values much easier
void AInteractableDuck::SetDuckDiffuseValue(bool value_)
{
	if (materialInst) materialInst->SetScalarParameterValue(TEXT("DiffuseColorMapWeight"), static_cast<float>(value_));
}

void AInteractableDuck::SetDuckCanTick(bool value_)
{
	// Update duck to either call Tick() every frame or not
	PrimaryActorTick.bCanEverTick = value_;

	//UE_LOG(LogTemp, Warning, TEXT("%d"), value_);
}

void AInteractableDuck::ResetToOriginalLocation()
{
	SetActorLocation(startingLocation);
	SetActorRotation(startingRotation);
}

bool AInteractableDuck::IsPlayerNotThrowing() const
{
	return player && player->GetGameplayTagComponent() && 
		!player->GetGameplayTagComponent()->HasActionTag("PlayerActions.Throwing");
}

bool AInteractableDuck::IsPlayerCurrentlyThrowing() const
{
	return player && player->GetGameplayTagComponent() &&
		player->GetGameplayTagComponent()->HasActionTag("PlayerActions.Throwing");
}

void AInteractableDuck::DisableAnyMovement()
{
	if (isFalling != false) isFalling = false;

	// Destroy projectile movement component
	if (projectileMovement && IsValid(projectileMovement))
	{
		projectileMovement->DestroyComponent();
		projectileMovement = nullptr;
	}

	ResetToOriginalLocation();

	SetDuckCanTick(false); // Also disable can tick

	threwPastOutOfBoundsTrigger = false;
}
