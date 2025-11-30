// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClasses/MainEnemy.h"

// Sets default values
AMainEnemy::AMainEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	regularHealthComponent = CreateDefaultSubobject<URegularHealthComponent>(TEXT("RegularHealthComponent"));

	spriteComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MinimapSprite"));
	spriteComponent->SetupAttachment(GetMesh());

	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	spriteComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyWidgetComponent"));
	widgetComponent->SetupAttachment(GetMesh());

	detectionValue = 0.0f;
	widgetVisible = false;
}

// Called when the game starts or when spawned
void AMainEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (regularHealthComponent)
	{
		regularHealthComponent->StartDamaged.AddDynamic(this, &AMainEnemy::SetEnemyDamaged);
	}
	
	if (detectionValue <= 0.0f && widgetComponent)
	{
		widgetComponent->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));

		if (widgetVisible != false) widgetVisible = false;
	}
}

// Called every frame
void AMainEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainEnemy::SetEnemyDamaged(float newValue)
{
	if (regularHealthComponent)
	{
		regularHealthComponent->SetDamaged(newValue);
	}
}
