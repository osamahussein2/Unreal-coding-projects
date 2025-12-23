// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/ShotIndicator.h"
#include "Player/PlayerCharacter.h"

// Sets default values
AShotIndicator::AShotIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	spriteComponent->SetupAttachment(RootComponent);

	sceneComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SceneComponent"));
	sceneComponent->SetupAttachment(spriteComponent);

	rotationFlipped = false;
	didRotationFlipFirstTime = false;
}

// Called when the game starts or when spawned
void AShotIndicator::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> foundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), foundPlayer);

	playerCharacter = Cast<APlayerCharacter>(foundPlayer[0]);

	foundPlayer[0] = nullptr;
}

// Called every frame
void AShotIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!rotationFlipped && playerCharacter)
	{
		spriteComponent->AddRelativeRotation(playerCharacter->GetAnimateLeftRotation() * DeltaTime);

		rotationTimer += DeltaTime;

		// Make sure it does double the time to flip rotation once rotation has flipped for the first time
		if (rotationTimer >= playerCharacter->GetTimeToFlipRotation() * 2.0f && didRotationFlipFirstTime)
		{
			rotationTimer = 0.0f;
			rotationFlipped = true;
		}

		// Otherwise, flip rotation by whatever the time to flip rotation is if rotation didn't flip for the first time
		else if (rotationTimer >= playerCharacter->GetTimeToFlipRotation() && !didRotationFlipFirstTime)
		{
			rotationTimer = 0.0f;
			rotationFlipped = true;

			didRotationFlipFirstTime = true;
		}
	}

	else if (rotationFlipped && playerCharacter)
	{
		spriteComponent->AddRelativeRotation(playerCharacter->GetAnimateRightRotation() * DeltaTime);

		rotationTimer += DeltaTime;

		// Make sure it does double the time to flip rotation
		if (rotationTimer >= playerCharacter->GetTimeToFlipRotation() * 2.0f)
		{
			rotationTimer = 0.0f;
			rotationFlipped = false;
		}
	}
}

USceneComponent* AShotIndicator::GetSceneComponent() const
{
	return sceneComponent;
}

void AShotIndicator::InvalidateShotIndicatorReferences()
{
	if (spriteComponent)
	{
		spriteComponent->DestroyComponent();
		spriteComponent = nullptr;
	}

	if (sceneComponent)
	{
		sceneComponent->DestroyComponent();
		sceneComponent = nullptr;
	}

	if (playerCharacter) playerCharacter = nullptr;
}