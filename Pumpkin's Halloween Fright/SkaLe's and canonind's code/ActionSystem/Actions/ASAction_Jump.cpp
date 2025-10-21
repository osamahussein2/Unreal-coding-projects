// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/Actions/ASAction_Jump.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "Characters/HJPlayerCharacter.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void UASAction_Jump::StartAction(UActionSystemComponent* ActionComp)
{
	AActor* AvatarActor = ActionComp->GetAvatarActor();
	if (!AvatarActor) return;
	ACharacter* AvatarChar = Cast<ACharacter>(AvatarActor);
	if (!AvatarChar) return;
	AHJPlayerCharacter* PlayerChar = Cast<AHJPlayerCharacter>(AvatarChar);
	if (!PlayerChar) return;

	PlayerChar->MovementModeChangedDelegate.AddDynamic(this, &UASAction_Jump::EndJump);

	OwningActionComp = ActionComp;
	PerformJump(ActionComp);
}

void UASAction_Jump::StopAction(UActionSystemComponent* ActionComp)
{
	AActor* AvatarActor = ActionComp->GetAvatarActor();
	if (!AvatarActor) return;
	ACharacter* AvatarChar = Cast<ACharacter>(AvatarActor);
	if (!AvatarChar) return;

	AvatarChar->StopJumping();

	AHJPlayerCharacter* PlayerChar = Cast<AHJPlayerCharacter>(AvatarChar);
	if (!PlayerChar) return;
}

void UASAction_Jump::PerformJump(UActionSystemComponent* ActionComp)
{
	AActor* AvatarActor = ActionComp->GetAvatarActor();
	if (!AvatarActor) return;
	ACharacter* AvatarChar = Cast<ACharacter>(AvatarActor);
	if (!AvatarChar) return;

	AvatarChar->Jump();
}

void UASAction_Jump::EndJump(class ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	if (PrevMovementMode == EMovementMode::MOVE_Falling)
	{
		OwningActionComp->StopActionByTag(ActionTag);
		Character->MovementModeChangedDelegate.RemoveDynamic(this, &UASAction_Jump::EndJump);
	}
}
