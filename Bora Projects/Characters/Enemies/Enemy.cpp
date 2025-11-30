// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Enemy.h"
#include "Widgets/EnemyWidget.h"
#include "Characters/Player/PlayerCharacter.h"
#include "Objects/Blender.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//aiPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	// Create and configure sight sense
	/*SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = 1200.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;*/

	// Add sight configuration
	/*aiPerception->ConfigureSense(*SightConfig);
	aiPerception->SetDominantSense(SightConfig->GetSenseImplementation());*/

	timerCleared = true;

	patrolIndex = 0;

	checkingOutDuckTimer = 0.0f;

	duckLocation = FVector::Zero();

	enemyState = EnemyState::EPatrolling;
	previousEnemyState = EnemyState::EPatrolling;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	aiController = Cast<AAIController>(GetController());

	if (!aiController)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI controller not available!"));
	}

	/*else if (aiController)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AI controller is available!"));

		// Set perception component from AI Controller class as the primary perception component
		aiController->SetPerceptionComponent(*aiPerception);
	}*/

	// Bind target perception update/forgotten delegate to the base enemy target callback functions
	/*if (aiPerception)
	{
		aiPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy::OnTargetPerceptionUpdated);
		aiPerception->OnTargetPerceptionForgotten.AddDynamic(this, &AEnemy::OnTargetPerceptionForgotten);
	}*/

	if (timerCleared != true) timerCleared = true;

	if (patrolIndex != 0) patrolIndex = 0;

	if (checkingOutDuckTimer != 0.0f) checkingOutDuckTimer = 0.0f;

	// Find a player in the level
	TArray<AActor*> FoundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundPlayer);

	player = Cast<APlayerCharacter>(FoundPlayer[0]);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	switch (enemyState)
	{
	case EnemyState::EPatrolling:

		// Test code for enemy detection and chasing the player
		if (FVector::Distance(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(),
			GetActorLocation()) <= distanceToDetect &&
			FVector::DotProduct(GetActorForwardVector(), GetDirectionVector(GetWorld()->GetFirstPlayerController()->
				GetPawn()->GetActorLocation())) >= minDetectionDotProductValue)
		{
			previousEnemyState = EnemyState::EPatrolling;
			enemyState = EnemyState::ESeenPlayer;
		}

		PatrolAround();
		break;

	case EnemyState::ECheckingOutDuck:

		// Test code for enemy detection and chasing the player
		if (FVector::Distance(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(),
			GetActorLocation()) <= distanceToDetect &&
			FVector::DotProduct(GetActorForwardVector(), GetDirectionVector(GetWorld()->GetFirstPlayerController()->
				GetPawn()->GetActorLocation())) >= minDetectionDotProductValue)
		{
			UpdateEnemyDetectionBarUponSeen(DeltaTime);
		}

		PatrolToDuckLocation(DeltaTime);
		break;

	case EnemyState::ELosingPlayer:

		// Test code for enemy detection and chasing the player
		if (FVector::Distance(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(),
			GetActorLocation()) <= distanceToDetect &&
			FVector::DotProduct(GetActorForwardVector(), GetDirectionVector(GetWorld()->GetFirstPlayerController()->
				GetPawn()->GetActorLocation())) >= minDetectionDotProductValue)
		{
			previousEnemyState = EnemyState::ELosingPlayer;
			enemyState = EnemyState::ESeenPlayer;
		}

		// Update enemy detection bar when enemy loses sight of player
		UpdateEnemyDetectionBarUponLost(DeltaTime);
		DamagePlayer(DeltaTime);

		PatrolAround();
		break;

	case EnemyState::ESeenPlayer:
		
		// Test code for enemy detection and losing sight of the player
		if (FVector::Distance(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(),
				GetActorLocation()) > distanceToDetect)
		{
			previousEnemyState = EnemyState::ESeenPlayer;
			enemyState = EnemyState::ELosingPlayer;
		}

		else if (FVector::DotProduct(GetActorForwardVector(), GetDirectionVector(
			GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation())) < minDetectionDotProductValue)
		{
			previousEnemyState = EnemyState::ESeenPlayer;
			enemyState = EnemyState::ELosingPlayer;
		}

		// Update enemy detection bar when player is seen
		UpdateEnemyDetectionBarUponSeen(DeltaTime);
		DamagePlayer(DeltaTime);

		PatrolAround();

		break;

	case EnemyState::EDetectedPlayer:

		DamagePlayer(DeltaTime);

		// Move the enemy towards the player
		aiController->MoveToLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), stoppingRadius);
		break;

	default:
		break;
	}
}

// Return a direction vector that can pass in a LEFT HAND reference argument
FVector AEnemy::GetDirectionVector(FVector& otherLocation_)
{
	return (otherLocation_ - GetActorLocation()).GetSafeNormal();
}

// Return a direction vector that can pass in a RIGHT HAND reference argument
FVector AEnemy::GetDirectionVector(FVector&& otherLocation_)
{
	return (otherLocation_ - GetActorLocation()).GetSafeNormal();
}

void AEnemy::UpdateEnemyDetectionBarUponSeen(float& DeltaTime)
{
	// If enemy hasn't fully detected the player yet
	if (!FMath::IsNearlyEqual(detectionValue, 1.15f, 0.1f))
	{
		// Clear the timer if needed only once
		if (!timerCleared)
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
			timerCleared = true;
		}

		// Make the enemy widget appear
		if (!widgetVisible)
		{
			widgetComponent->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			widgetVisible = true;
		}

		// Update the enemy detection bar using the enemy widget cast inside the widget component
		if (UEnemyWidget* enemyWidget = Cast<UEnemyWidget>(widgetComponent->GetWidget()))
		{
			// Increase the enemy's detection value by time
			detectionValue += DeltaTime;

			enemyWidget->SetEnemyDetectionBar(detectionValue);
			enemyWidget = nullptr;
		}
	}

	// Else the enemy has fully detected the player
	else
	{
		// Make sure widget is visible first to make it fully disappear after some time has passed
		if (widgetVisible)
		{
			if (UEnemyWidget* enemyWidget = Cast<UEnemyWidget>(widgetComponent->GetWidget()))
			{
				// Play the update bar size animation once player is detected
				if (!enemyWidget->GetIsBarAnimationPlaying())
				{
					// Call start animating blender to play its animation
					StartAnimatingBlender();

					// Play enemy detection sound
					UGameplayStatics::PlaySound2D(this, LoadObject<USoundBase>(this, 
						TEXT("/Game/Sounds/enemy_detection_1")));

					enemyWidget->PlayAnimation(enemyWidget->GetBarAnimation());
					enemyWidget->SetIsBarAnimationPlaying(true);

					enemyWidget = nullptr;
				}
			}

			FTimerHandle makeWidgetInvisibleTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(makeWidgetInvisibleTimerHandle, this, 
				&AEnemy::HideEnemyWidgetUponSeen, timeToHideWidget, false);
		}

		// Make sure player detected is true to prevent enemy from immediately losing sight of player
		if (enemyState != EnemyState::EDetectedPlayer) enemyState = EnemyState::EDetectedPlayer;

		// Update timer cleared once and make it false
		if (timerCleared != false) timerCleared = false;
	}
}

void AEnemy::UpdateEnemyDetectionBarUponLost(float& DeltaTime)
{
	// If enemy hasn't fully detected the player yet
	if (!FMath::IsNearlyEqual(detectionValue, -0.15f, 0.1f))
	{
		// Clear the timer if needed only once
		if (!timerCleared)
		{
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
			timerCleared = true;
		}

		// Make the enemy widget appear
		if (!widgetVisible)
		{
			widgetComponent->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			widgetVisible = true;
		}

		// Update the enemy detection bar using the enemy widget cast inside the widget component
		if (UEnemyWidget* enemyWidget = Cast<UEnemyWidget>(widgetComponent->GetWidget()))
		{
			// Decrease the enemy's detection value by time
			detectionValue -= DeltaTime;

			enemyWidget->SetEnemyDetectionBar(detectionValue);
			enemyWidget = nullptr;
		}
	}

	// Else the enemy has fully detected the player
	else
	{
		// Make sure widget is visible first to make it fully disappear after some time has passed
		if (widgetVisible)
		{
			FTimerHandle makeWidgetInvisibleTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(makeWidgetInvisibleTimerHandle, this, 
				&AEnemy::HideEnemyWidgetUponLost, timeToHideWidget, false);
		}

		// Update timer cleared once and make it false
		if (timerCleared != false) timerCleared = false;
	}
}

void AEnemy::HideEnemyWidgetUponSeen()
{
	if (UEnemyWidget* enemyWidget = Cast<UEnemyWidget>(widgetComponent->GetWidget()))
	{
		enemyWidget->SetIsBarAnimationPlaying(false);
		enemyWidget = nullptr;
	}

	detectionValue = 1.0f;

	if (widgetComponent) widgetComponent->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	widgetVisible = false;
}

void AEnemy::HideEnemyWidgetUponLost()
{
	if (widgetComponent) widgetComponent->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

	detectionValue = 0.0f;
	widgetVisible = false;

	if (enemyState != EnemyState::EPatrolling)
	{
		previousEnemyState = EnemyState::ELosingPlayer;
		enemyState = EnemyState::EPatrolling;
	}
}

void AEnemy::DamagePlayer(float& DeltaTime)
{
	if (player && FVector::Distance(player->GetActorLocation(), GetActorLocation()) <= 300.0f &&
		!player->GetHealthRegenerationComponent()->IsDead())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player found"));
		player->GetHealthRegenerationComponent()->StartDamaged.Broadcast(0.3f);
	}
}

void AEnemy::StartAnimatingBlender()
{
	TArray<AActor*> FoundBlender;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlender::StaticClass(), FoundBlender);

	for (AActor* blenderActor : FoundBlender)
	{
		ABlender* blender = Cast<ABlender>(blenderActor);
		blenderActor = nullptr;

		if (blender)
		{
			blender->PlayBlenderAlarmedAnimation();
			blender = nullptr;
		}
	}
}

void AEnemy::PatrolAround()
{
	if (aiController && !patrolRoutes.IsEmpty())
	{
		// Change patrol route after being near one of the patrol points
		if (FVector::Distance(GetActorLocation(), patrolRoutes[patrolIndex]) <= distanceToSwitchPatrolRoute)
		{
			if (patrolIndex < patrolRoutes.Num() - 1) ++patrolIndex;
			else patrolIndex = 0;
		}

		// Make sure the AI moves to some patrol route at its index
		else
		{
			aiController->MoveToLocation(patrolRoutes[patrolIndex], stopAtPatrolRadius);
		}
	}
}

void AEnemy::PatrolToDuckLocation(float& DeltaTime)
{
	if (aiController)
	{
		// Wait for a bit to check out the duck and start patrolling again
		if (FVector::Distance(GetActorLocation(), duckLocation) <= distanceToDuck)
		{
			checkingOutDuckTimer += DeltaTime;

			if (checkingOutDuckTimer >= timeToPatrolAgain)
			{
				checkingOutDuckTimer = 0.0f;

				previousEnemyState = EnemyState::ECheckingOutDuck;
				enemyState = EnemyState::EPatrolling;
			}
		}

		// Move the AI to the duck's location if it's not close to the duck yet
		else
		{
			aiController->MoveToLocation(duckLocation, stopAtPatrolRadius);
		}
	}
}

void AEnemy::SetEnemyState(EnemyState enemyState_)
{
	if (enemyState_ == EnemyState::EDetectedPlayer)
	{
		// Make enemy fully detect the player if the passed in state is set to detected player
		detectionValue = 1.1f;
	}

	enemyState = enemyState_;
}

void AEnemy::EnemyIsCloseToDuck()
{
	if (enemyState == EnemyState::ESeenPlayer) previousEnemyState = EnemyState::ESeenPlayer;
	enemyState = EnemyState::ECheckingOutDuck;
}

void AEnemy::SetEnemyToDetectPlayer()
{
	if (previousEnemyState == EnemyState::ESeenPlayer)
	{
		// Make enemy fully detect the player if they saw the player throw the duck near them
		detectionValue = 1.1f;

		// Also update the enemy detection bar in its widget
		if (UEnemyWidget* enemyWidget = Cast<UEnemyWidget>(widgetComponent->GetWidget()))
		{
			enemyWidget->SetEnemyDetectionBar(detectionValue);
			enemyWidget = nullptr;
		}

		enemyState = EnemyState::ESeenPlayer;
		previousEnemyState = EnemyState::ECheckingOutDuck;
	}
}

void AEnemy::GoToDuckLocation(FVector&& duckLocation_)
{
	duckLocation = duckLocation_;
}

/*void AEnemy::OnTargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)
{
	// Once the enemy sees the player
	if (TargetActor->IsA(APlayerCharacter::StaticClass()) && Stimulus.WasSuccessfullySensed() && !seenPlayer)
	{
		// TODO: Make AI do something once they see the player
		seenPlayer = true;
	}
}

void AEnemy::OnTargetPerceptionForgotten(AActor* TargetActor)
{
	// TODO: Make AI forget they ever saw something (potentially)
}*/