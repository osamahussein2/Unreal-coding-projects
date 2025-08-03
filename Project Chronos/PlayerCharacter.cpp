// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PauseMenuActor.h"

// Initialize movement keys
FKey APlayerCharacter::currentMoveBackKey = EKeys::S;
FKey APlayerCharacter::currentMoveForwardKey = EKeys::W;
FKey APlayerCharacter::currentMoveLeftKey = EKeys::A;
FKey APlayerCharacter::currentMoveRightKey = EKeys::D;

// Initialize action keys
FKey APlayerCharacter::currentPauseKey = EKeys::Escape;
FKey APlayerCharacter::currentAttackKey = EKeys::SpaceBar;

// Initialize modify movement keys booleans
bool APlayerCharacter::modifyMoveForwardKey = false;
bool APlayerCharacter::modifyMoveBackKey = false;
bool APlayerCharacter::modifyMoveLeftKey = false;
bool APlayerCharacter::modifyMoveRightKey = false;

// Initialize modify action keys boolean
bool APlayerCharacter::modifyPauseKey = false;
bool APlayerCharacter::modifyAttackKey = false;

// Initialize static booleans
bool APlayerCharacter::gamePaused = false;
bool APlayerCharacter::isInMenu = true;

// Initialize other static variables
unsigned int APlayerCharacter::phaseNumber = 1;

float APlayerCharacter::playerHealth = maxPlayerHealth;

int APlayerCharacter::attackNumber = -1;
float APlayerCharacter::attackTime = 0.0f;

float APlayerCharacter::deathTime = 0.0f;
bool APlayerCharacter::isDead = false;

bool APlayerCharacter::isAttacked = false;
float APlayerCharacter::damagedTime = 0.0f;

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize spring arm component
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	springArmComponent->SetupAttachment(RootComponent);

	// Set its pawn control rotation to true
	if (springArmComponent) springArmComponent->bUsePawnControlRotation = true;

	// Initialize camera component
	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetupAttachment(springArmComponent);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	gamePaused = false;

	modifyMoveForwardKey = false;
	modifyMoveBackKey = false;
	modifyMoveLeftKey = false;
	modifyMoveRightKey = false;
	
	modifyPauseKey = false;

	attackNumber = -1;

	attackTime = 0.0f;

	deathTime = 0.0f;
	isDead = false;

	isAttacked = false;
	damagedTime = 0.0f;

	// Set it to any number greater than 0 to be able to perform an attack for the first time
	timeAfterAttack = 0.2f;

	canPerformAttackCombos = false;
	comboAttackNumber = 0;
	attackComboTime = 0.0f;

	// Only do this if spring arm component is valid and its pawn control rotation isn't true
	if (springArmComponent && springArmComponent->bUsePawnControlRotation != true)
	{
		springArmComponent->bUsePawnControlRotation = true;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Don't do anything if the player is in menu
	if (isInMenu) return;

	// If the player isn't in the menu nor paused the game, detect any input and animate the player as well
	if (!gamePaused && !isInMenu)
	{
		if (!isDead)
		{
			CheckForMovementInput();
			CheckForActionInput();

			// Do attacks
			PerformAttack(DeltaTime);
			
			// Handle attack combos
			PerformComboAttacks();
			HandleAttackComboLogic(DeltaTime);

			AnimateAttacked(DeltaTime);
			
			if (deathTime != 0.0f) deathTime = 0.0f;
			if (!isAttacked && damagedTime != 0.0f) damagedTime = 0.0f;
			if (attackNumber <= -1 && timeAfterAttack <= 0.25f) timeAfterAttack += DeltaTime;
		}

		// Reset attack boolean when the player's health is at 0 and set the dead logic instead
		if (playerHealth <= 0.0f)
		{
			if (attackNumber >= 0) attackNumber = -1;
			if (isDead != true) isDead = true;

			if (isDead && deathTime <= 1.1f) deathTime += DeltaTime;

			if (isAttacked != false)
			{
				damagedTime = 0.0f;
				isAttacked = false;
			}
		}

		// Otherwise the player's health is greater than 0, and we can make sure is dead is false for alive
		else if (playerHealth > 0.0f)
		{
			if (isDead != false) isDead = false;
		}
	}

	// Make sure the pause input logic only occurs when the player isn't in the menu
	if (!isInMenu)
	{
		// Toggle pause menu
		if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(currentPauseKey))
		{
			if (gamePaused == false) gamePaused = true;
			else if (gamePaused == true) gamePaused = false;


			if (APauseMenuActor::pauseStateChanged != false) APauseMenuActor::pauseStateChanged = false;
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Camera movement
	PlayerInputComponent->BindAxis(TEXT("HorizontalCamera"), this, &APlayerCharacter::MoveCameraHorizontally);
	PlayerInputComponent->BindAxis(TEXT("VerticalCamera"), this, &APlayerCharacter::MoveCameraVertically);
}

void APlayerCharacter::CheckForMovementInput()
{
	// Start moving back
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(currentMoveBackKey))
	{
		AddMovementInput(GetActorForwardVector(), -1.0f * playerSpeed);
	}

	// Move forward
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(currentMoveForwardKey))
	{
		AddMovementInput(GetActorForwardVector(), 1.0f * playerSpeed);
	}

	// Move left
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(currentMoveLeftKey))
	{
		AddMovementInput(GetActorRightVector(), -1.0f * playerSpeed);
	}

	// Move right
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(currentMoveRightKey))
	{
		AddMovementInput(GetActorRightVector(), 1.0f * playerSpeed);
	}
}

void APlayerCharacter::CheckForActionInput()
{
	/* Start attacking only if the player isn't attacked while pressing the binded attack key and some time has passed
	after an attack to prevent animation glitches */
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(currentAttackKey) && !isAttacked && 
		timeAfterAttack >= 0.2f)
	{
		// TODO: Attack functionality (DONE)
		if (attackNumber <= -1)
		{
			if (canPerformAttackCombos != true)
			{
				if (comboAttackNumber <= 0) comboAttackNumber = 1;
				canPerformAttackCombos = true;
			}

			// Play player attack sound as well while attack number is set to a number other than -1
			UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(NULL, TEXT("/Game/Sounds/Player-Attack")));
		}
	}
}

void APlayerCharacter::MoveCameraHorizontally(float value_)
{
	if (!isInMenu && !gamePaused) AddControllerYawInput(value_ * cameraSpeed);
}

void APlayerCharacter::MoveCameraVertically(float value_)
{
	if (!isInMenu && !gamePaused) AddControllerPitchInput(-value_ * cameraSpeed);
}

void APlayerCharacter::PerformAttack(float DeltaTime_)
{
	// Update player attack animation logic
	switch (attackNumber)
	{
	case 0:
		if (attackTime <= 0.8f)
		{
			attackTime += DeltaTime_;
		}

		// Finish attacking
		else if (attackTime > 0.8f)
		{
			attackTime = 0.0f;
			timeAfterAttack = 0.0f;
			
			if (canPerformAttackCombos)
			{
				attackComboTime = 0.0f;
				comboAttackNumber += 1;

				canPerformAttackCombos = false;

				attackNumber = -1;
			}
		}
		break;

	case 1:
		if (attackTime <= 0.8f)
		{
			attackTime += DeltaTime_;
		}

		// Finish attacking
		else if (attackTime > 0.8f)
		{
			attackTime = 0.0f;
			timeAfterAttack = 0.0f;

			if (canPerformAttackCombos)
			{
				attackComboTime = 0.0f;
				comboAttackNumber += 1;

				canPerformAttackCombos = false;

				attackNumber = -1;
			}
		}
		break;

	case 2:
		if (attackTime <= 0.8f)
		{
			attackTime += DeltaTime_;
		}

		// Finish attacking
		else if (attackTime > 0.8f)
		{
			attackTime = 0.0f;
			timeAfterAttack = 0.0f;

			if (canPerformAttackCombos)
			{
				attackComboTime = 0.0f;
				comboAttackNumber += 1;

				canPerformAttackCombos = false;

				attackNumber = -1;
			}
		}
		break;

	case 3:
		if (attackTime <= 0.8f)
		{
			attackTime += DeltaTime_;
		}

		// Finish attacking
		else if (attackTime > 0.8f)
		{
			attackTime = 0.0f;
			timeAfterAttack = 0.0f;

			if (canPerformAttackCombos)
			{
				attackComboTime = 0.0f;
				comboAttackNumber += 1;

				canPerformAttackCombos = false;

				attackNumber = -1;
			}
		}
		break;

	default:
		return;
		break;
	}
}

void APlayerCharacter::PerformComboAttacks()
{
	// Update player attack combo logic
	if (canPerformAttackCombos)
	{
		switch (comboAttackNumber)
		{
		case 1:
			attackNumber = 0;
			break;

		case 2:
			attackNumber = 1;
			break;

		case 3:
			attackNumber = 2;
			break;

		case 4:
			attackNumber = 3;
			break;

		case 5:
			comboAttackNumber = 1;
			break;

		default:
			return;
			break;
		}
	}
}

void APlayerCharacter::HandleAttackComboLogic(float DeltaTime_)
{
	if (!canPerformAttackCombos)
	{
		/* Make sure the time is less than some threshold value and player isn't attacking while their combo attack 
		number isn't 0 */
		if (attackComboTime <= 0.6f && attackNumber <= -1 && comboAttackNumber > 0)
		{
			// Increment the attack combo time
			attackComboTime += DeltaTime_;
		}

		// Else we exceeded the attack combo time and must reset the combo attack number to 0 to reset to first attack
		else if (attackComboTime > 0.6f && attackNumber <= -1 && comboAttackNumber != 0)
		{
			comboAttackNumber = 0;
		}
	}
}

void APlayerCharacter::AnimateAttacked(float DeltaTime_)
{
	// Update attacked animation logic
	if (isAttacked)
	{
		if (damagedTime <= 0.366667f)
		{
			damagedTime += DeltaTime_;
		}

		// Finish being attacked
		else if (damagedTime > 0.366667f)
		{
			damagedTime = 0.0f;
			timeAfterAttack = 0.0f;
			isAttacked = false;
		}
	}
}