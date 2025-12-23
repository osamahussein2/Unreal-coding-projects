// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/SoccerBall.h"
#include "Player/PlayerCharacter.h"
#include "Triggers/GoalTrigger.h"
#include "Triggers/OutOfBoundsTrigger.h"
#include "Triggers/InsidePostTrigger.h"
#include "Triggers/OutsidePostTrigger.h"
#include "Goalie/Goalkeeper.h"
#include "Objects/ShotIndicator.h"

// Sets default values
ASoccerBall::ASoccerBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	spriteComponent->SetupAttachment(RootComponent);

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	sphereComponent->SetupAttachment(spriteComponent);

	sphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASoccerBall::OnComponentBeginOverlap);

	ballState = BallState::ENormal;

	ballLocation = FVector::ZeroVector;
	shootDirection = FVector::ZeroVector;

	initialBallPosition = FVector::ZeroVector;

	shotIndicatorLocation = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void ASoccerBall::BeginPlay()
{
	Super::BeginPlay();
	
	initialBallPosition = GetActorLocation();
}

// Called every frame
void ASoccerBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (playerShotBall)
	{
		switch (ballState)
		{
		case BallState::ENormal:

			// Move the soccer ball to its intended direction
			ballLocation = shotIndicatorLocation - GetActorLocation();
			shootDirection = FVector(ballLocation.X, ballLocation.Y, ballLocation.Z);

			ballLocation += (shootDirection - GetActorLocation()).GetSafeNormal() * ballSpeed * DeltaTime;

			SetActorLocation(ballLocation);
			break;

		case BallState::EReverseDirection:

			// Move the soccer ball to its reversed direction
			ballLocation = -shotIndicatorLocation - GetActorLocation();
			shootDirection = FVector(ballLocation.X, ballLocation.Y, ballLocation.Z);

			ballLocation += (shootDirection - GetActorLocation()).GetSafeNormal() * reverseBallSpeed * DeltaTime;

			SetActorLocation(ballLocation);
			break;

		case BallState::EHitInsidePost:

			// Move the soccer ball to its reversed x direction only once the ball has hit the inside post
			ballLocation = FVector(-shotIndicatorLocation.X, shotIndicatorLocation.Y, shotIndicatorLocation.Z) - GetActorLocation();
			shootDirection = FVector(ballLocation.X, ballLocation.Y, ballLocation.Z);

			ballLocation += (shootDirection - GetActorLocation()).GetSafeNormal() * ballSpeed * DeltaTime;

			SetActorLocation(ballLocation);
			break;

		default:
			break;
		}
	}
}

bool ASoccerBall::GetPlayerShotBall() const
{
	return playerShotBall;
}

void ASoccerBall::SetPlayerShotBall(bool value_)
{
	playerShotBall = value_;
}

void ASoccerBall::ResetSoccerBall()
{
	// Reset all soccer ball variables if necessary
	if (GetActorLocation() != initialBallPosition) SetActorLocation(initialBallPosition);
	if (ballState != BallState::ENormal) ballState = BallState::ENormal;
	if (ballLocation != FVector::ZeroVector) ballLocation = FVector::ZeroVector;
	if (shootDirection != FVector::ZeroVector) shootDirection = FVector::ZeroVector;
	if (shotIndicatorLocation != FVector::ZeroVector) shotIndicatorLocation = FVector::ZeroVector;
	if (shotIndicatorClass && !shotIndicator) SpawnShotIndicator();
	if (playerShotBall != false) playerShotBall = false;
}

void ASoccerBall::SpawnShotIndicator()
{
	FVector SpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y - 15.0f, GetActorLocation().Z + 0.5f);
	FRotator SpawnRotation = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + 180.0f, GetActorRotation().Roll);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	shotIndicator = GetWorld()->SpawnActor<AShotIndicator>(shotIndicatorClass, SpawnLocation,
		SpawnRotation, SpawnParams);
}

void ASoccerBall::DestroyShotIndicator()
{
	if (shotIndicator)
	{
		shotIndicator->InvalidateShotIndicatorReferences();
		shotIndicator->Destroy();
		shotIndicator = nullptr;
	}
}

void ASoccerBall::SetShotIndicatorLocation()
{
	if (shotIndicator)
	{
		// Set the shot indicator location as soon as player shoots the ball only if shot indicator is valid
		shotIndicatorLocation = shotIndicator->GetSceneComponent()->GetComponentLocation();
	}
}

void ASoccerBall::IncreaseBallSpeed(int value_)
{
	ballSpeed += value_;
}

void ASoccerBall::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// If the ball overlaps the goal trigger while the player shot the ball
	if (OtherActor->IsA(AGoalTrigger::StaticClass()) && playerShotBall)
	{
		TArray<AActor*> foundPlayer;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

		// Increase player's score by 1
		if (Cast<APlayerCharacter>(foundPlayer[0]))
		{
			Cast<APlayerCharacter>(foundPlayer[0])->IncrementPlayerScore();

			foundPlayer[0] = nullptr;
		}

		ResetSoccerBall();
	}

	// If the ball overlaps the out of bounds trigger instead while the player shot the ball
	if (OtherActor->IsA(AOutOfBoundsTrigger::StaticClass()) && playerShotBall)
	{
		ResetSoccerBall();
	}

	// If the ball overlaps the goalkeeper's trigger instead while the player shot the ball
	if (OtherActor->IsA(AGoalkeeper::StaticClass()) && playerShotBall)
	{
		TArray<AActor*> foundGoalkeeper;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoalkeeper::StaticClass(), foundGoalkeeper);

		// If the ball's y position is greater than the found goalkeeper in level's y position (where its overlap trigger is)
		if (Cast<AGoalkeeper>(foundGoalkeeper[0]) && 
			Cast<AGoalkeeper>(foundGoalkeeper[0])->GetActorLocation().Y + 15.0f <= GetActorLocation().Y)
		{
			ballState = BallState::EReverseDirection;

			FTimerHandle timerHandle;
			GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ASoccerBall::ResetSoccerBall, timeToRespawnBall, false);
		}

		if (foundGoalkeeper[0]) foundGoalkeeper[0] = nullptr;
	}

	// If the ball overlaps the inside post trigger instead while the player shot the ball (without the ball reversing its direction)
	if (OtherActor->IsA(AInsidePostTrigger::StaticClass()) && playerShotBall && ballState != BallState::EReverseDirection)
	{
		ballState = BallState::EHitInsidePost;
	}

	// If the ball overlaps the outside post trigger instead while the player shot the ball (without the ball hitting the inside post)
	if (OtherActor->IsA(AOutsidePostTrigger::StaticClass()) && playerShotBall && ballState != BallState::EHitInsidePost)
	{
		ballState = BallState::EReverseDirection;

		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ASoccerBall::ResetSoccerBall, timeToRespawnBall, false);
	}
}
