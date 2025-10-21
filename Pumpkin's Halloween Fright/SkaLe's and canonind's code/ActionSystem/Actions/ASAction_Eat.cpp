// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Actions/ASAction_Eat.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

bool UASAction_Eat::OnAnimNotifyReceived(UActionSystemComponent* ActionComp, FName EventName)
{
	bool bHandled = Super::OnAnimNotifyReceived(ActionComp, EventName);
	if (bHandled) return bHandled;

	if (EventName == "SpawnCandy")
	{
		SpawnCandy(ActionComp);
		return true;
	}
	else if (EventName == "EatCandy")
	{
		EatCandy(ActionComp);
		return true;
	}
	return false;
}

void UASAction_Eat::SpawnCandy(UActionSystemComponent* ActionComp)
{
	if (!ActionComp) return;

	AActor* Avatar = ActionComp->GetAvatarActor();
	if (!Avatar) return;
	ACharacter* AvatarChar = Cast<ACharacter>(Avatar);
	if (!AvatarChar) return;
	USkeletalMeshComponent* AvatarMesh = AvatarChar->GetMesh();
	if (!AvatarMesh)return;

	if (AvatarMesh->DoesSocketExist(SpawnSocket))
	{
		FVector SpawnLocation = AvatarMesh->GetSocketLocation(SpawnSocket);
		FRotator SpawnRotation = Avatar->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Avatar;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CandyActor = Avatar->GetWorld()->SpawnActor<AActor>(CandyClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (CandyActor)
		{
			USceneComponent* RootComp = CandyActor->GetRootComponent();
			if (RootComp)
			{
				RootComp->AttachToComponent(AvatarChar->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SpawnSocket);
			}
		}
	}

}

void UASAction_Eat::EatCandy(UActionSystemComponent* ActionComp)
{
	if (!ActionComp) return;

	AActor* Avatar = ActionComp->GetAvatarActor();
	if (!Avatar) return;
	ACharacter* AvatarChar = Cast<ACharacter>(Avatar);
	if (!AvatarChar) return;

	UInventoryComponent* Inventory = AvatarChar->GetComponentByClass<UInventoryComponent>();
	if (!Inventory) return;

	UHealthComponent* Health = AvatarChar->GetComponentByClass<UHealthComponent>();
	if (!Health) return;

	float HealAmount = Inventory->GetCandyHealAmount();

	Inventory->EatCandy();
	Health->Heal(HealAmount);

	if (!CandyActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASACtion_Eat::EatCandy: Candy actor is not valid!"));
	}

	SpawnSound(CandyActor->GetActorLocation());
	SpawnVisualEffect(CandyActor->GetActorLocation(), CandyActor->GetActorRotation());

	if (CandyActor)
	{
		CandyActor->Destroy();
	}
}

void UASAction_Eat::SpawnSound(const FVector& Location)
{
	if (!Sound) return;

	UAudioComponent* AudioComp = UGameplayStatics::SpawnSoundAtLocation(
		CandyActor,
		Sound,
		Location,
		FRotator::ZeroRotator,
		Volume,
		Pitch
		
	);
}

void UASAction_Eat::SpawnVisualEffect(const FVector& Location, const FRotator& Rotation)
{
	if (!NiagaraSystem) return;

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		CandyActor,
		NiagaraSystem,
		Location,
		Rotation,
		FVector(Scale),
		true,
		true,
		ENCPoolMethod::AutoRelease
	);
}
