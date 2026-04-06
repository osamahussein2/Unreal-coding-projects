// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ActionComponent.h"

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	GameplayActiveTagsRemoved.AddUniqueDynamic(this, &UActionComponent::RemoveActiveActionTags);
	GameplayBlockedTagsRemoved.AddUniqueDynamic(this, &UActionComponent::RemoveBlockedActionTags);

	GameplayActiveTagsAdded.AddUniqueDynamic(this, &UActionComponent::AddActiveActionTags);
	GameplayBlockedTagsAdded.AddUniqueDynamic(this, &UActionComponent::AddBlockedActionTags);
}


// Called every frame
void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActionComponent::RemoveActiveActionTags(TArray<FGameplayTag> RemovedTags)
{
	for (FGameplayTag& activeActionTag : RemovedTags)
	{
		if (ActiveGameplayTags.HasTag(activeActionTag))
		{
			ActiveGameplayTags.RemoveTag(activeActionTag);
		}
	}
}

void UActionComponent::RemoveBlockedActionTags(TArray<FGameplayTag> RemovedTags)
{
	for (FGameplayTag& blockedActionTag : RemovedTags)
	{
		if (BlockedGameplayTags.HasTag(blockedActionTag))
		{
			BlockedGameplayTags.RemoveTag(blockedActionTag);
		}
	}
}

void UActionComponent::AddActiveActionTags(TArray<FGameplayTag> AddedTags)
{
	for (FGameplayTag& activeActionTag : AddedTags)
	{
		if (!ActiveGameplayTags.HasTag(activeActionTag))
		{
			ActiveGameplayTags.AddTag(activeActionTag);
		}
	}
}

void UActionComponent::AddBlockedActionTags(TArray<FGameplayTag> AddedTags)
{
	for (FGameplayTag& blockedActionTag : AddedTags)
	{
		if (!BlockedGameplayTags.HasTag(blockedActionTag))
		{
			BlockedGameplayTags.AddTag(blockedActionTag);
		}
	}
}

bool UActionComponent::HasBlockedTag(FGameplayTag BlockedTag)
{
	return BlockedGameplayTags.HasTag(BlockedTag);
}