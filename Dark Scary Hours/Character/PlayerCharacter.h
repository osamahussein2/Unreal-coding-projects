// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UPlayerHUDWidget;
class UPaperSpriteComponent;
class UDialogueWidget;

UENUM()
enum class ELocomotionState : uint8
{
	EIdle UMETA(DisplayName = "Idle"),
	EWalking UMETA(DisplayName = "Walking"),
	ERunning UMETA(DisplayName = "Running")
};

UCLASS()
class EHGAMEJAM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void InitializePlayerHUD();
	void DestroyHUD();

	void ApplyArmor();

	void OnCompleteQuest();

	void PlayerIsInAmmoCrateTrigger();
	void PlayerIsOutOfAmmoCrateTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveCharacter(const struct FInputActionValue& value_);
	void IdleCharacter();

	void MoveCamera(const FInputActionValue& value_);

	void TriggerDialogue();
	void PauseGame();

	// Weapon related input functions
	void PickUpWeapon();

	void GrabAmmo();

	void StartFiring();

	void SwitchToPreviousWeapon();
	void SwitchToNextWeapon();

	UFUNCTION()
	void FireWeapon();
	void CancelWeaponFire();

	void ReloadWeapon();

	void TriggerLockOn();
	void SwitchToLeftTarget();
	void SwitchToRightTarget();

	void StopLockOn();

	void ProceedToNextDialogue();

private:
	//Weapon tag related functions
	void RemoveActionTag(FString tagName_);
	void AddActionTag(FString tagName_);

	bool HasActionTag(FString tagName_);

	/*void UpdateHealthLogic(float& DeltaTime);

	void UpdateRadialBar();
	void UpdateRadialBar(float& DeltaTime);*/

	void TriggerWeaponFireTimerHandle(FName tagName_);

	void TriggerLockOnUsingTag(FName tagName_);
	void SwitchToLeftTargetUsingTag(FName tagName_);
	void SwitchToRightTargetUsingTag(FName tagName_);

	void RotateWeaponUsingTag(FName tagName_);
	void FireWeaponUsingTag(FName tagName_);
	void ReloadWeaponUsingTag(FName tagName_);
	void CheckForEnemies();

	void ShowWeapon(FName tagName_);
	void HideWeapon(FName tagName_);

	void UpdateWeaponHUD();
	void UpdateWeaponHUD(FName tagName_, FString textureFile_);

	void GoToNextWeaponBasedOnIndex();
	void GoToPreviousWeaponBasedOnIndex();

	void NPCNotGivenDialogue();
	void NPCGivenDialogue();
	void NPCCompletedDialogue();

private:
	// Player camera
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> springArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> cameraComponent;

	// Minimap for the player icon
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> minimapSpringArm;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneCaptureComponent2D> sceneCaptureComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperSpriteComponent> spriteComponent;

	// Input objects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* inputMapping;

	// Input Actions
	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_Camera;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_PauseGame;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_NextDialogue;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_FireWeapon;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_ReloadWeapon;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_PreviousWeapon;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_NextWeapon;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_Aiming;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_TargetLeft;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_TargetRight;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_PickUpWeapon;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_TriggerDialogue;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input", BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UInputAction* IA_RefillAmmo;

	// Parameters
	UPROPERTY(EditAnywhere, Category = "Speed")
	float controlRotationSpeed = 10.0f;

	// Player animations
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* idleAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* walkingAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimSequence* runningAnimation = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Locomotion")
	ELocomotionState locomotionState = ELocomotionState::EIdle;

	UPROPERTY(EditAnywhere, Category = "Player Widgets")
	TSubclassOf<UPlayerHUDWidget> playerHUDClass;

	UPROPERTY()
	UPlayerHUDWidget* playerHUD = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float currentHealth = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float maxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	float currentArmor = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float maxArmor = 100.0f;

	// Weapon component
	UPROPERTY(EditAnywhere, Category = "Components")
	class UWeaponComponent* weaponComponent = nullptr;

	// Timer handle for player firing weapon
	FTimerHandle timerHandle;

	bool canFireImmediately = true;

	const FName pistol = FName("Pistol");
	const FName shotgun = FName("Shotgun");
	const FName rifle = FName("Rifle");

	int32 weaponIndex = 0;

	UPROPERTY(VisibleAnywhere, Category = "Camera Settings")
	FVector normalCameraLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Camera Settings")
	FVector lockedOnCameraLocation = FVector::ZeroVector;

	// Player actions
	UPROPERTY(VisibleAnywhere, Category = "Tags")
	TArray<FString> PlayerActions;

	// NPC
	UPROPERTY(EditAnywhere)
	class ANPC* npc;

	// Dialogue widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueWidget> dialogueClass;

	UPROPERTY()
	UDialogueWidget* dialogueUI;
};
