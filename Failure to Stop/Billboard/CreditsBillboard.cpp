// Fill out your copyright notice in the Description page of Project Settings.


#include "Billboard/CreditsBillboard.h"

// Sets default values
ACreditsBillboard::ACreditsBillboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	billboardHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BillboardHolder"));
	billboardHolder->SetupAttachment(GetRootComponent());

	billboardSign = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BillboardSign"));
	billboardSign->SetupAttachment(billboardHolder);
}

// Called when the game starts or when spawned
void ACreditsBillboard::BeginPlay()
{
	Super::BeginPlay();
}

void ACreditsBillboard::ConstructCreditsWidget()
{
	if (!billboardWidgetComponent && creditsWidgetClass)
	{
		// Create a new billboard widget component object
		billboardWidgetComponent = NewObject<UWidgetComponent>(this, UWidgetComponent::StaticClass(), 
			TEXT("BillboardWidgetComponent"));

		billboardWidgetComponent->SetupAttachment(billboardSign);

		// Register the created component during runtime
		billboardWidgetComponent->RegisterComponent();

		// Set the billboard widget component transform values
		billboardWidgetComponent->SetRelativeLocation(widgetComponentSpawnLocation);
		billboardWidgetComponent->SetRelativeRotation(widgetComponentSpawnRotation);
		billboardWidgetComponent->SetRelativeScale3D(widgetComponentSpawnScale);

		// Also set its widget class and draw size with it
		billboardWidgetComponent->SetWidgetClass(creditsWidgetClass);
		billboardWidgetComponent->SetDrawSize(widgetComponentDrawSize);
	}
}

void ACreditsBillboard::DestroyCreditsWidget()
{
	if (billboardWidgetComponent && billboardWidgetComponent->GetWidgetClass())
	{
		// Set widget class to null and destroy the billboard widget component with it
		billboardWidgetComponent->SetWidgetClass(nullptr);
		billboardWidgetComponent->DestroyComponent();
		billboardWidgetComponent = nullptr;
	}
}