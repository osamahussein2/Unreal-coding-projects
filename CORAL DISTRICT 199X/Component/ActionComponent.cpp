// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ActionComponent.h"

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

	if (GetOwner())
	{
		OnAddAction.AddUniqueDynamic(this, &UActionComponent::AddAction);
		OnRemoveAction.AddUniqueDynamic(this, &UActionComponent::RemoveAction);
	}
}


// Called every frame
void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActionComponent::AddAction(FString action_)
{
	if (!HasAction(action_)) actions.Emplace(action_);
}

void UActionComponent::RemoveAction(FString action_)
{
	if (HasAction(action_)) actions.Remove(action_);
}

bool UActionComponent::HasAction(FString action_) const
{
	return actions.Contains(action_);
}