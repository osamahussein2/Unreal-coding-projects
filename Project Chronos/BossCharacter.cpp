// Fill out your copyright notice in the Description page of Project Settings.

#include "BossCharacter.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

int ABossCharacter::attackNumber = -1;
float ABossCharacter::attackTime = 0.0f;

bool ABossCharacter::isDead = false;

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	health = maxHealth;

	canPlayerLandAttack = false;
	playerAttackLanded = false;

	canBossLandAttack = false;
	bossAttackLanded = false;

	attackNumber = -1;

	attackTime = 0.0f;

	attackCombo = 0;
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();

	startingPosition = GetActorLocation();

	health = maxHealth;

	canPlayerLandAttack = false;
	playerAttackLanded = false;

	canBossLandAttack = false;
	bossAttackLanded = false;

	attackNumber = -1;
	attackTime = 0.0f;
	attackCombo = 0;

	isDead = false;
	deathTime = 0.0f;

	isAttacked = false;
	damagedTime = 0.0f;

	// Find the player character in the level
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		playerCharacter = Cast<APlayerCharacter>(Actor);
	}
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (health > 0.0f && !APlayerCharacter::IsGamePaused())
	{
		FVector playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

		// Start AI move to player after logic if player is within their threshold
		if (FVector::Distance(GetActorLocation(), playerLocation) <= distanceThreshold)
		{
			if (ABossAIController::playerCloseToAI != true) ABossAIController::playerCloseToAI = true;

			// Look for landing an attack
			if (FVector::Distance(GetActorLocation(), playerLocation) <= 165.0f && !canBossLandAttack &&
				!isAttacked)
			{
				if (canBossLandAttack != true) canBossLandAttack = true;
			}

			// Start attacking the player
			else if (FVector::Distance(GetActorLocation(), playerLocation) <= 165.0f && canBossLandAttack &&
				!isAttacked)
			{
				PerformAttack(DeltaTime);
				PerformAttackCombos();
				DamagePlayer();
			}

			// Reset attack variables and get out of attack animations
			else if (FVector::Distance(GetActorLocation(), playerLocation) > 165.0f)
			{
				if (attackCombo != 0) attackCombo = 0;
				if (attackNumber >= 0) attackNumber = -1;

				if (canBossLandAttack != false) canBossLandAttack = false;
			}

			if (!isAttacked)
			{
				if (damagedTime != 0.0f) damagedTime = 0.0f;
			}
		}

		else
		{
			if (ABossAIController::playerCloseToAI != false) ABossAIController::playerCloseToAI = false;
			if (attackCombo != 0) attackCombo = 0;
			if (attackNumber >= 0) attackNumber = -1;

			if (canBossLandAttack != false) canBossLandAttack = false;

			if (!isAttacked)
			{
				if (damagedTime != 0.0f) damagedTime = 0.0f;
			}
		}

		if (APlayerCharacter::GetAttackNumber() <= -1 && playerAttackLanded != false)
		{
			playerAttackLanded = false;
		}

		if (attackNumber <= -1 && bossAttackLanded != false)
		{
			bossAttackLanded = false;
		}

		if (FVector::Distance(GetActorLocation(), playerLocation) <= 200.0f && !canPlayerLandAttack)
		{
			if (canPlayerLandAttack != true) canPlayerLandAttack = true;
		}

		else
		{
			if (canPlayerLandAttack != false) canPlayerLandAttack = false;
		}

		DamageLogic();

		if (isDead != false) isDead = false;
		if (deathTime != 0.0f) deathTime = 0.0f;

		AnimateAttacked(DeltaTime);
		LookAtPlayer();
	}

	else if (health <= 0.0f && !APlayerCharacter::IsGamePaused())
	{
		if (deathTime <= 1.1f) deathTime += DeltaTime;
		if (attackNumber >= 0) attackNumber = -1;
		if (attackCombo > 0) attackCombo = 0;
		if (isDead != true) isDead = true;
	}
}

FVector ABossCharacter::GetStartingPosition() const
{
	return startingPosition;
}

float ABossCharacter::GetHealth() const
{
	return health;
}

void ABossCharacter::ResetBoss()
{
	health = maxHealth;
}

void ABossCharacter::DamageLogic()
{
	if (canPlayerLandAttack && !playerAttackLanded && APlayerCharacter::GetAttackNumber() == 0 &&
		APlayerCharacter::GetAttackingTime() >= 0.17f && APlayerCharacter::GetAttackingTime() <= 0.4f)
	{
		UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Boss_Hurt")));

		health -= FMath::FRandRange(minimumHealthLost, maximumHealthLost);
		isAttacked = true;
		playerAttackLanded = true;
	}

	else if (canPlayerLandAttack && !playerAttackLanded && APlayerCharacter::GetAttackNumber() == 1 &&
		APlayerCharacter::GetAttackingTime() >= 0.3f && APlayerCharacter::GetAttackingTime() <= 0.67f)
	{
		UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Boss_Hurt")));

		health -= FMath::FRandRange(minimumHealthLost, maximumHealthLost);
		isAttacked = true;
		playerAttackLanded = true;
	}

	else if (canPlayerLandAttack && !playerAttackLanded && APlayerCharacter::GetAttackNumber() == 2 &&
		APlayerCharacter::GetAttackingTime() >= 0.267f && APlayerCharacter::GetAttackingTime() <= 0.5f)
	{
		UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Boss_Hurt")));

		health -= FMath::FRandRange(minimumHealthLost, maximumHealthLost);
		isAttacked = true;
		playerAttackLanded = true;
	}

	else if (canPlayerLandAttack && !playerAttackLanded && APlayerCharacter::GetAttackNumber() == 3 &&
		APlayerCharacter::GetAttackingTime() >= 0.4f && APlayerCharacter::GetAttackingTime() <= 0.5f)
	{
		UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Boss_Hurt")));

		health -= FMath::FRandRange(minimumHealthLost, maximumHealthLost);
		isAttacked = true;
		playerAttackLanded = true;
	}
}

void ABossCharacter::DamagePlayer()
{
	if (canBossLandAttack && !bossAttackLanded && attackNumber == 0 &&
		attackTime >= 0.5f && attackTime <= 0.8f)
	{
		APlayerCharacter::SetDamaged(FMath::FRandRange(minimumPlayerHealthLost, maximumPlayerHealthLost));
		APlayerCharacter::SetIsAttacked(true);
		bossAttackLanded = true;
	}

	else if (canBossLandAttack && !bossAttackLanded && attackNumber == 1 &&
		attackTime >= 0.3f && attackTime <= 0.67f)
	{
		APlayerCharacter::SetDamaged(FMath::FRandRange(minimumPlayerHealthLost, maximumPlayerHealthLost));
		APlayerCharacter::SetIsAttacked(true);
		bossAttackLanded = true;
	}

	else if (canBossLandAttack && !bossAttackLanded && attackNumber == 2 &&
		attackTime >= 0.08f && attackTime <= 0.14f)
	{
		APlayerCharacter::SetDamaged(FMath::FRandRange(minimumPlayerHealthLost, maximumPlayerHealthLost));
		APlayerCharacter::SetIsAttacked(true);
		bossAttackLanded = true;
	}

	else if (canBossLandAttack && !bossAttackLanded && attackNumber == 3 &&
		attackTime >= 0.33f && attackTime <= 0.62f)
	{
		APlayerCharacter::SetDamaged(FMath::FRandRange(minimumPlayerHealthLost, maximumPlayerHealthLost));
		APlayerCharacter::SetIsAttacked(true);
		bossAttackLanded = true;
	}
}

void ABossCharacter::PerformAttack(float DeltaTime_)
{
	switch (attackNumber)
	{
	case 0:
		if (attackTime <= maxAttackTime)
		{
			attackTime += DeltaTime_;
		}

		else if (attackTime > maxAttackTime)
		{
			attackTime = 0.0f;
			attackCombo += 1;
			bossAttackLanded = false;
		}
		break;

	case 1:
		if (attackTime <= maxAttackTime)
		{
			attackTime += DeltaTime_;
		}

		else if (attackTime > maxAttackTime)
		{
			attackTime = 0.0f;
			attackCombo += 1;
			bossAttackLanded = false;
		}
		break;

	case 2:
		if (attackTime <= maxAttackTime)
		{
			attackTime += DeltaTime_;
		}

		else if (attackTime > maxAttackTime)
		{
			attackTime = 0.0f;
			attackCombo += 1;
			bossAttackLanded = false;
		}
		break;

	case 3:
		if (attackTime <= maxAttackTime)
		{
			attackTime += DeltaTime_;
		}

		else if (attackTime > maxAttackTime)
		{
			attackTime = 0.0f;
			attackCombo += 1;
			bossAttackLanded = false;
		}
		break;

	default:
		return;
		break;
	}
}

void ABossCharacter::PerformAttackCombos()
{
	switch (attackCombo)
	{
	case 0:
		attackNumber = 0;
		break;

	case 1:
		attackNumber = 1;
		break;

	case 2:
		attackNumber = 2;
		break;

	case 3:
		attackNumber = 3;
		break;

	case 4:
		attackCombo = 0;
		break;

	default:
		return;
		break;
	}
}

void ABossCharacter::AnimateAttacked(float DeltaTime_)
{
	if (isAttacked)
	{
		if (damagedTime <= 0.366667f)
		{
			if (attackTime != 0.0f) attackTime = 0.0f;
			damagedTime += DeltaTime_;
		}

		else if (damagedTime > 0.366667f)
		{
			damagedTime = 0.0f;
			isAttacked = false;
		}
	}
}

void ABossCharacter::LookAtPlayer()
{
	if (playerCharacter)
	{
		// Look at player using find look at rotation
		FRotator seePlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), 
			playerCharacter->GetActorLocation());

		// Set the AI to look at the player at all times
		SetActorRotation(seePlayer);
	}
}