// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/ASAction.h"
#include "ActionSystem/ActionSystemComponent.h"


void UASAction::StartAction(UActionSystemComponent* ActionComp)
{

}

void UASAction::StopAction(UActionSystemComponent* ActionComp)
{

}

void UASAction::RestartAction(UActionSystemComponent* ActionComp)
{

}

bool UASAction::CanStartAction(UActionSystemComponent* ActionComp) const
{
	if (ActionComp)
	{
		return DoesSatisfyTagRequirements(ActionComp->ActiveGameplayTags);
	}
	return true;
}

bool UASAction::CanStopAction(UActionSystemComponent* ActionComp) const
{
	return true;
}

bool UASAction::DoesSatisfyTagRequirements(const FGameplayTagContainer& ActiveGameplayTags) const
{
	return !ActiveGameplayTags.HasAny(BlockedTags);
}
