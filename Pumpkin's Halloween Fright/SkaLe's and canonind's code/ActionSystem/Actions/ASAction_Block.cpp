// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Actions/ASAction_Block.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "Characters/HJPlayerCharacter.h"
#include "Components/CombatComponent.h"

void UASAction_Block::StartAction(UActionSystemComponent* ActionComp)
{
	AActor* AvatarActor = ActionComp->GetAvatarActor();
	if (!AvatarActor) return;

	UCombatComponent* CombatComp = AvatarActor->GetComponentByClass<UCombatComponent>();
	if (!CombatComp) return;

	CombatComp->StartBlocking(DamageReductionFactor);
	if (!AvatarActor->GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set timer: GetWorld() returned nullptr."));
		return;
	}

	if (BlockMaxTime > 0.f)
	{
		AvatarActor->GetWorld()->GetTimerManager().SetTimer(StopBlock_TimerHandle, [this, ActionComp]()
			{
				if (ActionComp && ActionComp->IsValidLowLevel())
				{
					ActionComp->StopActionByTag(ActionTag);
				}
			}
		, BlockMaxTime, false);
	}
}

void UASAction_Block::StopAction(UActionSystemComponent* ActionComp)
{
	AActor* AvatarActor = ActionComp->GetAvatarActor();
	if (!AvatarActor) return;

	UCombatComponent* CombatComp = AvatarActor->GetComponentByClass<UCombatComponent>();
	CombatComp->StopBlocking();

	if (!AvatarActor->GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to clear timer: GetWorld() returned nullptr."));
		return;
	}

	AvatarActor->GetWorld()->GetTimerManager().ClearTimer(StopBlock_TimerHandle);
}
