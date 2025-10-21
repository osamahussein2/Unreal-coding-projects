// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/AnimNotify_ActionEvent.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "GameFramework/Character.h"


void UAnimNotify_ActionEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (UActionSystemComponent* ActionSystem = Character->FindComponentByClass<UActionSystemComponent>())
		{
			ActionSystem->HandleAnimNotifyActionEvent(EventName);
		}
	}
}
