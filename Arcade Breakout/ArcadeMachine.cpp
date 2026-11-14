// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcadeMachine.h"
#include "Components/BoxComponent.h"
#include "Player/PlayerCharacter.h"

// Sets default values
AArcadeMachine::AArcadeMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	arcadeMachine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArcadeMachine"));
	SetRootComponent(arcadeMachine);

	arcadeMachine->SetCollisionResponseToAllChannels(ECR_Block);
	arcadeMachine->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	boxComponent->SetupAttachment(arcadeMachine);

	boxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	boxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Ignore);
	boxComponent->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);

	boxComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AArcadeMachine::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	
	if (boxComponent)
	{
		boxComponent->OnComponentBeginOverlap.AddDynamic(this, &AArcadeMachine::OnComponentBeginOverlap);
		boxComponent->OnComponentEndOverlap.AddDynamic(this, &AArcadeMachine::OnComponentEndOverlap);
	}
}

// Called every frame
void AArcadeMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (matInstanceDynamic)
	{
		if (!colorLerpChanged)
		{
			timer += lerpSpeed * DeltaTime;

			matInstanceDynamic->SetVectorParameterValue(TEXT("BaseColorFactor"),
				FLinearColor::LerpUsingHSV(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), playerInTriggerColor, timer));

			if (timer >= 1.0f)
			{
				timer = 0.0f;
				colorLerpChanged = true;
			}
		}

		else
		{
			timer += lerpSpeed * DeltaTime;

			matInstanceDynamic->SetVectorParameterValue(TEXT("BaseColorFactor"),
				FLinearColor::LerpUsingHSV(playerInTriggerColor, FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), timer));

			if (timer >= 1.0f)
			{
				timer = 0.0f;
				colorLerpChanged = false;
			}
		}
	}
}

void AArcadeMachine::RemoveCoinFromArray(ACoin* coinToRemove_)
{
	if (!coinToRemove_) return;

	coins.Remove(coinToRemove_);
}

void AArcadeMachine::StopFlashing()
{
	if (IsActorTickEnabled()) SetActorTickEnabled(false);

	if (!matInstanceDynamic) return;

	matInstanceDynamic->SetVectorParameterValue(TEXT("BaseColorFactor"), FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
}

void AArcadeMachine::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (coins.IsEmpty()) return;

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		playerInTrigger = true;

		if (arcadeMachine)
		{
			if (!matInstanceDynamic)
			{
				matInstanceDynamic = UMaterialInstanceDynamic::Create(arcadeMachine->GetMaterial(0), this);
				if (!matInstanceDynamic) return;

				arcadeMachine->SetMaterial(0, matInstanceDynamic); // Assign the dynamic material instance
			}

			SetActorTickEnabled(true);
		}
	}
}

void AArcadeMachine::OnComponentEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (coins.IsEmpty()) return;

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		playerInTrigger = false;

		if (arcadeMachine)
		{
			SetActorTickEnabled(false);

			if (colorLerpChanged) colorLerpChanged = false;
			if (timer != 0.0f) timer = 0.0f;

			if (!matInstanceDynamic)
			{
				matInstanceDynamic = UMaterialInstanceDynamic::Create(arcadeMachine->GetMaterial(0), this);
				if (!matInstanceDynamic) return;

				arcadeMachine->SetMaterial(0, matInstanceDynamic); // Assign the dynamic material instance
				matInstanceDynamic->SetVectorParameterValue(TEXT("BaseColorFactor"), FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
			}

			else
			{
				matInstanceDynamic->SetVectorParameterValue(TEXT("BaseColorFactor"), FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
			}
		}
	}
}

