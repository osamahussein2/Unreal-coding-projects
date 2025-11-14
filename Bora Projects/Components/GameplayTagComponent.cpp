// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/GameplayTagComponent.h"

// Sets default values for this component's properties
UGameplayTagComponent::UGameplayTagComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UGameplayTagComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGameplayTagComponent::RemoveActionTag(FName tagName_)
{
	// We need to be sure the gameplay tag is active first before we remove it
	if (HasActionTag(tagName_))
	{
		GameplayTags.RemoveTag(FGameplayTag::RequestGameplayTag(tagName_));
	}
}

void UGameplayTagComponent::AddActionTag(FName tagName_)
{
	GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(tagName_));
}

bool UGameplayTagComponent::HasActionTag(FName tagName_)
{
	return GameplayTags.HasTag(FGameplayTag::RequestGameplayTag(tagName_));
}
