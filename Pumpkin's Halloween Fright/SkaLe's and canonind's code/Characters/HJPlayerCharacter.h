// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HJCharacterBase.h"
#include "Characters/InputConfigDataAsset.h"
#include "GameplayTagContainer.h"
#include "HJPlayerCharacter.generated.h"

class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UStaminaComponent;
class UActionSystemComponent;
class UCurveFloat;
class UHUDUIWidget;
class UBaseUIWidget;
class UInventoryComponent;
class UCombatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDOFUpdateSignature, float, TargetDistance);

/**
 *
 */
UCLASS()
class GDTV_HALLOWEEN2025_API AHJPlayerCharacter : public AHJCharacterBase
{
	GENERATED_BODY()

public:
	AHJPlayerCharacter();

	UCameraComponent* GetPlayerCameraComponent() const;
	bool GetIsHidden() const;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsBlocking() const;
	bool IsSprinting() const;
	bool IsAlive() const;
	bool IsDead() const;

	void SetPlayerHUD(UBaseUIWidget* HUDWidget);

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	/* Input Callbacks */
	void Move(const FInputActionValue& Value);
	void SpinCamera(const FInputActionValue& Value);
	void PerformJump();
	void EatCandy();
	void ThrowAttack();
	void BlockIncomingAttack();
	void StopBlocking();
	void StartSprinting();
	void StopSprinting();
	void CameraZoom(const FInputActionValue& Value);

	/* Actions */
	UFUNCTION()
	void OnStartedAction(FGameplayTag ActionTag);
	UFUNCTION()
	void OnStoppedAction(FGameplayTag ActionTag);

	/* Get Hit */
	virtual void OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType) override;
	virtual void OnDeath(UHealthComponent* HealthComp, AActor* Killer, const class UDamageType* DamageType) override;

	UFUNCTION()
	void OnCandyPickedUp(float NewCount);
	UFUNCTION()
	void OnCandyEaten(float NewCount);

	void UpdateZoom();
	void UpdateDOF();
	void UpdateUIHealth();
	void UpdateUIStamina();
	void UpdateUICandies();


	//void StartTouchInput(ETouchIndex::Type touchType, FVector newTouchLocation_);
	
	// Mobile related functions
	//void MobileControlCamera();
	//void ToggleMobileInterfaceVisibility();

public:
	FOnDOFUpdateSignature OnDOFUpdate;

private:


	// Mobile related variables
	//FVector2D previousTouchLocation;
	//FVector2D newTouchLocation;

	//UPROPERTY(EditAnywhere, Category = "Touch Interface", meta = (AllowPrivateAccess = "true"))
	//UTouchInterface* touchInterface;

	//// Mobile booleans
	//bool isTouchInterfaceEnabled;
	//bool canDragCamera;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* BaseInputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* BaseInputMappingContext_AZERTY;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	bool bUseAZERTYLayout = false;


	UPROPERTY(EditAnywhere, Category = "Input")
	UInputConfigDataAsset* InputConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UActionSystemComponent* ActionSystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCombatComponent* CombatComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Components", meta=(AllowPrivateAccess=true))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Input")
	float SpinSensitivity = 200.f;
	float ZoomDirection = 0.0f;
	float ZoomValue = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
	UCurveFloat* ZoomCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomMinDistance = 400.f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomMaxDistance = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomMinAngle = -35.f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float ZoomMaxAngle = -45.f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float NearFov = 60.f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	float FarFov = 40.f;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	bool bEnableDOF = true;


	UPROPERTY(EditAnywhere, Category = "HideStatus")
	bool isHidden = false;

	UPROPERTY()
	TObjectPtr<UHUDUIWidget> PlayerHUD;
};
