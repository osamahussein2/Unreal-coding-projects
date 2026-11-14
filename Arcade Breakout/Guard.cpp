// Fill out your copyright notice in the Description page of Project Settings.

#include "Guard.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraActor.h"
#include "Manager/WidgetManager.h"
#include "Widget/PlayerHUDWidget.h"
#include "Camera/CameraComponent.h"
#include "GameMode/ShroomJamGM.h"
#include "Widget/GameOverWidget.h"
#include "ShroomJam2026/ShroomJam2026.h"
#include "ArcadeMachine.h"
#include "PrizeMachine.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AGuard::AGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();

	if (GetCapsuleComponent()) GetCapsuleComponent()->OnComponentHit.AddUniqueDynamic(this, &AGuard::OnGuardHit);
	
	if (GetController()) aiController = Cast<AAIController>(GetController());

	if (!aiController) SetActorTickEnabled(false);

	for (TActorIterator<AActor> actorsIt(GetWorld()); actorsIt; ++actorsIt)
	{
		// Skip iterations where the actor isn't an arcade machine, prize machine, guard or doesn't have floor/wall tags
		if (!Cast<AArcadeMachine>(*actorsIt) && !Cast<APrizeMachine>(*actorsIt) && !Cast<AGuard>(*actorsIt) &&
			!actorsIt->ActorHasTag("Floor") && !actorsIt->ActorHasTag("Wall"))
		{
			continue;
		}

		// Add to ignored actors list for patrol trace
		ignoredActors.Add(*actorsIt);
	}
}

void AGuard::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (cameraActor)
	{
		cameraActor->Destroy();
		cameraActor = nullptr;
	}
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!aiController || !playerCharacter || !playerCharacter->IsValidLowLevel()) return;

	switch (guardState)
	{
	case EGuardState::Patrolling:

		if (currentPatrolIndex <= patrolRoutes.Num() - 1)
		{
			aiController->MoveToLocation(patrolRoutes[currentPatrolIndex]);

			if (FVector::Distance(GetActorLocation(), patrolRoutes[currentPatrolIndex]) <= 60.0f) ++currentPatrolIndex;
		}

		else currentPatrolIndex = 0;

		if (!hasDetectedPlayer)
		{
			FHitResult hitResult{};

			EDrawDebugTrace::Type drawDebugTrace = EDrawDebugTrace::None;

			// Shows the sphere trace only in development build or in editor
#if UE_BUILD_DEVELOPMENT
			drawDebugTrace = EDrawDebugTrace::ForOneFrame;
#endif

			if (!UKismetSystemLibrary::SphereTraceSingle(this, GetActorLocation(), 
				GetActorLocation() + GetActorForwardVector() * detectionForwardMultiplier, 40.0f,
				UEngineTypes::ConvertToTraceType(COLLISION_PLAYER), true, { ignoredActors }, drawDebugTrace, hitResult, true)) return;

			if (OnStartDetection.IsBound()) OnStartDetection.Broadcast(this);

			hasDetectedPlayer = true;
		}

		else
		{
			if (detectionValue < 1.0f)
			{
				FHitResult hitResult{};

				EDrawDebugTrace::Type drawDebugTrace = EDrawDebugTrace::None;

				// Shows the sphere trace only in development build or in editor
#if UE_BUILD_DEVELOPMENT
				drawDebugTrace = EDrawDebugTrace::ForOneFrame;
#endif

				if (UKismetSystemLibrary::SphereTraceSingle(this, GetActorLocation(),
					GetActorLocation() + GetActorForwardVector() * detectionForwardMultiplier, 40.0f,
					UEngineTypes::ConvertToTraceType(COLLISION_PLAYER), true, { ignoredActors }, drawDebugTrace, hitResult, true))
				{
					detectionValue += detectionSpeed * DeltaTime;

					if (OnDetection.IsBound()) OnDetection.Broadcast(this, detectionValue);

					if (detectionValue >= 1.0f)
					{
						PlayerDetected();
					}
				}

				else
				{
					detectionValue -= detectionSpeed * DeltaTime;
					if (OnDetection.IsBound()) OnDetection.Broadcast(this, detectionValue);

					if (detectionValue <= 0.0f)
					{
						detectionValue = 0.0f;
						if (OnStopDetection.IsBound()) OnStopDetection.Broadcast(this);

						hasDetectedPlayer = false;
					}
				}
			}
		}

		break;

	case EGuardState::SpottedPlayer:

		// Could do animation here
		if (OnDetectedAnimation.IsBound()) OnDetectedAnimation.Broadcast(this);
		break;

	default:
		break;
	}
}

// Called to bind functionality to input
void AGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGuard::SpawnSpottedCamera()
{
	if (!GetWorld()) return;

	FActorSpawnParameters spawnParams{};
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	cameraActor = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass(),
		GetActorLocation() + (GetActorForwardVector() * 50.0f) + (GetActorUpVector() * 60.0f), FRotator::ZeroRotator, spawnParams);

	if (!cameraActor) return;

	cameraActor->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(cameraActor->GetActorLocation(),
		GetActorLocation()));

	if (playerCharacter)
	{
		if (playerCharacter->GetCameraComponent())
		{
			FPostProcessSettings Settings = playerCharacter->GetCameraComponent()->PostProcessSettings;
			cameraActor->GetCameraComponent()->PostProcessSettings = Settings;
		}

		playerCharacter->Destroy();
		playerCharacter = nullptr;
	}

	if (AShroomJamGM* shroomJamGM = Cast<AShroomJamGM>(UGameplayStatics::GetGameMode(this)))
	{
		shroomJamGM->SetPlayerCharacterToNull();
	}

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		playerController->SetViewTargetWithBlend(cameraActor, 1.0f);
		playerController->Possess(nullptr);

		if (AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD()))
		{
			widgetManager->PopWidget(widgetManager->GetPlayerHUDClass());
		}
	}

	if (GetWorld())
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		if (!playerController) return;

		AWidgetManager* widgetManager = Cast<AWidgetManager>(playerController->GetHUD());
		if (!widgetManager) return;

		FTimerHandle timer{};
		GetWorld()->GetTimerManager().SetTimer(timer, [playerController, widgetManager]()
			{
				widgetManager->PushWidget(widgetManager->GetGameOverClass());
				widgetManager->EnableMouseCursor();
			}, 3.0f, false);
	}
}

void AGuard::PlayerDetected()
{
	detectionValue = 1.0f;
	if (OnDetection.IsBound()) OnDetection.Broadcast(this, detectionValue);

	if (playerSpottedSound) UGameplayStatics::PlaySound2D(this, playerSpottedSound);

	guardState = EGuardState::SpottedPlayer;

	if (aiController) aiController->StopMovement();
	
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), playerCharacter->GetActorLocation()));

	if (APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		playerCharacter->DisableInput(playerController);
	}

	for (TActorIterator<AGuard> otherGuards(GetWorld()); otherGuards; ++otherGuards)
	{
		if (*otherGuards == this) continue;

		if (otherGuards->GetOnStopDetection().IsBound()) otherGuards->GetOnStopDetection().Broadcast(*otherGuards);

		otherGuards->SetGuardState(EGuardState::SpottedPlayer);
		otherGuards->SetActorTickEnabled(false);
	}
}

void AGuard::MoveCameraToGuardThatSpottedPlayer()
{
	// Transition to game over state
	SpawnSpottedCamera();

	if (OnStopDetection.IsBound()) OnStopDetection.Broadcast(this);

	SetActorTickEnabled(false);
}

void AGuard::OnGuardHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this) return;

	if (playerCharacter == Cast<APlayerCharacter>(OtherActor))
	{
		PlayerDetected();
	}
}

