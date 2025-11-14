// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Enemy.h"
#include "Widgets/EnemyWidget.h"
#include "Characters/Player/PlayerCharacter.h"

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

	seenPlayer = false;
	timerCleared = true;

	playerNear = false;
	playerDetected = false;
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

	if (seenPlayer != false) seenPlayer = false;
	if (timerCleared != true) timerCleared = true;
	if (playerNear != false) playerNear = false;
	if (playerDetected != false) playerDetected = false;

	// Find a player in the level
	TArray<AActor*> FoundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundPlayer);

	player = Cast<APlayerCharacter>(FoundPlayer[0]);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (aiController && seenPlayer)
	{
		UpdateEnemyDetectionBarUponSeen(DeltaTime);
		DamagePlayer(DeltaTime);
	}

	if (!seenPlayer && playerNear && !playerDetected)
	{
		UpdateEnemyDetectionBarUponLost(DeltaTime);
	}

	UpdatePlayerSeenBoolean();
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

void AEnemy::UpdatePlayerSeenBoolean()
{
	// Test code for enemy detection and chasing the player
	if (!seenPlayer && FVector::Distance(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(),
		GetActorLocation()) <= distanceToDetect &&
		FVector::DotProduct(GetActorForwardVector(), GetDirectionVector(GetWorld()->GetFirstPlayerController()->
			GetPawn()->GetActorLocation())) >= minDetectionDotProductValue)
	{
		seenPlayer = true;
	}

	// Test code for enemy detection and losing sight of the player
	else if (seenPlayer && FVector::Distance(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(),
		GetActorLocation()) > distanceToDetect && !playerDetected)
	{
		seenPlayer = false;
	}

	else if (seenPlayer && FVector::DotProduct(GetActorForwardVector(), GetDirectionVector(GetWorld()->
		GetFirstPlayerController()->GetPawn()->GetActorLocation())) < minDetectionDotProductValue && !playerDetected)
	{
		seenPlayer = false;
	}
}

void AEnemy::UpdateEnemyDetectionBarUponSeen(float& DeltaTime)
{
	// If enemy hasn't fully detected the player yet
	if (!FMath::IsNearlyEqual(detectionValue, 1.15f, 0.1f))
	{
		// Make sure the player near is set to true first to update the enemy losing sight of player logic
		if (playerNear != true) playerNear = true;

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
		if (playerDetected != true) playerDetected = true;

		// Update timer cleared once and make it false
		if (timerCleared != false) timerCleared = false;

		// Move the enemy towards the player
		aiController->MoveToLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), stoppingRadius);
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

	if (widgetComponent) widgetComponent->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	widgetVisible = false;
}

void AEnemy::HideEnemyWidgetUponLost()
{
	if (widgetComponent) widgetComponent->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

	detectionValue = 0.0f;
	widgetVisible = false;

	playerNear = false;
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