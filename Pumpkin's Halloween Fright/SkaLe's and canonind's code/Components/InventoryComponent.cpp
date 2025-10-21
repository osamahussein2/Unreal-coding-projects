// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"


UInventoryComponent::UInventoryComponent()
{



}

void UInventoryComponent::AddCandy()
{
	CandyCounter++;
	OnCandyPickedUp.Broadcast(CandyCounter);
}

void UInventoryComponent::EatCandy()
{
	if (HasCandy())
	{
		CandyCounter--;
		OnCandyEaten.Broadcast(CandyCounter);
	}
}

uint32 UInventoryComponent::GetCandyCount() const
{
	return CandyCounter;
}

float UInventoryComponent::GetCandyHealAmount() const
{
	return CandyHealthAmount;
}

bool UInventoryComponent::HasCandy() const
{
	return CandyCounter > 0;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();


	
}


