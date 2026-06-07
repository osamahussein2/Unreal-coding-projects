// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputAction.h"
#include "InputMappingContext.h"

#include "Camera/CameraComponent.h"
#include "CableComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameManager/WidgetManager.h"
#include "Widget/PauseMenuWidget.h"

#include "Actor/Grapplable.h"
#include "Actor/Swingable.h"
#include "InteractInterface.h"
#include "ConnectionInterface.h"
#include "PowerCable.h"
#include "EngineUtils.h"

#include "DefaultPlayerController.h"

// Sets default values
ADefaultPlayer::ADefaultPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;

	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);

	GetCapsuleComponent()->SetEnableGravity(true);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetCapsuleComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Set its pawn control rotation to true
	springArm->bUsePawnControlRotation = true;

	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComponent->SetupAttachment(springArm);

	cableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
	cableComponent->SetupAttachment(GetMesh());

	hookMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HookMesh"));
	hookMesh->SetupAttachment(cableComponent);

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComponent->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ADefaultPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (cableComponent)
	{
		relativeCableComponentLocation = cableComponent->GetRelativeLocation();
		initialCableEndPoint = cableComponent->EndLocation;
	}

	if (hookMesh)
	{
		hookMeshRelativeLocation = hookMesh->GetRelativeLocation();
		hookMeshRelativeRotation = hookMesh->GetRelativeRotation();
	}
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ADefaultPlayer::Move(const FInputActionValue& Value)
{
	if (locomotionState == ELocomotionState::AboutToGrapple || locomotionState == ELocomotionState::Grappling ||
		locomotionState == ELocomotionState::AboutToSwing || locomotionState == ELocomotionState::SwingingToPoint ||
		locomotionState == ELocomotionState::SwingingInbounds) return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		FRotator Rotation = Controller->GetControlRotation();

		FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		// Play footstep sounds if the current player movement mode is walking
		if (GetCharacterMovement() && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking && 
			!footstepSounds.IsEmpty() && audioComponent && !audioComponent->IsPlaying())
		{
			const int& randomFootstepSoundIndex = FMath::RandRange(0, footstepSounds.Num() - 1);

			if (footstepSounds[randomFootstepSoundIndex])
			{
				audioComponent->SetSound(footstepSounds[randomFootstepSoundIndex]);
				audioComponent->Play();
			}
		}

		if (locomotionState == ELocomotionState::SwingingFreely)
		{
			swingingX = MovementVector.X;
			swingingY = MovementVector.Y;

			if (audioComponent && !audioComponent->IsPlaying() && swingSound)
			{
				// Play swing sound
				audioComponent->SetSound(swingSound);
				audioComponent->Play();
			}
		}
	}
}

void ADefaultPlayer::SwingAround()
{
	if (locomotionState == ELocomotionState::SwingingFreely)
	{
		if (swingingX > minSwingValue)
		{
			swingingX -= GetWorld()->GetDeltaSeconds();

			FRotator Rotation = Controller->GetControlRotation();
			FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

			FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(RightDirection, swingingX);
		}

		else if (swingingX < -minSwingValue)
		{
			swingingX += GetWorld()->GetDeltaSeconds();

			FRotator Rotation = Controller->GetControlRotation();
			FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

			FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(RightDirection, swingingX);
		}

		if (swingingY > minSwingValue)
		{
			swingingY -= GetWorld()->GetDeltaSeconds();

			FRotator Rotation = Controller->GetControlRotation();
			FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

			FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			AddMovementInput(ForwardDirection, swingingY);
		}

		else if (swingingY < -minSwingValue)
		{
			swingingY += GetWorld()->GetDeltaSeconds();

			FRotator Rotation = Controller->GetControlRotation();
			FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

			FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			AddMovementInput(ForwardDirection, swingingY);
		}

		const float& distanceToSwingPointX = FMath::Abs(swingPoint.X - GetActorLocation().X);
		const float& distanceToSwingPointY = FMath::Abs(swingPoint.Y - GetActorLocation().Y);

		if (distanceToSwingPointX <= distanceToStopSwinging && swingingX >= -minSwingValue && swingingX <= minSwingValue)
		{
			swingingX = 0.0f;

			FRotator Rotation = Controller->GetControlRotation();
			FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

			FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(RightDirection, swingingX);
		}

		if (distanceToSwingPointY <= distanceToStopSwinging && swingingY >= -minSwingValue && swingingY <= minSwingValue)
		{
			swingingY = 0.0f;

			FRotator Rotation = Controller->GetControlRotation();
			FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

			FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			AddMovementInput(ForwardDirection, swingingY);
		}

		if (swingingX != 0.0f)
		{
			FRotator Rotation = Controller->GetControlRotation();
			FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

			FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(RightDirection, swingingX);
		}

		if (swingingY != 0.0f)
		{
			FRotator Rotation = Controller->GetControlRotation();
			FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

			FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			AddMovementInput(ForwardDirection, swingingY);
		}
	}
}

void ADefaultPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(-LookAxis.Y);
}

void ADefaultPlayer::Interact(const FInputActionValue& Value)
{
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (CurrentEquipment)
	{
		QueryParams.AddIgnoredActor(CurrentEquipment);
	}

	FVector TraceStart = GetPlayerCameraLocation();
	FVector TraceEnd = TraceStart + (GetPlayerCameraForwardVector() * InteractionDistance);


	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.0f, 0, 2.0f);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	APowerCable* BestCable = nullptr;
	float BestDistance = TNumericLimits<float>::Max();

	for (TActorIterator<APowerCable> It(GetWorld()); It; ++It)
	{
		APowerCable* Cable = *It;

		if (!Cable) continue;

		float Distance;

		if (It->IsHitByTrace(TraceStart, TraceEnd, 20.0f, Distance))
		{
			if (Distance < BestDistance)
			{
				BestDistance = Distance;
				BestCable = Cable;
			}
		}
	}

	if (BestCable)
	{
		if (IInteractInterface::Execute_CanInteract(BestCable, this))
		{
			IInteractInterface::Execute_Interact(BestCable, this);
		}

		return;
	}

	if (bHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 15.0f, 12, FColor::Red, false, 2.0f);

		AActor* HitActor = HitResult.GetActor();
		if (!HitActor) return;

		UE_LOG(LogTemp, Warning, TEXT("HitActor: %s"), *HitActor->GetName());
		if (bIsHoldingCable)
		{
			if (HitActor && HitActor->Implements<UConnectionInterface>())
			{
				APowerCable* Cable = Cast<APowerCable>(CurrentEquipment);

				if (Cable)
				{
					Cable->TryConnect(HitActor);
				}

				return;
			}
		}

		if (HitActor && HitActor->Implements<UInteractInterface>())
		{
			if (IInteractInterface::Execute_CanInteract(HitActor, this))
			{
				IInteractInterface::Execute_Interact(HitActor, this);
			}
		}
	}
}

void ADefaultPlayer::Sprint(const FInputActionValue& Value)
{
	bIsSprinting = Value.Get<bool>();

	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ADefaultPlayer::Pause()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!playerController) return;

		AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
		if (!widgetManager) return;

		widgetManager->EnableMouseCursor();
		widgetManager->PushWidget(widgetManager->GetPauseMenuClass());
	}

	else
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!playerController) return;

		AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
		if (!widgetManager) return;

		widgetManager->ResumeGameplay();
	}
}

void ADefaultPlayer::Fire(const FInputActionValue& Value)
{
	if (locomotionState == ELocomotionState::Normal)
	{
		// If grapplable or swing on actor is valid
		if (grapplableOrSwingActor)
		{
			// Grapple to location if a grapplable actor is valid
			if (AGrapplable* grappleableObject = Cast<AGrapplable>(grapplableOrSwingActor))
			{
				if (GetCharacterMovement())
				{
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
				}

				grappleableObject->DestroyWidgetComponent();

				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Climbable actor is found"));

				grapplePoint = grappleableObject->GetGrapplePoint();

				if (GetCharacterMovement()) GetCharacterMovement()->StopMovementImmediately();

				SetActorRotation(FRotator(GetActorRotation().Pitch, GetControlRotation().Yaw, GetControlRotation().Roll));

				// Play grapple sound
				if (audioComponent && grappleSound)
				{
					audioComponent->SetSound(grappleSound);
					audioComponent->Play();
				}

				locomotionState = ELocomotionState::AboutToGrapple;
			}

			// Swing to location if a test swinging actor is valid
			else if (ASwingable* swingableObject = Cast<ASwingable>(grapplableOrSwingActor))
			{
				if (GetCharacterMovement())
				{
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
				}

				swingPoint = swingableObject->GetActorLocation();
				if (swingToPointTime != 1.0f) swingToPointTime = 1.0f;

				swingableObject->DestroyWidgetComponent();

				if (GetCharacterMovement()) GetCharacterMovement()->StopMovementImmediately();

				SetActorRotation(FRotator(GetActorRotation().Pitch, GetControlRotation().Yaw, GetControlRotation().Roll));

				// Play grapple sound
				if (audioComponent && grappleSound)
				{
					audioComponent->SetSound(grappleSound);
					audioComponent->Play();
				}

				locomotionState = ELocomotionState::AboutToSwing;
			}
		}
	}
}

void ADefaultPlayer::Jump()
{
	// If player is swinging freely, allow the player to jump
	if (locomotionState == ELocomotionState::SwingingFreely)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}

		const FVector& forwardLaunch = GetActorForwardVector() * forwardLaunchValue;
		const FVector& upLaunch = GetActorUpVector() * upLaunchValue;

		const FVector& launchVelocity = forwardLaunch + upLaunch;

		LaunchCharacter(launchVelocity, false, false);

		// Attach the cable component back onto the player mesh and reset to starting values
		if (cableComponent)
		{
			cableComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
			cableComponent->SetRelativeLocation(relativeCableComponentLocation);
			cableComponent->SetRelativeRotation(FRotator::ZeroRotator);
			cableComponent->EndLocation = initialCableEndPoint;
		}

		if (hookMesh)
		{
			hookMesh->SetVisibleInSceneCaptureOnly(false);
			hookMesh->SetRelativeLocation(hookMeshRelativeLocation);
			hookMesh->SetRelativeRotation(hookMeshRelativeRotation);
		}

		swingPoint = FVector::ZeroVector;

		// Play jump sound
		if (audioComponent && jumpSound)
		{
			audioComponent->SetSound(jumpSound);
			audioComponent->Play();
		}

		locomotionState = ELocomotionState::Normal;
	}

	// If player is swinging inbounds, allow the player to jump
	else if (locomotionState == ELocomotionState::SwingingInbounds)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}

		const FVector& forwardLaunch = GetActorForwardVector() * forwardLaunchValue;
		const FVector& upLaunch = GetActorUpVector() * upLaunchValue;

		const FVector& launchVelocity = forwardLaunch + upLaunch;

		LaunchCharacter(launchVelocity, false, false);

		// Attach the cable component back onto the player mesh and reset to starting values
		if (cableComponent)
		{
			cableComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
			cableComponent->SetRelativeLocation(relativeCableComponentLocation);
			cableComponent->SetRelativeRotation(FRotator::ZeroRotator);
			cableComponent->EndLocation = initialCableEndPoint;
		}

		if (hookMesh)
		{
			hookMesh->SetVisibleInSceneCaptureOnly(false);
			hookMesh->SetRelativeLocation(hookMeshRelativeLocation);
			hookMesh->SetRelativeRotation(hookMeshRelativeRotation);
		}

		swingPoint = FVector::ZeroVector;

		// Play jump sound
		if (audioComponent && jumpSound)
		{
			audioComponent->SetSound(jumpSound);
			audioComponent->Play();
		}

		locomotionState = ELocomotionState::Normal;
	}

	// If player is about to swing, don't swing to the swing point anymore
	else if (locomotionState == ELocomotionState::AboutToSwing)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		}

		if (cableComponent) cableComponent->EndLocation = initialCableEndPoint;

		if (hookMesh)
		{
			hookMesh->SetRelativeLocation(hookMeshRelativeLocation);
			hookMesh->SetRelativeRotation(hookMeshRelativeRotation);
		}

		swingPoint = FVector::ZeroVector;

		locomotionState = ELocomotionState::Normal;
	}
}

// Called every frame
void ADefaultPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (cableComponent && hookMesh)
	{
		switch (locomotionState)
		{
		case ELocomotionState::AboutToGrapple:
		{
			grappleToPointTime += grappleToPointSpeed * DeltaTime;

			FVector directionToGrapplePoint = cableComponent->GetComponentLocation();
			directionToGrapplePoint += (grapplePoint - cableComponent->GetComponentLocation()).GetSafeNormal() * grappleToPointTime;

			cableComponent->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(),
				directionToGrapplePoint);

			const FRotator& hookMeshLookAt = UKismetMathLibrary::FindLookAtRotation(hookMesh->GetComponentLocation(),
				directionToGrapplePoint);

			hookMesh->SetWorldLocationAndRotation(directionToGrapplePoint, hookMeshLookAt + hookMeshRotation);

			// If the cable component's end location is near the grapple point, start grappling
			if (FVector::Distance(cableComponent->EndLocation, UKismetMathLibrary::InverseTransformLocation(
				GetActorTransform(), grapplePoint)) <= distanceToGrappleToPoint)
			{
				locomotionState = ELocomotionState::Grappling;

				// Remove cable component from the player to set its location onto the player when grappling
				cableComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
				cableComponent->SetWorldLocation(GetActorLocation());

				grappleToPointTime = 1.0f;
			}
			break;
		}

		case ELocomotionState::Grappling:
		{
			FVector directionToGrapple = GetActorLocation();
			directionToGrapple += (grapplePoint - GetActorLocation()).GetSafeNormal() * grappleSpeed;

			SetActorLocation(directionToGrapple);

			const FRotator& hookMeshLookAt = UKismetMathLibrary::FindLookAtRotation(hookMesh->GetComponentLocation(),
				grapplePoint);

			if (hookMesh->GetComponentLocation() != grapplePoint) hookMesh->SetWorldLocation(grapplePoint);
			hookMesh->SetWorldRotation(hookMeshLookAt + hookMeshRotation);

			if (cableComponent)
			{
				cableComponent->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), grapplePoint);
				cableComponent->SetWorldLocation(GetActorLocation());
			}

			if (FVector::Distance(grapplePoint, GetActorLocation()) <= distanceToStopGrappling) StopGrappling();
			break;
		}

		case ELocomotionState::Climbing:
		{
			FVector climbLocation = GetActorLocation();
			climbLocation += (grapplePoint - GetActorLocation()).GetSafeNormal() * climbSpeed;

			SetActorLocation(climbLocation);

			if (!GetWorld()->GetTimerManager().IsTimerActive(stopClimbingHandle))
			{
				GetWorld()->GetTimerManager().SetTimer(stopClimbingHandle, this, &ADefaultPlayer::StopClimbing,
					timeToStopClimbing, false);
			}
			break;
		}

		case ELocomotionState::AboutToSwing:
		{
			swingToPointTime += swingToPointSpeed * DeltaTime;

			FVector swingingActorLocation = cableComponent->GetComponentLocation();
			swingingActorLocation += (swingPoint - cableComponent->GetComponentLocation()).GetSafeNormal() * swingToPointTime;

			cableComponent->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(),
				swingingActorLocation);

			const FRotator& hookMeshLookAt = UKismetMathLibrary::FindLookAtRotation(hookMesh->GetComponentLocation(),
				swingingActorLocation);

			hookMesh->SetWorldLocationAndRotation(swingingActorLocation, hookMeshLookAt + hookMeshRotation);

			// If the cable component's end location is near the swing point, start swinging
			if (FVector::Distance(cableComponent->EndLocation, UKismetMathLibrary::InverseTransformLocation(
				GetActorTransform(), swingPoint)) <= distanceToMoveToPoint)
			{
				locomotionState = ELocomotionState::SwingingToPoint;

				// Remove cable component from the player to set its location onto the player when swinging
				cableComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
				cableComponent->SetWorldLocation(GetActorLocation());

				swingToPointTime = 1.0f;
			}
			break;
		}

		case ELocomotionState::SwingingToPoint:
		{
			cableComponent->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), swingPoint);

			const FVector& swingPointOffset = FVector(swingPoint.X, swingPoint.Y, swingPoint.Z + valueBelowSwingingActor);

			FVector directionToSwingingPoint = GetActorLocation();
			directionToSwingingPoint += (swingPointOffset - GetActorLocation()).GetSafeNormal() * moveToPointSpeed;

			SetActorLocation(directionToSwingingPoint);

			if (hookMesh->GetComponentLocation() != swingPoint) hookMesh->SetWorldLocation(swingPoint);

			const float& distanceToSwingPointX = FMath::Abs(swingPoint.X - GetActorLocation().X);
			const float& distanceToSwingPointY = FMath::Abs(swingPoint.Y - GetActorLocation().Y);

			if (distanceToSwingPointX <= distanceToStartSwinging && distanceToSwingPointY <= distanceToStartSwinging)
			{
				locomotionState = ELocomotionState::SwingingFreely;
				SetActorLocation(swingPointOffset);

				if (swingingX != 0.0f) swingingX = 0.0f;
				if (swingingY != 0.0f) swingingY = 0.0f;

				hookMesh->SetVisibleInSceneCaptureOnly(true);
			}

			// Set the cable component to follow the player around for swing effect
			cableComponent->SetWorldLocation(GetActorLocation());
			break;
		}

		case ELocomotionState::SwingingFreely:
		{
			cableComponent->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), swingPoint);

			const float& distanceToSwingPointX = FMath::Abs(swingPoint.X - GetActorLocation().X);
			const float& distanceToSwingPointY = FMath::Abs(swingPoint.Y - GetActorLocation().Y);

			// Move the player swing using parabola motion (Z = v0 * t - 0.5 * g * t^2), where v0 is the initial position
			const float& playerSwing = (((swingPoint.Z + valueBelowSwingingActor) + 
				(distanceToSwingPointX + distanceToSwingPointY)) * swingingMultiplier) -
				(0.5f * GetWorld()->GetGravityZ() * FMath::Square(swingingMultiplier));

			// Make sure the swinging motion is being applied by going up and down when moving the swing
			if (GetActorLocation().Z != playerSwing)
			{
				SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, playerSwing));
			}

			// Swing inbounds once the player exceeds the max swing distance along the X or Y axis
			if (distanceToSwingPointX > maxSwingDistance || distanceToSwingPointY > maxSwingDistance)
			{
				locomotionState = ELocomotionState::SwingingInbounds;

				// Reverse swinging motion
				swingingX = -swingingX;
				swingingY = -swingingY;
			}

			// Set the cable component to follow the player around for swing effect
			cableComponent->SetWorldLocation(GetActorLocation());
			break;
		}

		case ELocomotionState::SwingingInbounds:
		{
			cableComponent->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), swingPoint);

			const float& distanceToSwingPointX = FMath::Abs(swingPoint.X - GetActorLocation().X);
			const float& distanceToSwingPointY = FMath::Abs(swingPoint.Y - GetActorLocation().Y);

			// Move the player swing using parabola motion (Z = v0 * t - 0.5 * g * t^2), where v0 is the initial position
			const float& playerSwing = (((swingPoint.Z + valueBelowSwingingActor) +
				(distanceToSwingPointX + distanceToSwingPointY)) * swingingMultiplier) -
				(0.5f * GetWorld()->GetGravityZ() * FMath::Square(swingingMultiplier));

			const FVector& swingPointOffset = FVector(GetActorLocation().X, GetActorLocation().Y, playerSwing);
			
			FVector directionToSwingPoint = swingPointOffset;
			directionToSwingPoint += (swingPoint - swingPointOffset).GetSafeNormal() * swingInboundsSpeed;

			SetActorLocation(directionToSwingPoint);

			// Look at swing point
			if (distanceToSwingPointX < maxSwingDistance - distanceThresholdToSwingFreelyAgain &&
				distanceToSwingPointY < maxSwingDistance - distanceThresholdToSwingFreelyAgain)
			{
				locomotionState = ELocomotionState::SwingingFreely;
			}

			// Set the cable component to follow the player around for swing effect
			cableComponent->SetWorldLocation(GetActorLocation());
			break;
		}

		default:
			break;
		}
	}
}

// Called to bind functionality to input
void ADefaultPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADefaultPlayer::StopGrappling()
{
	grapplePoint = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 10000.0f);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Grappling stopped"));

	// Attach the cable component back onto the player mesh and reset to starting values
	if (cableComponent)
	{
		cableComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
		cableComponent->SetRelativeLocation(relativeCableComponentLocation);
		cableComponent->SetRelativeRotation(FRotator::ZeroRotator);
		cableComponent->EndLocation = initialCableEndPoint;
	}

	if (hookMesh)
	{
		hookMesh->SetRelativeLocation(hookMeshRelativeLocation);
		hookMesh->SetRelativeRotation(hookMeshRelativeRotation);
	}

	locomotionState = ELocomotionState::Climbing;
}

void ADefaultPlayer::StopClimbing()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);

		grapplePoint = FVector::ZeroVector;

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Climbing stopped"));

		locomotionState = ELocomotionState::Normal;
	}
}

// Controller getter definitions
FVector ADefaultPlayer::GetPlayerCameraLocation() const
{
	if (!GetController() || !Cast<APlayerController>(GetController())) return FVector();

	return Cast<APlayerController>(GetController())->PlayerCameraManager->GetCameraLocation();
}

FVector ADefaultPlayer::GetPlayerCameraForwardVector() const
{
	if (!GetController() || !Cast<APlayerController>(GetController())) return FVector();

	return Cast<APlayerController>(GetController())->PlayerCameraManager->GetCameraRotation().Vector();
}

TObjectPtr<APlayerCameraManager> ADefaultPlayer::GetPlayerCameraManager() const
{
	if (!GetController() || !Cast<APlayerController>(GetController())) return nullptr;

	return Cast<APlayerController>(GetController())->PlayerCameraManager;
}