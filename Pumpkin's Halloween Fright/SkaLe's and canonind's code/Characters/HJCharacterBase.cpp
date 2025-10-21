// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HJCharacterBase.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


AHJCharacterBase::AHJCharacterBase()
{

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

}

void AHJCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &AHJCharacterBase::OnHealthChanged);
		HealthComponent->OnDeath.AddDynamic(this, &AHJCharacterBase::OnDeath);
	}
	
}

void AHJCharacterBase::OnHealthChanged(UHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType)
{
	if (HealthDelta < 0.0f)
	{
		// Play hurt sound, animtion, particle
	}
	UE_LOG(LogTemp, Warning, TEXT("Character Health changed: %f"), HealthDelta);
}

void AHJCharacterBase::OnDeath(UHealthComponent* HealthComp, AActor* Killer, const class UDamageType* DamageType)
{
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Play death anim 
	// May be ragdoll
	UE_LOG(LogTemp, Warning, TEXT("Character Died"))
}

void AHJCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



