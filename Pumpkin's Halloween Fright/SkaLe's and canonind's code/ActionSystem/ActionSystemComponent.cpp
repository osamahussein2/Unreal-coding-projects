// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ActionSystemComponent.h"
#include "ActionSystem/ASAction.h"
#include "GameFramework/Character.h"

UActionSystemComponent::UActionSystemComponent()
{

}


void UActionSystemComponent::HandleAnimNotifyActionEvent(FName EventName)
{
	for (UASAction* ActiveAction : ActiveActions)
	{
		if (ActiveAction)
		{
			bool bHandled = ActiveAction->OnAnimNotifyReceived(this, EventName);
			if (bHandled) return;
		}
	}
}

void UActionSystemComponent::GrantActiveTags(const FGameplayTagContainer& InTags)
{
	ActiveGameplayTags.AppendTags(InTags);
	OnActiveTagsUpdated.Broadcast(InTags);
}

void UActionSystemComponent::ClearActiveTags(const FGameplayTagContainer& InTags)
{
	ActiveGameplayTags.RemoveTags(InTags);
	OnActiveTagsUpdated.Broadcast(InTags);
}

void UActionSystemComponent::GiveAction(UASAction* InAction)
{
	if (!InAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action Not Valid"));
	}
	AvailableActions.Add(InAction);
}

void UActionSystemComponent::ClearAction(UASAction* InAction)
{
	if (!InAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action Not Valid"));
	}
	AvailableActions.Remove(InAction);
}

void UActionSystemComponent::AddActiveAction(UASAction* InAction)
{
	if (!InAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action Not Valid"));
	}
	ActiveActions.Add(InAction);
}

void UActionSystemComponent::ClearActiveAction(UASAction* InAction)
{
	if (!InAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action Not Valid"));
	}
	ActiveActions.Remove(InAction);
}

bool UActionSystemComponent::StartActionByTag(const FGameplayTag ActionTag)
{
	bool bWantsToRestart = false;
	if (IsActionRunning(ActionTag))
	{
// 		if (CanActionRestartSelf(ActionTag))
// 		{
// 
// 			bWantsToRestart = true;
// 
// 		}
// 		else
// 		{
// 			// Can't restart
			return false;
//		}
	}

	for (UASAction* Action : AvailableActions)
	{
		if (Action && Action->ActionTag == ActionTag)
		{
			if (bWantsToRestart)
			{
				Action->RestartAction(this);
			}
			else
			{
				if (!Action->CanStartAction(this)) { continue; }
				ClearActiveAction(Action);
				Action->StartAction(this);
				GrantActiveTags(Action->GrantedTags);
				AddActiveAction(Action);
				OnActionStart.Broadcast(ActionTag);
				return true;
			}

		}
	}
	return false;
}

bool UActionSystemComponent::StopActionByTag(const FGameplayTag ActionTag)
{
	for (UASAction* Action : ActiveActions)
	{
		if (Action && Action->ActionTag == ActionTag)
		{

			if (!Action->CanStopAction(this)) { continue; }
			Action->StopAction(this);
			ClearActiveTags(Action->GrantedTags);
			ClearActiveAction(Action);
			OnActionStop.Broadcast(ActionTag);
			return true;
		}
	}
	return false;
}

bool UActionSystemComponent::IsActionRunning(const FGameplayTag ActionTag) const
{
	for (const UASAction* Element : ActiveActions)
	{
		if (Element && Element->ActionTag == ActionTag)
		{
			return true;
		}
	}
	return false;
}

bool UActionSystemComponent::CanActionRestartSelf(const FGameplayTag ActionTag) const
{
	for (const UASAction* Element : ActiveActions)
	{
		if (Element && Element->ActionTag == ActionTag)
		{
			return Element->bCanSelfRestart;
		}
	}
	return false;
}

float UActionSystemComponent::PlayActionMontage(class UAnimMontage* MontageToPlay, const float PlayRate /*= 1.f*/, const FName& StartSection /*= NAME_None*/)
{
	if (!MontageToPlay)
	{
		UE_LOG(LogTemp, Warning, TEXT("Montage not valid"));
		return 0.f;
	}

	if (ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActor()))
	{
		return AvatarCharacter->PlayAnimMontage(MontageToPlay, PlayRate, StartSection);
	}
	return 0.f;
}

AActor* UActionSystemComponent::GetAvatarActor() const
{
	if (AvatarActor)
	{
		return AvatarActor;
	}
	return GetOwner();
}

void UActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	for (UASAction* Action : DefaultActions)
	{
		GiveAction(Action);
	}
}


