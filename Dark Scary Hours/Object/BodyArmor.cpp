// Fill out your copyright notice in the Description page of Project Settings.

#include "Object/BodyArmor.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABodyArmor::ABodyArmor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bodyArmorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyArmorMesh"));
	bodyArmorMesh->SetupAttachment(GetRootComponent());

	bodyArmorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyArmorMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	glowingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlowingMesh"));
	glowingMesh->SetupAttachment(GetRootComponent());

	glowingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	glowingMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(glowingMesh);

	boxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABodyArmor::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void ABodyArmor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABodyArmor::DestroyGlowingMesh()
{
	if (boxComponent)
	{
		boxComponent->UnregisterComponent();
		boxComponent->DestroyComponent();
		boxComponent = nullptr;
	}

	if (glowingMesh)
	{
		glowingMesh->UnregisterComponent();
		glowingMesh->DestroyComponent();
		glowingMesh = nullptr;
	}
}

void ABodyArmor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			player->ApplyArmor();
		}

		DestroyGlowingMesh();
		Destroy();
	}
}