// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ASAction_MontagePlay.h"
#include "ActionSystem/ActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

bool UASAction_MontagePlay::OnAnimNotifyReceived(UActionSystemComponent* ActionComp, FName EventName)
{
	if (EventName == "MontageEarlyEnd")
	{
		ActionComp->StopActionByTag(ActionTag);
		return true;
	}
	return false;
}

void UASAction_MontagePlay::StartAction(UActionSystemComponent* ActionComp)
{
	if (!ActionComp) return;
	OwningActionComp = ActionComp;

	if (ActionMontage.IsValid())
	{
		UAnimMontage* Montage = ActionMontage.ActionMontage;
		const float PlayRate = ActionMontage.PlayRate;
		const FName StartSection = ActionMontage.GetMontageSectionName(SectionIndex);
		float Len = ActionComp->PlayActionMontage(Montage, PlayRate, StartSection);

		if (ACharacter* AvatarChar = Cast<ACharacter>(ActionComp->GetAvatarActor()))
		{
			if (UAnimInstance* AnimInst = AvatarChar->GetMesh()->GetAnimInstance())
			{
				AnimInst->OnMontageEnded.AddUniqueDynamic(this, &UASAction_MontagePlay::OnAnimMontageEnded);
			}
		}
	}
}

void UASAction_MontagePlay::StopAction(UActionSystemComponent* ActionComp)
{
	if (!ActionComp) return;

	if (ACharacter* AvatarChar = Cast<ACharacter>(ActionComp->GetAvatarActor()))
	{
		if (UAnimInstance* AnimInst = AvatarChar->GetMesh()->GetAnimInstance())
		{
			AnimInst->OnMontageEnded.RemoveAll(this);
			if (ActionMontage.IsValid())
			{
				AnimInst->Montage_Stop(MontageStopBlendTime, ActionMontage.ActionMontage);
			}
		}
	}
	OwningActionComp = nullptr;
}

void UASAction_MontagePlay::RestartAction(UActionSystemComponent* ActionComp)
{
	if (!OwningActionComp || !ActionMontage.IsValid()) return;

	ACharacter* AvatarChar = Cast<ACharacter>(OwningActionComp->GetAvatarActor());
	if (!AvatarChar) return;

	UAnimInstance* AnimInst = AvatarChar->GetMesh()->GetAnimInstance();
	if (!AnimInst) return;

	UAnimMontage* Montage = ActionMontage.ActionMontage;
	FName SectionName = ActionMontage.GetMontageSectionName(SectionIndex);

	if (AnimInst->Montage_IsPlaying(Montage))
	{
		AnimInst->Montage_JumpToSection(SectionName, Montage);
		AnimInst->Montage_SetPosition(Montage, 0.f);
	}
	else
	{
		AnimInst->Montage_Play(Montage, ActionMontage.PlayRate);
		AnimInst->Montage_JumpToSection(SectionName, Montage);
	}
}

void UASAction_MontagePlay::OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!ActionMontage.IsValid() || Montage != ActionMontage.ActionMontage) return;

	if (OwningActionComp)
	{
		OwningActionComp->StopActionByTag(ActionTag);
	}
}
