// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HJPlayerCharacter.h"
#include "Components/StaminaComponent.h"
#include "Components/HealthComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/CombatComponent.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "UI/UIManager.h"
#include "UI/HUDUIWidget.h"
#include "UI/GameOverWidget.h"
#include "Core/HJGameModeBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/Scene.h"

#include "Components/CapsuleComponent.h"
#include "Combat/Enemies/BaseEnemyProjectile.h"
#include <Kismet/GameplayStatics.h>
#include <ImpactSystem/SurfaceImpactSubsystem.h>
#include "Engine/DamageEvents.h"

#include "Kismet/GameplayStatics.h"



AHJPlayerCharacter::AHJPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mobile related variables should be null and false upon construction
	/*touchInterface = nullptr;

	isTouchInterfaceEnabled = false;
	canDragCamera = false;*/


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bEnableCameraLag = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));
	ActionSystemComponent = CreateDefaultSubobject<UActionSystemComponent>(TEXT("ActionSystem"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat"));

}

UCameraComponent* AHJPlayerCharacter::GetPlayerCameraComponent() const
{
	return FollowCamera ? FollowCamera : nullptr;
}

void AHJPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateUIStamina();
}

bool AHJPlayerCharacter::GetIsHidden() const
{
	return isHidden;
}

void AHJPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	ActionSystemComponent->OnActionStart.AddDynamic(this, &AHJPlayerCharacter::OnStartedAction);
	ActionSystemComponent->OnActionStop.AddDynamic(this, &AHJPlayerCharacter::OnStoppedAction);

	if (InventoryComponent)
	{
		InventoryComponent->OnCandyPickedUp.AddDynamic(this, &AHJPlayerCharacter::OnCandyPickedUp);
		InventoryComponent->OnCandyEaten.AddDynamic(this, &AHJPlayerCharacter::OnCandyEaten);
	}

	UpdateZoom();

}

void AHJPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* ThisController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(ThisController->GetLocalPlayer());
		if (bUseAZERTYLayout)
		{
			Subsystem->AddMappingContext(BaseInputMappingContext_AZERTY, 0);
		}
		else
		{
			Subsystem->AddMappingContext(BaseInputMappingContext, 0);
		}

		// Only for developing the build on mobile devices
		/*if (touchInterface && ThisController)
		{
			ThisController->ActivateTouchInterface(touchInterface);
			canDragCamera = true;
			isTouchInterfaceEnabled = true;
		}*/
	}
}

void AHJPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Only valid when building for mobile
	//if (touchInterface)
	//{
	//	// Camera movement (mobile touch and drag event)
	//	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AHJPlayerCharacter::StartTouchInput);
	//}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputConfig && InputConfig->MoveAction)
		{
			EnhancedInputComponent->BindAction(InputConfig->MoveAction, ETriggerEvent::Triggered, this, &AHJPlayerCharacter::Move);
		}
		if (InputConfig && InputConfig->SpinCameraAction)
		{
			EnhancedInputComponent->BindAction(InputConfig->SpinCameraAction, ETriggerEvent::Triggered, this, &AHJPlayerCharacter::SpinCamera);
		}
		if (InputConfig && InputConfig->ZoomAction)
		{
			EnhancedInputComponent->BindAction(InputConfig->ZoomAction, ETriggerEvent::Triggered, this, &AHJPlayerCharacter::CameraZoom);
		}
		if (InputConfig && InputConfig->JumpAction)
		{
			EnhancedInputComponent->BindAction(InputConfig->JumpAction, ETriggerEvent::Triggered, this, &AHJPlayerCharacter::PerformJump);
		}
		if (InputConfig && InputConfig->EatAction)
		{
			EnhancedInputComponent->BindAction(InputConfig->EatAction, ETriggerEvent::Triggered, this, &AHJPlayerCharacter::EatCandy);
		}
		if (InputConfig && InputConfig->ThrowAttackAction)
		{
			EnhancedInputComponent->BindAction(InputConfig->ThrowAttackAction, ETriggerEvent::Triggered, this, &AHJPlayerCharacter::ThrowAttack);
		}
		if (InputConfig && InputConfig->BlockAction)
		{
			EnhancedInputComponent->BindAction(InputConfig->BlockAction, ETriggerEvent::Started, this, &AHJPlayerCharacter::BlockIncomingAttack);
			EnhancedInputComponent->BindAction(InputConfig->BlockAction, ETriggerEvent::Completed, this, &AHJPlayerCharacter::StopBlocking);
		}
		if (InputConfig && InputConfig->SprintAction)
		{
			EnhancedInputComponent->BindAction(InputConfig->SprintAction, ETriggerEvent::Started, this, &AHJPlayerCharacter::StartSprinting);
		}
		if (InputConfig && InputConfig->SprintAction)
		{
			EnhancedInputComponent->BindAction(InputConfig->SprintAction, ETriggerEvent::Completed, this, &AHJPlayerCharacter::StopSprinting);
		}

	}
}



bool AHJPlayerCharacter::IsBlocking() const
{
	return CombatComponent ? CombatComponent->IsBlocking() : false;
}

bool AHJPlayerCharacter::IsSprinting() const
{
	return StaminaComponent ? StaminaComponent->IsSprinting() : false;
}


bool AHJPlayerCharacter::IsAlive() const
{
	return !HealthComponent->IsDead();
}

bool AHJPlayerCharacter::IsDead() const
{
	return HealthComponent->IsDead();
}

void AHJPlayerCharacter::SetPlayerHUD(UBaseUIWidget* HUDWidget)
{

	PlayerHUD = Cast<UHUDUIWidget>(HUDWidget);
	UpdateUIHealth();
	UpdateUIStamina();
	UpdateUICandies();
}

void AHJPlayerCharacter::StartSprinting()
{
	ActionSystemComponent->StartActionByTag(FGameplayTag::RequestGameplayTag(FName("Action.Type.Sprint")));

}

void AHJPlayerCharacter::StopSprinting()
{
	ActionSystemComponent->StopActionByTag(FGameplayTag::RequestGameplayTag(FName("Action.Type.Sprint")));
}





void AHJPlayerCharacter::OnStartedAction(FGameplayTag ActionTag)
{
	if (ActionTag == FGameplayTag::RequestGameplayTag(FName("Action.Type.Sprint"))) { if (StaminaComponent) StaminaComponent->StartSprinting(); }
}

void AHJPlayerCharacter::OnStoppedAction(FGameplayTag ActionTag)
{
	if (ActionTag == FGameplayTag::RequestGameplayTag(FName("Action.Type.Sprint"))) { if (StaminaComponent) StaminaComponent->StopSprinting(); }
	else if (ActionTag == FGameplayTag::RequestGameplayTag(FName("Action.Type.Death")))
	{
		FGameplayTag DeadTag = FGameplayTag::RequestGameplayTag(FName("Action.Type.Dead"));
		ActionSystemComponent->StartActionByTag(DeadTag);

	}
}

void AHJPlayerCharacter::OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType)
{
	Super::OnHealthChanged(HealthComp, Health, HealthDelta, DamageType);
	if (HealthDelta < 0.0f)
	{
		// Play hurt sound, animation, particle
		ActionSystemComponent->StartActionByTag(FGameplayTag::RequestGameplayTag(FName("Action.Type.HitReact")));
	}
	UpdateUIHealth();
}

void AHJPlayerCharacter::OnDeath(UHealthComponent* HealthComp, AActor* Killer, const class UDamageType* DamageType)
{
	Super::OnDeath(HealthComp, Killer, DamageType);
	ActionSystemComponent->StartActionByTag(FGameplayTag::RequestGameplayTag(FName("Action.Type.Death")));

	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
	if (!GameModeBase) return;

	AHJGameModeBase* PumpkinGameMode = Cast<AHJGameModeBase>(GameModeBase);
	if (!PumpkinGameMode) return;

	PumpkinGameMode->OnPlayerDied();
	if (CombatComponent) CombatComponent->StopBlocking();

}

void AHJPlayerCharacter::OnCandyPickedUp(float NewCount)
{
	UpdateUICandies();
}

void AHJPlayerCharacter::OnCandyEaten(float NewCount)
{
	UpdateUICandies();
}

void AHJPlayerCharacter::UpdateZoom()
{
	ZoomValue = FMath::Clamp(ZoomDirection * 0.01f + ZoomValue, 0.f, 1.f);

	if (!ZoomCurve) return;

	float ZoomAlpha = ZoomCurve->GetFloatValue(ZoomValue);
	float TargetArmLen = FMath::Lerp(ZoomMinDistance, ZoomMaxDistance, ZoomAlpha);
	float NewPitch = FMath::Lerp(ZoomMinAngle, ZoomMaxAngle, ZoomAlpha);
	float Fov = FMath::Lerp(NearFov, FarFov, ZoomAlpha);

	CameraBoom->TargetArmLength = TargetArmLen;

	FRotator OldRotation = CameraBoom->GetComponentRotation();
	CameraBoom->SetWorldRotation(FRotator(NewPitch, OldRotation.Yaw, OldRotation.Roll));

	UpdateDOF();
	FollowCamera->SetFieldOfView(Fov);
}

void AHJPlayerCharacter::UpdateDOF()
{
	FPostProcessSettings& Settings = FollowCamera->PostProcessSettings;

	Settings.bOverride_DepthOfFieldFocalDistance = true;
	Settings.DepthOfFieldFocalDistance = CameraBoom->TargetArmLength;

	Settings.bOverride_DepthOfFieldFstop = true;
	Settings.DepthOfFieldFstop = 3.f;

	Settings.bOverride_DepthOfFieldSensorWidth = true;
	Settings.DepthOfFieldSensorWidth = 36.f;

	if (bEnableDOF)
	{
		OnDOFUpdate.Broadcast(CameraBoom->TargetArmLength);
	}

}

void AHJPlayerCharacter::UpdateUIHealth()
{
	if (PlayerHUD && HealthComponent)
	{
		PlayerHUD->UpdateHealth(HealthComponent->GetHealthPercentage());
	}
}

void AHJPlayerCharacter::UpdateUIStamina()
{
	if (PlayerHUD && StaminaComponent)
	{
		PlayerHUD->UpdateStamina(StaminaComponent->GetStaminaPercent());
	}
}

void AHJPlayerCharacter::UpdateUICandies()
{
	if (PlayerHUD && InventoryComponent)
	{
		PlayerHUD->UpdateCandyCount(InventoryComponent->GetCandyCount());
	}
}

void AHJPlayerCharacter::Move(const FInputActionValue& Value)
{

	FVector2D InputDirection = Value.Get<FVector2D>();
	if (!InputDirection.IsNearlyZero())
	{
		FVector2D NormalizedInput = InputDirection.GetSafeNormal();

		const float InputMagnitude = FMath::Clamp(InputDirection.Size(), 0.0f, 1.0f);
		const FRotator CameraRotation = GetControlRotation();
		FVector ForwardDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);

		ForwardDirection.Z = 0.0f;
		RightDirection.Z = 0.0f;
		ForwardDirection.Normalize();
		RightDirection.Normalize();

		FVector MoveDirection = (ForwardDirection * NormalizedInput.Y + RightDirection * NormalizedInput.X).GetSafeNormal();
		AddMovementInput(MoveDirection, InputMagnitude);
	}
}

void AHJPlayerCharacter::SpinCamera(const FInputActionValue& Value)
{
	const float SpinAmount = Value.Get<float>();
	if (!FMath::IsNearlyZero(SpinAmount))
	{
		FRotator NewRotation = CameraBoom->GetComponentRotation();;
		NewRotation.Yaw += SpinAmount * GetWorld()->GetDeltaSeconds() * SpinSensitivity;
		CameraBoom->SetRelativeRotation(NewRotation);

		if (Controller)
		{
			FRotator ControlRot = Controller->GetControlRotation();
			ControlRot.Yaw = NewRotation.Yaw;
			Controller->SetControlRotation(ControlRot);
		}
	}
}

void AHJPlayerCharacter::CameraZoom(const FInputActionValue& Value)
{
	ZoomDirection = Value.Get<float>();

	UpdateZoom();
	UpdateDOF();
}

void AHJPlayerCharacter::PerformJump()
{
	if (CanJump())
		ActionSystemComponent->StartActionByTag(FGameplayTag::RequestGameplayTag(FName("Action.Type.Jump")));
}


void AHJPlayerCharacter::EatCandy()
{
	if (!HealthComponent) return;
	if (!InventoryComponent) return;
	if (!ActionSystemComponent) return;

	if (HealthComponent->IsAtFullHealth()) return;
	if (!InventoryComponent->HasCandy()) return;

	ActionSystemComponent->StartActionByTag(FGameplayTag::RequestGameplayTag(FName("Action.Type.Eat")));
}

void AHJPlayerCharacter::ThrowAttack()
{
	ActionSystemComponent->StartActionByTag(FGameplayTag::RequestGameplayTag(FName("Action.Type.Throw")));
}

void AHJPlayerCharacter::BlockIncomingAttack()
{
	ActionSystemComponent->StartActionByTag(FGameplayTag::RequestGameplayTag(FName("Action.Type.Block")));
}

void AHJPlayerCharacter::StopBlocking()
{
	ActionSystemComponent->StopActionByTag(FGameplayTag::RequestGameplayTag(FName("Action.Type.Block")));
}

//void AHJPlayerCharacter::StartTouchInput(ETouchIndex::Type touchType, FVector newTouchLocation_)
//{
//	if (touchType == ETouchIndex::Touch1 && isTouchInterfaceEnabled && canDragCamera)
//	{
//		bool isPressed;
//
//		if (APlayerController* ThisController = Cast<APlayerController>(GetController()))
//		{
//			ThisController->GetInputTouchState(ETouchIndex::Touch1, previousTouchLocation.X, previousTouchLocation.Y,
//				isPressed);
//		}
//	}
//}
//
//void AHJPlayerCharacter::MobileControlCamera()
//{
//	if (isTouchInterfaceEnabled)
//	{
//		bool isPressed;
//
//		if (APlayerController* ThisController = Cast<APlayerController>(GetController()))
//		{
//			ThisController->GetInputTouchState(ETouchIndex::Touch1, newTouchLocation.X, newTouchLocation.Y,
//				isPressed);
//
//			// Make sure the camera touch and drag only occurs during the press and drag camera is true
//			if (isPressed && canDragCamera)
//			{
//				FVector2D deltaTouchLocation = newTouchLocation - previousTouchLocation;
//
//				FVector deltaOffset = FVector(deltaTouchLocation.X, 0.0f, deltaTouchLocation.Y);
//
//				const float SpinAmount = deltaOffset.X;
//				if (!FMath::IsNearlyZero(SpinAmount) && pauseMenuUI && !pauseMenuUI->IsVisible())
//				{
//					FRotator NewRotation = CameraBoom->GetComponentRotation();;
//					NewRotation.Yaw += SpinAmount * GetWorld()->GetDeltaSeconds() * SpinSensitivity;
//					CameraBoom->SetRelativeRotation(NewRotation);
//				}
//
//				previousTouchLocation = newTouchLocation;
//			}
//
//			/* Otherwise, if player hasn't pressed on the screen and touch interface is enabled, but the drag camera isn't
//			enabled yet, enable drag camera */
//			else if (!isPressed && canDragCamera != true)
//			{
//				canDragCamera = true;
//			}
//		}
//	}
//}
//
//void AHJPlayerCharacter::ToggleMobileInterfaceVisibility()
//{
//	// Place the task graph for toggling mobile interface visibility inside the game thread for execution
//	FGraphEventRef toggleMobileInterfaceVisibilityTask = FFunctionGraphTask::CreateAndDispatchWhenReady([this]()
//		{
//			if (APlayerController* ThisController = Cast<APlayerController>(GetController()))
//			{
//				if (pauseMenuUI && pauseMenuUI->IsVisible() && isTouchInterfaceEnabled)
//				{
//					ThisController->ActivateTouchInterface(nullptr);
//
//					isTouchInterfaceEnabled = false;
//					canDragCamera = false;
//				}
//
//				else if (pauseMenuUI && !pauseMenuUI->IsVisible() && !isTouchInterfaceEnabled)
//				{
//					ThisController->ActivateTouchInterface(touchInterface);
//
//					isTouchInterfaceEnabled = true;
//				}
//			}
//		}, TStatId(), nullptr, ENamedThreads::GameThread);
//}