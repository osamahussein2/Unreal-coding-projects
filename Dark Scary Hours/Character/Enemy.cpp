// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Object/EnemyWeapon.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
//#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PaperSpriteComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*minimapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArm"));
	minimapSpringArm->SetupAttachment(GetCapsuleComponent());*/

	sceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapSceneCapture"));
	sceneCaptureComponent->SetupAttachment(GetCapsuleComponent());

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MinimapSprite"));
	spriteComponent->SetupAttachment(GetMesh());

	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (spriteComponent) spriteComponent->bVisibleInSceneCaptureOnly = true;

	static_cast<float&>(currentHealth) = maxHealth;

	if (enemyWeaponClass)
	{
		enemyWeapon = GetWorld()->SpawnActor<AEnemyWeapon>(enemyWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator,
			FActorSpawnParameters());

		enemyWeapon->SetActorScale3D(FVector(100.0f, 100.0f, 100.0f));

		// Attach the weapon onto the player
		enemyWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_rSocket"));

		// Use this to look at player
		//enemyWeapon->SetActorRotation(FRotator(0.0f, enemyWeapon->GetActorRotation().Yaw, 90.0f));

		enemyWeapon->SetActorRotation(FRotator(-90.0f, -90.0f, 0.0f));

		if (enemyWeapon->IsAttachedTo(this) && GetMesh()->DoesSocketExist(FName("hand_rSocket")))
		{
			// Set the weapon location to where the socket is found in the player character mesh
			enemyWeapon->SetActorLocation(GetMesh()->GetSocketLocation(FName("hand_rSocket")));
		}

		FTimerHandle moveWeaponHandle;
		GetWorld()->GetTimerManager().SetTimer(moveWeaponHandle, this, &AEnemy::UpdateSocketLocation,
			GetWorld()->GetDeltaSeconds(), true);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!player && currentHealth > 0.0f) player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (player && enemyWeapon && currentHealth > 0.0f)
	{
		const float& distance = FVector::Distance(GetActorLocation(), player->GetActorLocation());

		if (distance <= enemyWeapon->GetWeaponRange())
		{
			const FRotator& lookAtPlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player->GetActorLocation());
			SetActorRotation(lookAtPlayer);

			// Use this to look at player
			enemyWeapon->SetActorRelativeRotation(FRotator(0.0f, -2.0f, 90.0f));

			fireTimer += DeltaTime;

			if (fireTimer >= 1.0f / enemyWeapon->GetFiringSpeed())
			{
				enemyWeapon->FireWeapon(enemyWeapon->GetSceneComponentDestination()->GetComponentLocation());
				fireTimer = 0.0f;
			}
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	currentHealth -= DamageAmount;

	if (currentHealth <= 0.0f)
	{
		if (enemyWeapon)
		{
			enemyWeapon->Destroy();
			enemyWeapon = nullptr;
		}

		Destroy();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Enemy taken damage: %f"), currentHealth);

	return DamageAmount;
}

void AEnemy::SpawnCrosshair()
{
	if (!crosshairWidgetComponent && crosshairWidgetClass)
	{
		// Create a new billboard widget component object
		crosshairWidgetComponent = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(),
			TEXT("CrosshairWidgetComponent"));

		crosshairWidgetComponent->SetupAttachment(GetMesh());

		// Register the created component during runtime
		crosshairWidgetComponent->RegisterComponent();

		// Set the locked on widget component location
		crosshairWidgetComponent->SetRelativeLocation(widgetComponentSpawnLocation);

		// Also set its widget class and draw size with it
		crosshairWidgetComponent->SetWidgetClass(crosshairWidgetClass);
		crosshairWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

		crosshairWidgetComponent->SetDrawSize(widgetComponentDrawSize);

		lockedOn = true;
	}
}

void AEnemy::DestroyCrosshair()
{
	if (crosshairWidgetComponent && crosshairWidgetComponent->GetWidgetClass())
	{
		// Set widget class to null and destroy the crosshair widget component with it
		crosshairWidgetComponent->SetWidgetClass(nullptr);
		crosshairWidgetComponent->DestroyComponent();
		crosshairWidgetComponent = nullptr;

		lockedOn = false;
	}
}

void AEnemy::UpdateSocketLocation()
{
	if (enemyWeapon->IsAttachedTo(this) && GetMesh()->DoesSocketExist(FName("hand_rSocket")))
	{
		// Set the weapon location to where the socket is found in the player character mesh
		enemyWeapon->SetActorLocation(GetMesh()->GetSocketLocation(FName("hand_rSocket")));
	}
}